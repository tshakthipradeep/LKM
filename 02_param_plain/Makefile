obj-m += plain.o

KDIR = /home/shakthi/Documents/Projects/AM335x_SK/Project/ti-sdk-am335x-evm-07.00.00.00/board-support/linux-3.12.10-ti2013.12.01

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
