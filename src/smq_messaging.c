#include "smq_messaging.h"

#include <linux/list.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include "smq_msg_allocator.h"

#define MAX_GROUPS_NUMBER 100
#define MAX_SUBSCRIBERS_NUMBER 100

static DECLARE_WAIT_QUEUE_HEAD(wq_process);

static struct smq_messaging_cb {
    smq_group_t groups[MAX_GROUPS_NUMBER];
    smq_subscriber_t subscribers[MAX_SUBSCRIBERS_NUMBER];
} smq_cb_g;

static smq_msg_t* get_next_msg(smq_subscriber_t* subscriber)
{
    if (subscriber->next_msg->next != &subscriber->group->msg_queue) {
        return list_entry(subscriber->next_msg->next, struct smq_msg_t, queue_node);
    }

    return NULL;
}

bool smq_messaging_init(void)
{

}

void smq_send_msg(smq_group_id_t group_id, smq_msg_id_t msg_id)
{
    struct smq_msg_t *msg = NULL;
    struct smq_group_t *group = &smq_cb_g.groups[group_id];
    
    spin_lock(&cb_spinlock);

    msg = smq_get_msg_struct(msg_id);
    list_add_tail(&msg->queue_node, &group->msg_queue);
    wake_up(&wq_process);
    atomic_add(&msg->ref_cnt, group->cnt_subscribers);

    spin_unlock(&cb_spinlock);
}

smq_msg_t* smq_get_msg(smq_subscriber_id_t subscriber_id)
{
    struct smq_subscriber_t *subscriber = &smq_cb_g.subscribers[subscriber_id];
    struct smq_msg_t *msg = NULL;

    spin_lock(&cb_spinlock);

    msg = get_next_msg(subscriber);
    if (msg != NULL) {
        subscriber->next_msg = &msg->queue_node;
    }

    spin_unlock(&cb_spinlock);

    return msg;
}

void smq_create_group(char *group_name)
{
    struct smq_group_t *new_group = NULL;
    unsigned int i;

    spin_lock(&cb_spinlock);

    for (i = 0; i < MAX_GROUPS_NUMBER; ++i) {
        new_group = &smq_cb_g.groups[i];
        if (!new_group->is_valid) {
            break;
        }
    }

    new_group->id = i;
    new_group->is_valid = true;
    new_group->cnt_subscribers = 0;
    new_group->subscribers_list.next = &new_group->subscribers_list;
    new_group->subscribers_list.prev = &new_group->subscribers_list;
    new_group->msg_queue.next = &new_group->msg_queue;
    new_group->msg_queue.prev = &new_group->msg_queue;
    strncpy(new_group->name, group_name, MAX_NAME_LENGHT);
    
    spin_unlock(&cb_spinlock);
}

smq_subscriber_id_t smq_subscribe_group(char *group_name)
{
    struct smq_group_t *group = NULL;
    struct smq_subscriber_t *new_subscriber = NULL;
    int i;

    spin_lock(&cb_spinlock);

    for (i = 0; i < MAX_GROUPS_NUMBER; ++i) {
        group = &smq_cb_g.groups[i];
        if (strncmp(group_name, group->name, MAX_NAME_LENGHT) == 0) {
            break;
        }
    }

    for (i = 0; i < MAX_SUBSCRIBERS_NUMBER; ++i) {
        new_subscriber = &smq_cb_g.subscribers[i];
        if (!new_subscriber->is_valid) {
            break;
        }
    }

    new_subscriber->id = i;
    new_subscriber->is_valid = true;
    new_subscriber->group = group;
    new_subscriber->next_msg = group->msg_queue.prev;

    spin_unlock(&cb_spinlock);

    return new_subscriber->id;
}

void smq_unsubscribe_group(smq_subscriber_id_t subscriber_id)
{
    /* to do */
}

void smq_poll(smq_subscriber_id_t subscriber_id)
{
    smq_subscriber_t *subscriber = &smq_cb_g.subscribers[subscriber_id];
    wait_event(wq_process, (get_next_msg(subscriber) != NULL));
}