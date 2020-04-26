#ifndef SMQ_MESSAGING_H
#define SMQ_MESSAGING_H

typedef unsigned int group_id;

bool smq_messaging_init(void);

group_id smq_get_group_id(char* group_name);

bool smq_subscribe_group(char* group_name);



#endif