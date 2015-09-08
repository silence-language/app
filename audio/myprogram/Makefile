INCLUDE = -I/home/tao/work/gitlatest/pospal/audio/install/include/
CC = /opt/mktools/gcc-linaro-arm-linux-gnueabihf-4.7-2013.04-20130415_linux/bin/arm-linux-gnueabihf-gcc
LIBPATH := -L/home/tao/work/gitlatest/pospal/audio/install/lib/
LIBS := -lasound
SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))
target : $(OBJS)
	$(CC) -o lplay  $(LIBPATH) $(LIBS) wav_parser.o sndwav_common.o lplay.o
	$(CC) -o lrecord $(LIBPATH) $(LIBS) wav_parser.o sndwav_common.o lrecord.o
%.o : %.c
	$(CC) -c $(INCLUDE) $^ -o $@
.PHONY : clean
clean:
	rm -rf *.o lplay lrecord
