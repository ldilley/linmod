obj-m += linmod.o linmod2.o linmod_dev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	-rmmod linmod2
	-rmmod linmod
	modinfo linmod.ko
	insmod linmod.ko
	insmod linmod2.ko
	@dmesg | tail | grep linmod | tail -n3
	@lsmod | head -n1
	@lsmod | grep linmod
	rmmod linmod2
	rmmod linmod
