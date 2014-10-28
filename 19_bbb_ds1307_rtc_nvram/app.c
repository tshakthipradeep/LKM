#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NVRAM_SIZE	5

int main(int argc, char *argv[])
{
	int ret = 0;
	int count = 0;
	int index = 0;

	unsigned char nvram[NVRAM_SIZE] = "";

	int fd = open ("/sys/class/i2c-dev/i2c-3/device/3-0068/nvram",O_RDWR);

	if(fd < 0)
	{
		printf("Error opening NVRAM file\n");
		exit(0);
	}
	else
	{
		ret = read(fd,nvram,NVRAM_SIZE);

		printf("NVRAM read: %d\n",ret);
		printf("NVRAM: ",ret);

		for(index = 0; index < 5; index++)
			printf("%d ",nvram[index]);

		printf("\n");

		for(index = 0; index < NVRAM_SIZE; index++)
			nvram[index] = index;

		ret = write(fd,nvram,NVRAM_SIZE);

		printf("NVRAM write: %d\n",ret);
                printf("NVRAM: ");

		memset(nvram,0,NVRAM_SIZE);

		ret = read(fd,nvram,NVRAM_SIZE);
		printf("NVRAM read: %d\n",ret);

                for(index = 0; index < 5; index++)
                        printf("%d ",nvram[index]);

                printf("\n");
	}
}
