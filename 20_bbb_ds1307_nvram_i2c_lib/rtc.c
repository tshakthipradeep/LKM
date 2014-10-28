#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#define DS1307_I2C_ADDR		0x68
#define DS1307_NVRAM_ADDR	0x08

int main(int argc, char *argv[])
{
	int fd = 0;
	int ret = 0;
	unsigned char buf[8] = "";

	/*
	* Open I2C bus 3 to which DS1307 RTC is connected
	*/

	fd = open("/dev/i2c-3",O_RDWR);

	if(fd < 0)
	{
		printf("Error: Could not open /dev/i2c-3 interface\n");
		exit(1);
	}
	else
	{
		/*
		* Set I2C address to DS1307 I2C address i.e 0x68
		*/

		if (ioctl(fd, I2C_SLAVE, DS1307_I2C_ADDR) < 0) 
		{
			printf("Error: Could not i2c address\n");
			exit(1);
		}
		else
		{
			/*
			* Before write is performed to the NVRAM of the RTC, address pointer of DS1307
			* has to be set to the address of NVRAM (0x08). This can be done by sending 0x08
			* as first byte of the write command followed by the data which we want 
			* to write. In the below test we are writing 0x01 to 0x07 to the SRAM.
			*/

			buf[0] = DS1307_NVRAM_ADDR;
			buf[1] = 0x01;
			buf[2] = buf[1] + 0x01;
			buf[3] = buf[2] + 0x01;
			buf[4] = buf[3] + 0x01;
			buf[5] = buf[4] + 0x01;
			buf[6] = buf[5] + 0x01;
			buf[7] = buf[6] + 0x01;

			ret = write(fd,buf,sizeof(buf));

			if(ret < 0)
			{
				printf("Error: Write to i2c bus failed\n");
				exit(1);
			}
			else
			{
				printf("Step 1: Address pointer set to 0x08 and data written to RTC NVRAM. ");
				printf("(%x %x %x %x %x %x %x)\n",buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
			}

			memset(buf,0,sizeof(buf));

			/*
			* Before read is performed from the SRAM, we have to reset the address pointer to
			* 0x08 and then perform read.
			*/

			buf[0] = DS1307_NVRAM_ADDR;

			ret = write(fd,buf,1);
                        if(ret < 0)
                        {
                                printf("Error: Write to i2c bus failed\n");
                                exit(1);
                        }
			else
			{
				printf("Step 2: Address pointer reset to 0x08\n");
			}

                        /*
                        * Now read selected number of bytes from the NVRAM
                        */

			memset(buf,0,sizeof(buf));
			ret = read(fd,buf,sizeof(buf));

			if(ret < 0)
                        {
                                printf("Error: Read from i2c bus failed\n");
                                exit(1);
                        }
			else
			{
				printf("Step 3: Data read from RTC NVRAM. ");
				printf("(%x %x %x %x %x %x %x)\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
			}
		}

	}

	return 0;
}
