obj-m += rtc.o

KDIR = /home/shakthi/Documents/Projects/AM335x_SK/Project/ti-sdk-am335x-evm-06.00.00.00/board-support/linux-3.2.0-psp04.06.00.11
#KDIR = /home/shakthi/Documents/Projects/AM335x_SK/Project/ti-sdk-am335x-evm-07.00.00.00/board-support/linux-3.12.10-ti2013.12.01
#KDIR = /home/shakthi/Documents/Projects/CVS_REPO/64-Alpha-T_Viper/User_Area/WORKSPACE/Source_Code/HOST_Board/Board_Support_Package/dexcel-am335x
#UNAME = $(shell uname -r)

#KDIR = /usr/src/linux-headers-$(UNAME)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
