#include "smq_msg_allocator.h"

#include <linux/vmalloc.h>
#include <linux/printk.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/list.h>

const unsigned int BUFFER_SIZE = PAGE_SIZE * 120;

static DEFINE_SPINLOCK(cb_spinlock);

static LIST_HEAD(all_msgs);

static struct smq_msg_allocator_cb {
    char *buffer;
    char *end_buffer;
    unsigned int buffer_size;
    char *circle_buf_head;
    char *circle_buf_tail;
} smq_msg_allocator_cb_g;

static show_all_messages(void)
{
    struct list_head *node;
    struct smq_msg_t *msg;
    printk(KERN_INFO "PRINT ALL MESSAGES \nstart - %u, end - %u, head - %u, tail - %u, head_list - %p, tail_list - %p, list - %p ", 
        (void*) smq_msg_allocator_cb_g.buffer, (void*) smq_msg_allocator_cb_g.end_buffer, (void*) smq_msg_allocator_cb_g.circle_buf_head, 
        (void*) smq_msg_allocator_cb_g.circle_buf_tail, all_msgs.prev, all_msgs.next,
        &all_msgs);
    list_for_each(node, &all_msgs) {
        msg = list_entry(node, struct smq_msg_t, global_list_node);
        printk(KERN_INFO "msg_id - %u, size - %u, offset - %u, left - %p, right - %p", msg->msg_id, msg->size, msg->offset,
            msg->global_list_node.prev, msg->global_list_node.next);
    }
}

static void get_msg(smq_msg_t* msg)
{
    atomic_inc(&msg->ref_cnt);
}

static void put_msg(smq_msg_t* msg)
{
    struct list_head* next;

    if (atomic_dec_return(&msg->ref_cnt) == 0) {
        next = msg->global_list_node.next;
        list_del(&msg->global_list_node);

        printk(KERN_INFO "Deliting, next - %p, msg - %p", next, msg);

        if (next == &all_msgs) {
            smq_msg_allocator_cb_g.circle_buf_head = smq_msg_allocator_cb_g.buffer;
            smq_msg_allocator_cb_g.circle_buf_tail = smq_msg_allocator_cb_g.buffer;
            return;
        }

        if (next == all_msgs.prev) {
            msg = list_entry(next, struct smq_msg_t, global_list_node);
            smq_msg_allocator_cb_g.circle_buf_head = (char*) msg;
        }
    }
}

bool smq_msg_allocator_init(void)
{
    smq_msg_allocator_cb_g.buffer = vmalloc(BUFFER_SIZE);

    if (smq_msg_allocator_cb_g.buffer == NULL) {
        printk(KERN_INFO "Can't allocate buffer in smq_msg_allocator_init errno");
        return false;
    }

    smq_msg_allocator_cb_g.buffer_size = BUFFER_SIZE;
    smq_msg_allocator_cb_g.end_buffer = smq_msg_allocator_cb_g.buffer  + smq_msg_allocator_cb_g.buffer_size; 
    smq_msg_allocator_cb_g.circle_buf_head = smq_msg_allocator_cb_g.buffer;
    smq_msg_allocator_cb_g.circle_buf_tail = smq_msg_allocator_cb_g.buffer;
    
    return true;
}

void smq_msg_allocator_deinit(void)
{
    vfree(smq_msg_allocator_cb_g.buffer);
}

smq_msg_t* smq_alloc_message(unsigned int size)
{
    printk(KERN_INFO "Start allocate msg with size - %u", size);

    smq_msg_t *new_msg = NULL;
    
    spin_lock(&cb_spinlock);

    new_msg = (smq_msg_t*) (smq_msg_allocator_cb_g.circle_buf_tail);    

    if (((char*) new_msg) + size + sizeof(smq_msg_t) > smq_msg_allocator_cb_g.end_buffer) {
        new_msg = (smq_msg_t*) smq_msg_allocator_cb_g.buffer;
    }

    new_msg->msg_id = (unsigned long int) new_msg;
    new_msg->size = size;
    new_msg->offset = ((char*) new_msg) + sizeof(smq_msg_t) - smq_msg_allocator_cb_g.buffer;
    INIT_LIST_HEAD(&new_msg->global_list_node);
    atomic_set(&new_msg->ref_cnt, 0);
    get_msg(new_msg);

    list_add_tail(&new_msg->global_list_node, &all_msgs);
    
    smq_msg_allocator_cb_g.circle_buf_tail = ((char*) new_msg) + size + sizeof(smq_msg_t);

    show_all_messages();

    spin_unlock(&cb_spinlock);

    printk(KERN_INFO "Allocate new message at place %u", (void*) new_msg);

    return new_msg;
}

void smq_free_message(smq_msg_id_t msg_id)
{
    smq_msg_t *msg = (smq_msg_t*) msg_id;

    printk(KERN_INFO "Free message at place %u", (void*) msg);

    spin_lock(&cb_spinlock);

    put_msg(msg);

    show_all_messages();

    spin_unlock(&cb_spinlock);
}

char* smq_get_buffer_start(void)
{
    return smq_msg_allocator_cb_g.buffer;
}