#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "util.h"

#define SERIAL0 "/dev/ttyS0"
#define SERIAL1 "/dev/ttyS1"
#define SERIAL2 "/dev/ttyS2"
#define SERIAL3 "/dev/ttyS3"
#define SERIAL4 "/dev/ttyS4"
#define SERIAL5 "/dev/ttyS5"
#define SERIAL6 "/dev/ttyS6"

int SerialInit(struct serialConf *sc);
int SerialOpen(char *nComport);
void SerialClose(int Serialfd);
int SerialOpt(int Serialfd, int nSpeed, int nBits, int nFctl, char nEvent, int nStop);
int SerialRead(int Serialfd, char *ReadBuf, int ReadNum);
int SerialWrite(int Serialfd, char *WriteBuf, int WriteNum);

#endif 
