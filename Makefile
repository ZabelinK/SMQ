obj-m += smq.o 
smq-y += smq_main.o smq_operations_fn.o 

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean