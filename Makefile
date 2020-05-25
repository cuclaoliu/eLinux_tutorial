##Makefile
MODULE_NAME = drvdemo
CONFIG_MODULE_SIG=n
ifneq ($(KERNELRELEASE),)
#$(MODULE_NAME)-objs := drvdemo.o
obj-m := $(MODULE_NAME).o
else
KERNEL_DIR = /lib/modules/`uname -r`/build
MODULEDIR := $(shell pwd)
.PHONY: modules
default: all
all: modules drvdemo_write drvdemo_read
modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(MODULEDIR) modules
drvdemo_read:
	$(CC) -o $@ $@.c
drvdemo_write:
	$(CC) -o $@ $@.c
install: modules
	#插入模块
	insmod $(MODULE_NAME).ko
	#创建节点
	mknod $(MODULE_NAME) c 267 0 -m 666
	#显示内核信息，最后5行
	dmesg | tail -n 5
uninstall:
	rm $(MODULE_NAME) -f
	rmmod $(MODULE_NAME)
	#显示内核信息，最后5行
	dmesg | tail -n 5
clean distclean:
	rm -f *.o *.mod.c .*.*.cmd *.ko Mod*.* mo*.*
	rm -rf .tmp_versions
endif
