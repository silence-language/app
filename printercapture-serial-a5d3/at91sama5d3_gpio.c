#include "at91sama5d3_gpio.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
int gpio_set_level(int fd, IOCTL_GPIO_ARG_ST *parg)
{
    int i;
    int ret = RET_SUCCESS;

    if (NULL == parg)
	{
        return RET_FAILED;
	}
	if ((GPIO_LEVEL_LOW != parg->gpio_level) && (GPIO_LEVEL_HIGHT != parg->gpio_level))
    	{
        DEBUG_ERROR("Invalid argument!\n"); 
        return RET_FAILED;
	}	
    ret = ioctl(fd, IOCTL_GPIO_LEVEL_SET, parg);
   return ret;
}

int gpio_get_level(int fd, 	IOCTL_GPIO_ARG_ST *parg)
{
    int i;
    int ret = RET_SUCCESS;
    
    if (NULL == parg)
	{
        DEBUG_ERROR("Invalid argument!\n"); 
        return RET_FAILED;
	}
    ret = ioctl(fd, IOCTL_GPIO_LEVEL_GET, parg);
 
   return ret;
}

int gpio_dir_set_input(int fd, IOCTL_GPIO_ARG_ST *parg)
{
    int ret = RET_SUCCESS;
    
    if (NULL == parg)
	{
        DEBUG_ERROR("Invalid argument!\n"); 
        return RET_FAILED;
	}
	ret = ioctl(fd, IOCTL_GPIO_DIR_INPUT_SET, parg);
  
    return ret;
}

