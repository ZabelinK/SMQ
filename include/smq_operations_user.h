#ifndef SMQ_OPERATIONS_USER_H
#define SMQ_OPERATIONS_USER_H

/*
    smq_get_sh_name - получить имя shared memory
    smq_alloc_buf_op - allocate new buffer
    smq_free_buf_op - освободить буфер сообщения (по настоящему освободится только когда все получатели и отправитель выполнят эту операцию)
    smq_send_msg_op - отправить сообщение определенной группе получателей
    smq_recv_msg_op - получить новое сообщение
    smq_create_new_recipients_group_op - создать новую группу получателей
    smq_subscribe_on_group_op - подписаться на определенную группу сообщений
    smq_unsubsribe_from_group_op - отписаться от группы
*/

enum smq_operations {
    SMQ_GET_SH_NAME,
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
};

struct smq_get_sh_name {
    int           result;
    char         *sh_name;
    unsigned int  size;
};

struct smq_alloc_msg_op {
    int          result;
    unsigned int size;
    unsigned int offset;
    unsigned int msg_id;
};

struct smq_free_buf_op {
    int          result;
    unsigned int msg_id;
};

struct smq_send_msg_op {
    int          result;
    unsigned int msg_id;
    unsigned int receiver_group_id;
};

struct smq_recv_msg_op {
    int          result;
    unsigned int offset;
    unsigned int size;
    unsigned int msg_id;
};

struct smq_create_new_recioients_group_op {
    int           result;
    char         *group_name;
    unsigned int  size;
    unsigned int  group_id;
};

struct smq_subscribe_on_group_op {
    int result;
    unsigned int group_id;
};

struct smq_unsubsribe_from_group_op {
    int result;
    unsigned int group_id;
};

#endif