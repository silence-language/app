#include <unistd.h>                                                                                                                                
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "include/log.h"

static int Logfd = 0;

int LOGInit(void) {
}

void LOGD(const char *msg, ...) 
{
    va_list argp;       /* 定义保存函数参数的结构 */
    int argno = 0;      /* 纪录参数个数 */
    char sprint_buf[1024];

    memset(sprint_buf, '\0', sizeof(sprint_buf));
    va_start(argp, msg);
    argno = vsprintf(sprint_buf, msg, argp);
    va_end(argp);        //将argp置为NULL
    printf("[Debug]%s", sprint_buf);
}

void LOGE(const char *msg, ...) 
{
    va_list argp;       /* 定义保存函数参数的结构 */
    int argno = 0;      /* 纪录参数个数 */
    char sprint_buf[1024];

    memset(sprint_buf, '\0', sizeof(sprint_buf));
    va_start(argp, msg);
    argno = vsprintf(sprint_buf, msg, argp);
    va_end(argp);        //将argp置为NULL
    printf("[Error]%s", sprint_buf);
}

