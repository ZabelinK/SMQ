#ifndef SMQ_OPERATIONS_H
#define SMQ_OPERATIONS_H

#include "smq_operations_user.h"

int smq_get_sh_name_fn(void *);
int smq_alloc_msg_fn(void *);
int smq_free_buf_fn(void *);
int smq_send_msg_fn(void *);
int smq_recv_msg_fn(void *);
int smq_create_new_recipients_group_fn(void *);
int smq_subscribe_on_group_fn(void *);
int smq_unsubsribe_from_group_fn(void *);

#endif