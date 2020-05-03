#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

#include <smq_operations_user.h>

int main()
{
    int smq_fd = open("/dev/SharedMQDev1", O_RDWR);
    int res = 0;

    unsigned int offset, value;
    char op;

    if (smq_fd <= 0) {
        std::cerr << "failed to open SharedMQ " << strerror(errno);
        return -1;
    }

    struct smq_alloc_msg_op_t alloc_msg;
    struct smq_free_buf_op_t free_msg;

    char *buf = NULL;
    buf = (char*) mmap(NULL, 200000, PROT_READ | PROT_WRITE, MAP_SHARED, smq_fd, 0);

    if (buf == NULL) {
        std::cerr << "failed to mmap " << strerror(errno) << '\n';
    }

    while (true) {
        std::cin >> op;
        switch (op)
        {
        case 'r':
            std::cin >> offset;
            std::cout << "value " << buf[offset] << '\n';
            break;
        case 'w':
            std::cin >> offset >> value;
            buf[offset] = value;    
            std::cout << "set " << value << " for " << offset << '\n';
            break;
        case 'a':
            std::cin >> alloc_msg.size;
            res = ioctl(smq_fd, IOCTL_ALLOC_MSG, &alloc_msg);
            std::cout << "Alloc message result: res : " << alloc_msg.result << ", msg_id : " << alloc_msg.msg_id << ", offset : " << alloc_msg.offset << " ioctl " << res << '\n';
            break;
        case 'f':
            std::cin >> free_msg.msg_id;
            std::cout << "Before send free\n";
            res = ioctl(smq_fd, IOCTL_FREE_MSG, &free_msg);
            std::cout << "Free message result: res : " << free_msg.result << " ioctl " << res << '\n';
            break;
        }
    }

    return 0;
}