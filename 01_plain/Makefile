obj-m += plain.o

UNAME = $(shell uname -r)

KDIR = /usr/src/linux-headers-$(UNAME)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
