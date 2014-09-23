#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char *argv[],int argc)
{
	int fd = 0;

	fd = open("/dev/06_char",O_RDWR);

	printf("fd = %d\n",fd);

	if(fd > 0)
	{
		close(fd);
		printf("Closing fd\n");
	} 		
}
