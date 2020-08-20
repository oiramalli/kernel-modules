obj-m += cpu_grupo18.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
test:
	sudo dmesg -C
	sudo insmod cpu_grupo18.ko
	sudo rmmod cpu_grupo18.ko
	dmesg