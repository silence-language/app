#include "serial.h"
extern int SerialWritefd;
int PrintFile(char *filename)
{
    char buf[BUFFERSIZE];
    FILE *fp;
    int ret = 0;

    printf("================PrintFile\n");
    int num = 0;
    fp = fopen(filename, "r");
    //PrinterThreadPause();
    if (NULL == fp) {
        LOGE("Can't open file");
        return -1;
    }

    do {
        
        ret = fread(buf, sizeof(char), BUFFERSIZE, fp);
        //printf("%d data read!\n", ret);
        if (-1 == SerialWrite(SerialWritefd, buf, ret))
        //if (-1 == write_para(buf, ret))
        {
            printf("Printer Error!");
            return -1;
        }
        //msSleep(155000);
        /* printf("print %d data!\n", ret);
        num++;
        if(num%2 == 0) { 
            printf("I am going to sleep for 5s!\n");
            sleep(5);
        } */
    } while(BUFFERSIZE == ret);
    //PrinterThreadResume();
    fclose(fp);

    return 0;        
}
