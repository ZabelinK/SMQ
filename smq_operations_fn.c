#include "smq_operations_fn.h"

#include <linux/printk.h>


int smq_get_sh_name_fn(void * p)
{
    printk(KERN_EMERG "smq_get_sh_name_fn");
    return 0;
}

int smq_alloc_msg_fn(void * p)
{
    printk(KERN_EMERG "smq_alloc_msg_fn");
    return 0;
}

int smq_free_buf_fn(void * p)
{
    printk(KERN_EMERG "smq_free_buf_fn");
    return 0;
}

int smq_send_msg_fn(void * p)
{
    printk(KERN_EMERG "smq_send_msg_fn");
    return 0;
}

int smq_recv_msg_fn(void * p)
{
    printk(KERN_EMERG "smq_recv_msg_fn");
    return 0;
}

int smq_create_new_recipients_group_fn(void * p)
{
    printk(KERN_EMERG "smq_create_new_recipients_group_fn");
    return 0;
}

int smq_subscribe_on_group_fn(void * p)
{
    printk(KERN_EMERG "smq_subscribe_on_group_fn");
    return 0;
}

int smq_unsubsribe_from_group_fn(void * p)
{
    printk(KERN_EMERG "smq_unsubsribe_from_group_fn");
    return 0;
}
