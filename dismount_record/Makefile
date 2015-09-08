
APP ?= pc
SRC = main.c log.c config.c serial.c timer.c record.c queue.c socket_server.c at91sama5d3_gpio.c
#CC = arm-none-linux-gnueabi-gcc
CC = arm-buildroot-linux-uclibcgnueabihf-gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread
INSTALLDIR ?= ~/source/fs/fs_source/jffs2/bin

all: ${SRC}
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${APP} ${SRC}
	cp ${APP} $(INSTALLDIR)
	cp ${APP} /nfs
install: all
	cp ${APP} $(INSTALLDIR)


clean:
	rm -f ${APP} *~ *.o 
