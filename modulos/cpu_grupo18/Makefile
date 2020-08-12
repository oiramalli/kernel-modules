obj-m += cpu_grupo18.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	dmesg -C
	insmod cpu_grupo18.ko
	rmmod cpu_grupo18.ko
	dmesg