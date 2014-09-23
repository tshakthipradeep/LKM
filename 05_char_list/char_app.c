#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[])
{
	int fd = 0, ret = 0, i = 0;
	char buff[10] = "";

	if(argc != 3)
	{
		printf("Usage: %s cmd(1/2) arg\n",argv[0]);
		exit(0);
	}

	int cmd = atoi(argv[1]);
	int arg = atoi(argv[2]);

	fd = open("/dev/07_char_list",O_RDWR);

	printf("fd = %d\n",fd);

	if(fd > 0)
	{
		if(cmd == 1)
		{
			for(i=0;i<sizeof(buff);i++)
				buff[i] = arg;

			ret = write(fd,buff,sizeof(buff));
			printf("Writing %d bytes\n",ret);
		}
		else if(cmd == 2)
		{
			ret = read(fd,buff,sizeof(buff));
			printf("Reading %d bytes\n",ret);

			for(i=0;i<sizeof(buff);i++)
			{
				if(buff[i] != arg)
				{
					printf("Invalid data found: %d %d %d %d %d\n",buff[0],buff[1],buff[2],buff[3],buff[4]);
					close(fd);
					exit(-1);
				}
			}
		}

		close(fd);
	} 		
}
