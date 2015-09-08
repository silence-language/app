#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#define DEV_NAME "/dev/spidev1.1"
//#define DEV_NAME "/dev/at91_spi1"

int main()
{
	int fd;
	int i;
	unsigned char buf[250]={0};
	/* unsigned char buf = 0x55;	 */
	for(i=0;i<250;i++)
	{
		buf[i] = i;
	}

	printf("AT91 SPI1 test ...\n");
	
	fd = open(DEV_NAME,O_RDWR);
	if(fd < 0){
		printf("open at91_spi1 failed ...\n");
		return -1;
	}

	while(1)
	{
  		write(fd,buf,sizeof(buf)/sizeof(char));
  		/* write(fd,&buf,1); */
		sleep(1);
	}

	close(fd);
	return 0;
}
