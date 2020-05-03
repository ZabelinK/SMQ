obj-m += smq.o 
smq-y += ./src/smq_main.o ./src/smq_operations_fn.o ./src/smq_msg_allocator.o ./src/smq_messaging.o

EXTRA_CFLAGS=-I$(PWD)/include

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean