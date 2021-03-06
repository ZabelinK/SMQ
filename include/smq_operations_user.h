#ifndef SMQ_OPERATIONS_USER_H
#define SMQ_OPERATIONS_USER_H
#include<linux/ioctl.h>

/*
    smq_get_sh_name - получить имя shared memory
    smq_alloc_buf_op - allocate new buffer
    smq_free_buf_op - освободить буфер сообщения (по настоящему освободится только когда все получатели и отправитель выполнят эту операцию)
    smq_send_msg_op - отправить сообщение определенной группе получателей
    smq_recv_msg_op - получить новое сообщение
    smq_create_new_recipients_group_op - создать новую группу получателей
    smq_subscribe_on_group_op - подписаться на определенную группу сообщений
    smq_unsubsribe_from_group_op - отписаться от группы
    smq_get_group_id_op - получить id группы по имени
*/

#define IOCTL_MAGIC_NUMBER 100
#define IOCTL_GET_SH_NAME _IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_ALLOC_MSG _IO(IOCTL_MAGIC_NUMBER, 1)
#define IOCTL_FREE_MSG _IO(IOCTL_MAGIC_NUMBER, 2)
#define IOCTL_SEND_MSG _IO(IOCTL_MAGIC_NUMBER, 3)
#define IOCTL_RECV_MSG _IO(IOCTL_MAGIC_NUMBER, 4)
#define IOCTL_CREATE_NEW_GROUP _IO(IOCTL_MAGIC_NUMBER, 5)
#define IOCTL_SUBSCRIBE_ON_GROUP _IO(IOCTL_MAGIC_NUMBER, 6)
#define IOCTL_UNSUBSCRIBE_FROM_GROUP _IO(IOCTL_MAGIC_NUMBER, 7)

#define IOCTL_MAGIC_MAX 

enum smq_operations {
    SMQ_GET_SH_NAME = 0,
    SMQ_ALLOC_MSG,
    SMQ_FREE_MSG,
    SMQ_SEND_MSG,
    SMQ_RECV_MSG,
    SMQ_CREATE_NEW_GROUP,
    SMQ_SUBSCRIBE_ON_GROUP,
    SMQ_UNSUBSCRIBE_FROM_GROUP,
};

enum smq_result {
    SMQ_RESULT_SUCCESS,
    SMQ_RESULT_ALREAY_CREATED,
    SMQ_NOT_ENOUGH_SPACE,
    SMQ_THERE_ARE_NOT_MESSAGES,
    SMQ_INVALID_GROUP_ID,
    SMQ_INVALID_MSG_ID,
    SMQ_INVALID_GROUP_NAME,
};

struct smq_get_sh_name_t {
    int           result;
    char         *sh_name;
    unsigned int  size;
};

struct smq_alloc_msg_op_t {
    int          result;
    unsigned int size;
    unsigned int offset;
    unsigned int msg_id;
};

struct smq_free_buf_op_t {
    int          result;
    unsigned int msg_id;
};

struct smq_send_msg_op_t {
    int          result;
    unsigned int msg_id;
    unsigned int receiver_group_id;
};

struct smq_recv_msg_op_t {
    int          result;
    unsigned int offset;
    unsigned int size;
    unsigned int msg_id;
};

struct smq_create_new_recioients_group_op_t {
    int           result;
    char         *group_name;
    unsigned int  size;
    unsigned int  group_id;
};

struct smq_subscribe_on_group_op_t {
    int result;
    unsigned int group_id;
};

struct smq_unsubsribe_from_group_op_t {
    int result;
    unsigned int group_id;
};

struct smq_get_group_id_op_t {
    int result;
    char *name;
    unsigned int size;
    unsigned int group_id;
};

#endif