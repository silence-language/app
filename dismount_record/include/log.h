#ifndef _LOG_H_
#define _LOG_H_

#define LOGFILE "/home/root/log.txt"

int LOGInit(void);
void LOGD(const char *msg, ...);
void LOGE(const char *msg, ...);

#endif
