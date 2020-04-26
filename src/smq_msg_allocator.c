#include "smq_msg_allocator.h"

#include <linux/vmalloc.h>
#include <linux/printk.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

const unsigned int BUFFER_SIZE = PAGE_SIZE * 120;

static struct smq_msg_allocator_cb {
    char *buffer;
    char *end_buffer;
    unsigned int buffer_size;
    char *circle_buf_head;
    char *circle_buf_tail;
    list_head all_msgs;
    struct spinlock_t cb_spinlock;
} smq_msg_allocator_cb_g;

static void get_msg(smq_msg_t* msg)
{
    atomic_inc(&msg->ref_cnt);
}

static void put_msg(smq_msg_t* msg)
{
    struct list_head *next;

    if (atomic_dec_return(&msg->ref_cnt) == 0) {
        next = list_next(&msg->global_list_node);
        list_del(&msg->global_list_node);

        if (next == NULL) {
            smq_msg_allocator_cb_g.circle_buf_head = smq_msg_allocator_cb_g.buffer;
            smq_msg_allocator_cb_g.circle_buf_tail = smq_msg_allocator_cb_g.buffer;
            return;
        }
        
        msg = list_entry(next, smq_msg_t, global_list_node);
        if (msg->global_list_node == list_head(&smq_msg_allocator_cb_g->all_msgs)) {
            smq_msg_allocator_cb_g.circle_buf_head = (char*) msg;
        }
    }
}

bool smq_msg_allocator_init(void)
{
    smq_msg_allocator_cb_g.buffer = vmalloc(BUFFER_SIZE);

    if (smq_msg_allocator_cb_g.buffer == NULL) {
        printk(KERN_INFO "Can't allocate buffer in smq_msg_allocator_init errno - %d", errno);
        return FALSE;
    }

    smq_msg_allocator_cb_g.buffer_size = BUFFER_SIZE;
    smq_msg_allocator_cb_g.end_buffer = smq_msg_allocator_cb_g.buffer  + smq_msg_allocator_cb_g.buffer_size; 
    smq_msg_allocator_cb_g.circle_buf_head = smq_msg_allocator_cb_g.buffer;
    smq_msg_allocator_cb_g.circle_buf_tail = smq_msg_allocator_cb_g.buffer;
    smq_msg_allocator_cb_g.unique_cnt = ATOMIC_INIT(0);
    smq_msg_allocator_cb_g.msg_cnt = ATOMIC_INIT(0);
    INIT_LIST_HEAD(&smq_msg_allocator_cb_g.all_msgs);
    spin_lock_init(&smq_msg_allocator_cb_g.cb_spinlock);

    return TRUE;
}

void smq_msg_allocator_deinit(void)
{
    vfree(smq_msg_allocator_cb_g.buffer);
}

smq_msg_t* smq_alloc_message(unsigned int size)
{

    smq_msg_t *new_msg = NULL;
    
    spin_lock(&smq_msg_allocator_cb_g.cb_spinlock);

    new_msg = (smq_msg_t*) (smq_msg_allocator_cb_g.circle_buf_tail + size);    

    if (((char*) new_msg) + size + sizeof(smq_msg_t) > smq_msg_allocator_cb_g.end_buffer) {
        new_msg = smq_msg_allocator_cb_g.buffer;
    }

    new_msg->msg_id = (unsigned long int) new_msg;
    new_msg->size = size;
    new_msg->offset = ((char*) new_msg) + sizeof(smq_msg_t) - smq_msg_allocator_cb_g.buffer;
    new_msg->ref_cnt = ATOMIC_INIT(1);
    get_msg(new_msg);

    list_add_tail(&new_msg->global_list_node, &smq_msg_allocator_cb_g.all_msgs);
    
    smq_msg_allocator_cb_g.circle_buf_tail = ((char*) new_msg) + size + sizeof(smq_msg_t);
    smq_msg_allocator_cb_g.unique_cnt += 1;

    spin_unlock(&smq_msg_allocator_cb_g.cb_spinlock);

    printk(KERN_INFO "Allocate new message at place %p", new_msg);

    return new_msg;
}

void smq_free_message(smq_msg_id_t msg_id)
{
    struct list_head *pos, *q;
    smq_msg_t *msg = (smq_msg_t*) msg_id;

    spin_lock(&smq_msg_allocator_cb_g.cb_spinlock);

    put_msg(msg);

    spin_unlock(&smq_msg_allocator_cb_g.cb_spinlock);
}
