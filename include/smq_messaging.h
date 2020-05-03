#ifndef SMQ_MESSAGING_H
#define SMQ_MESSAGING_H

#include "smq_msg_allocator.h"

#define MAX_NAME_LENGHT 256

typedef unsigned int smq_group_id_t;
typedef unsigned int smq_subscriber_id_t;

typedef struct smq_group_t {
    smq_group_id_t id;
    char name[MAX_NAME_LENGHT];
    struct list_head msg_queue;
    struct list_head subscribers_list;
    unsigned int cnt_subscribers;
    bool is_valid;
} smq_group_t;

typedef struct smq_subscriber_t {
    smq_subscriber_id_t id;
    struct smq_group_t *group;
    struct list_head *next_msg;
    bool is_valid;
} smq_subscriber_t;

bool smq_messaging_init(void);

void smq_send_msg(smq_group_id_t group_id, smq_msg_id_t msg_id);

smq_msg_t* smq_get_msg(smq_subscriber_id_t subscriber_id);

void smq_create_group(char *group_name);

smq_subscriber_id_t smq_subscribe_group(char *group_name);

void smq_unsubscribe_group(smq_subscriber_id_t subscriber_id);

#endif