#ifndef SMQ_MSG_ALLOCATION_H
#define SMQ_MSG_ALLOCATION_H
#include <linux/list.h>
#include <asm/atomic.h>
#include <linux/spinlock.h>

extern spinlock_t cb_spinlock;

typedef unsigned long int smq_msg_id_t;

typedef struct smq_msg_t {
    smq_msg_id_t msg_id;
    unsigned int size;
    unsigned int offset;
    struct list_head queue_node;
    struct list_head global_list_node;
    atomic_t ref_cnt;
} smq_msg_t;

bool smq_msg_allocator_init(void);
smq_msg_t* smq_alloc_message(unsigned int size);
smq_msg_t* smq_get_msg_struct(smq_msg_id_t msg_id);
void smq_free_message(smq_msg_id_t msg_id);
char* smq_get_buffer_start(void);

#endif