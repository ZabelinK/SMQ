#include "smq_operations_fn.h"

#include <linux/printk.h>
#include <linux/uaccess.h>
#include "smq_msg_allocator.h"
#include "smq_operations_user.h"

int smq_get_sh_name_fn(void * p)
{
    printk(KERN_EMERG "smq_get_sh_name_fn");
    return 0;
}

int smq_alloc_msg_fn(void * cmd)
{
    struct smq_alloc_msg_op_t smq_cmd;
    copy_from_user((void*) &smq_cmd, cmd, sizeof(struct smq_alloc_msg_op_t));
    struct smq_msg_t* smq_message = smq_alloc_message(smq_cmd.size);

    smq_cmd.offset = smq_message->offset;
    smq_cmd.msg_id = smq_message->msg_id;
    smq_cmd.result = SMQ_RESULT_SUCCESS;

    copy_to_user(cmd, (void*) &smq_cmd, sizeof(struct smq_alloc_msg_op_t));

    printk(KERN_EMERG "smq_alloc_msg_fn");
    return 0;
}

int smq_free_buf_fn(void * cmd)
{
    printk(KERN_EMERG "smq_free_buf_fn");
    struct smq_free_buf_op_t smq_cmd;
    copy_from_user((void*) &smq_cmd, cmd, sizeof(struct smq_free_buf_op_t));
    smq_free_message(smq_cmd.msg_id);
    smq_cmd.result = SMQ_RESULT_SUCCESS;
    copy_to_user(cmd, (void*) &smq_cmd, sizeof(struct smq_free_buf_op_t));
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
