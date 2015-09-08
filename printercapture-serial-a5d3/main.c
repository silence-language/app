#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include "include/net.h"
#include "include/util.h"
#include "include/log.h"
#include "include/config.h"
#include "include/serial.h"
#include "include/timer.h"
#include "include/record.h"
#include "include/socket_server.h"
#include "at91sama5d3_gpio.h"
pthread_t threadPCSerial;
pthread_t threadPrinterSerial;
pthread_t threadSocket;
pthread_t threadServerForgoahead;
#define FAILED perror("FAILED")
#define BUF_MAX 4096
typedef struct 
{
    int data_length;
    pthread_mutex_t data_lock;
    char data[BUF_MAX];
} MESSAGE;

MESSAGE Message= {0};
MESSAGE Response = {0};
void pthreadSerial(void *message);
int fd_socket;
static int SerialReadfd;
static int SerialWritefd;
int GPIOFD = 0;
//pthread_mutex_t PCPrinterMutex;
//pthread_mutex_t PrinterPCMutex;
int GPIO_Init()
{
    IOCTL_GPIO_ARG_ST gpio_arg;
    GPIOFD = open(DEV_GPIO, O_RDWR);
    if (GPIOFD < 0)
        {
            DEBUG_ERROR("open gpio device failed!!\n");
            return -1;
        }
    gpio_arg.gpio_index = AT91_PIN_PA22;
    gpio_arg.gpio_level = GPIO_LEVEL_LOW;
    gpio_set_level(GPIOFD, &gpio_arg);
    gpio_arg.gpio_index = AT91_PIN_PA21;
    gpio_arg.gpio_level = GPIO_LEVEL_LOW;
    gpio_set_level(GPIOFD, &gpio_arg);
    close(GPIOFD);
    
    
}
int SerialInit(struct serialConf *sc) {
    int ret;
    char buf[10];
    Message.data_length = 0;
    Response.data_length = 0;
    //LOGD("Open serial...\n");
    //SerialReadfd = SerialOpen(SERIAL2);
    SerialWritefd = SerialOpen(SERIAL2);
    if(SerialWritefd < 0) {
        LOGE("threadSerialFunction SerialReadOpen failed.\n");
	goto fail_3 ;
	return -1;
    }
    LOGD("Open serial2 success!\n");
    // SerialWritefd = SerialOpen(SERIAL1);
    //SerialWritefd = SerialOpen(SERIAL3);
    SerialReadfd = SerialOpen(SERIAL4);
//	SerialWritefd = SerialReadfd;
    if(SerialReadfd < 0) {
        LOGE("threadSerialFunction SerialWriteOpen failed.\n");
            
        goto fail_2;
        /* return -1; */
    }
    
    LOGD("Open serial1 success!\n");
    /* while(1); */
    // ret = SerialOpt(SerialReadfd, 9600, 8, 0, 'N', 1);
    ret = SerialOpt(SerialReadfd, sc->speed, sc->bits, sc->fctl, sc->event, sc->stop);
    if(ret < 0)
        {
            LOGE("threadSerialFunction SerialReadOpt failed.\n");
            goto fail_1;
        }

    //ret = SerialOpt(SerialWritefd, 9600, 8, 0, 'N', 1);
    /* ret = SerialOpt(SerialWritefd, sc->speed, sc->bits, sc->fctl, sc->event, sc->stop); */
    ret = SerialOpt(SerialWritefd, sc->speed, sc->bits, sc->fctl, sc->event, sc->stop);
    if(ret < 0) {
        LOGE("threadSerialFunction SerialWriteOpt failed.\n");
	goto fail_1;
    }
    /* SerialRead(SerialReadfd, &(buf[0]), 5); */
    /* SerialRead(SerialWritefd, &(buf[0]), 5); */
    return 0;
 fail_1: 
    close(SerialWritefd);
 fail_2:
    close(SerialReadfd);
 fail_3:
    return -1;
}
void *ReadRecordFileAndSendToSocket()
{
    char buf[BUF_MAX];
    int ret = 0;
    //int socket;
    //fd_socket = client_init();
    //socket = client_init();
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0)
        {
            PRINTERR("Creat Socket Failed! \n");
            exit(1);
        }
    if(bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)))
        {
            PRINTERR("Client bind port failed!\n");
            exit(1);
        }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.9.95");
    server_addr.sin_port = htons(SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);
    if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length)<0)
        {
            PRINTERR("Can not Connect to 192.168.9.189");
            exit(1);
        }

     /* LOGD("In %s", __FUNCTION__); */
    for(;;)
        {
    /* LOGD("In %s", __FUNCTION__); */
            ret = ReadPrinterRecord(&buf[0], BUF_MAX);
            if(ret>0)
                {
                    //SendToServer(socket, buf, ret);
                    printf("Send data length is %d\n", ret);
                    send(client_socket, buf, ret, MSG_DONTWAIT);
                    //fsync(socket);
                }
            else if (ret == 0)
                {
                    sleep(10);
                }
            //else if (ret )
            //sleep(1);
        }
    close(client_socket);
}
void *threadSerialPCPrinterFunction(void *message) {
    int ret;
    char SerialPOSBuf[BUF_MAX];
    time_t CurrentTime = 0 ;
    time_t LastTime = 0;
    int i;
    int WriteFlag = 0;




    
    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (0!=ret)
        {
            perror("Thread pthread_setcancelstat failed");
            pthread_exit(NULL);
        } 
    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if (0!=ret)
        {
            perror("Thread pthread_setcanceltype failed");
            pthread_exit(NULL);
        }
    Message.data_length = 0;
    while(1) {
        pthread_mutex_lock(&(Message.data_lock));
        pthread_mutex_lock(&(Response.data_lock));
        if (Response.data_length>0)
            {
                int ret;
		printf("Write to PC %d Datas!\n", Response.data_length);
                ret = SerialWrite(SerialReadfd, &(Response.data[0]), Response.data_length);
                if(ret!=Response.data_length)
                    {
                        printf("Response to PC failed!");
                    }
                else
                    {
                        Response.data_length = 0;
                    }
            }
        pthread_mutex_unlock(&(Response.data_lock));
            
        if(Message.data_length<=0)
            {
                //printf("hhhhhhhhhh\n");
                //memset(&(Message.data[0]), '\0', BUF_MAX);
                //Message.data_length = SerialRead(SerialReadfd, &(Message.data[0]), sizeof(Message.data));
                Message.data_length = SerialRead(SerialReadfd, &(Message.data[0]), sizeof(Message.data));
                //printf("Data length = %d\n", Message.data_length);
                if(Message.data_length > 0) 
                    {
              
                        //#define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))

                        int n = 0;
                        printf("Received %d bytes data\n", Message.data_length);
                        LastTime = time(NULL);
                
        /*                for(n=0; n<Message.data_length; ++n)
                            {
                                printf("%x\t", Message.data[n]);
                            }*/
			//fflush(stdout);
                        WriteARecord(&Message.data[0], Message.data_length);//Write data to a file; 
                        //                LOGD("%s(%d)-%s:",__FILE__,__LINE__,__FUNCTION__);
                        WriteFlag = 1;
                   //      Message.data_length = 0; 
                        pthread_mutex_unlock(&Message.data_lock);
                        msSleep(10);       
                    }
                else
                    {
                        CurrentTime = time(NULL);
                        if (((CurrentTime - LastTime)>5)&&(1 == WriteFlag))
                            {
                                WriteFlag = 0;
                                WriteARecord(&Message.data[0], 0);//close the crrent file ,and creat a queue member;
                                printf("Creat file!\n");
                            }
                        /* printf("no data in serialread!\n"); */
                        pthread_mutex_unlock(&Message.data_lock);
                        msSleep(10);
                    }
            }
        else
            {
                /* printf("Length>0\n"); */
                pthread_mutex_unlock(&Message.data_lock);
                msSleep(10);
            }

    }
    pthread_exit(0);
}

void *threadSerialPrinterPCFunction(void *message) {
    int ret;
    char SerialPrinterBuf[BUF_MAX];
    int i;
    int fd;
    //ret = open
    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (0!=ret)
        {
            perror("Thread pthread_setcancelstat failed");
            pthread_exit(NULL);
        } 
    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if (0!=ret)
        {
            perror("Thread pthread_setcanceltype failed");
            pthread_exit(NULL);
        }
 
   
    while(1) {
        // memset(SerialPrinterBuf, '\0', BUF_MAX);
        /* printf("LOCK"); */
        
        pthread_mutex_lock(&Message.data_lock);
        pthread_mutex_lock(&Response.data_lock);
        
        if(Response.data_length<=0)
            {
                int n = 0;
                Response.data_length = SerialRead(SerialWritefd, &(Response.data[0]), sizeof(Response.data));
                
                if(Response.data_length>0)
                    {
                        printf("Receive Data length is = %d---------------------------------------------------\n", Response.data_length);
                        /*for(n=0; n<Response.data_length; ++n)
                            {
                                printf("%x\t", Response.data[n]);
                            }*/
                    }
               /* else
                    {
                        printf("NO DATA FROM REPONSE!\n");
                    }*/
            }
        pthread_mutex_unlock(&Response.data_lock);
        /* printf("Locked!"); */
	//SerialWrite(SerialWritefd, "fuckfuckfuckfuckfuckfuckfuck", 28);
        /* printf("Length= %d", Message.data_length); */
        // ret = SerialRead(SerialWritefd, SerialPrinterBuf, sizeof(SerialPrinterBuf));
        if(Message.data_length > 0) 
            {
                int tmp = 0;
                /* char tmp1[11]= "abcdeferty";//{a, b, c, d, e, f, g, g, g, g}; */
                /* SerialWrite(SerialWritefd, tmp1, 10); */
                /* Message.data_length = 0; */
                /* pthread_mutex_unlock(&Message.data_lock); */
                /* continue; */
                //tmp = 
                //tmp = SerialWrite(SerialWritefd, Message.data, Message.data_length);
                printf("in wirte!\n");
                tmp = SerialWrite(SerialWritefd, Message.data, Message.data_length);
                if (Message.data_length!=tmp)//write data to printer
                    {
                        printf("actual num = %d, therial num = %d\n", tmp, Message.data_length);
                        PRINTERR("Write wrong!\n")
                            //pthread_mutex_unlock(&Message.data_lock);
                            //pthread_exit(NULL);
                            }
                else
                    {
                        printf("Write to printer SUCCESS!\n");
                    }
                /* else */
                /* { */
                //send(fd_socket, Message.data, Message.data_length, 0);//for debug
                /* pthread_mutex_unlock(&Message.data_lock); */
                /* } */
                Message.data_length = 0;
            }
        pthread_mutex_unlock(&Message.data_lock);
    }
    pthread_exit(0);
    //return "successful";
}

void pthreadSerial(void *message) {
    int ret;
    void *threadPCResult;
    void *threadPrinterResult;

    ret = pthread_mutex_init(&Message.data_lock, NULL);
    if(ret != 0) {
        LOGE("PCPrinter Mutex initialization failed.\n");
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&threadPCSerial, NULL, threadSerialPCPrinterFunction, (void *)message);
    if(ret != 0) {
        LOGE("pthreadPCPrinterSerial creation failed.\n");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&threadPrinterSerial, NULL, threadSerialPrinterPCFunction, (void *)message);
    if(ret != 0) 
        {
            LOGE("pthreadPCPrinterSerial creation failed.\n");
            exit(EXIT_FAILURE);
        }
    /* ret = pthread_create(&threadSocket, NULL, ReadRecordFileAndSendToSocket, (void*)message); */
    /* if (ret != 0) */
    /*     { */
    /*         LOGE("Creat thread failed!"); */
    /*         exit(EXIT_FAILURE); */
    /*     } */
    /* ret = pthread_create(&threadServerForgoahead, NULL, server_init, (void*)message); */
    /* if(ret != 0) */
    /*     { */
    /*         LOGE("Creat thread threadServerForgoahead failed!"); */
    /*         exit(EXIT_FAILURE); */
    /*     } */
    /* SerialWrite(SerialWritefd,"adcadfadfadfadfasdfadfadf",20);     */
    ret = pthread_join(threadPCSerial, &threadPCResult);
    if(ret != 0) 
        {
            LOGE("pthreadSerial PC join failed.\n");
            exit(EXIT_FAILURE);
        }
    else
        {
            LOGD("success join threadPCSerial.\n");
        }

    ret = pthread_join(threadPrinterSerial, &threadPrinterResult);
    if(ret != 0) 
        {
            LOGE("pthreadSerial Printer join failed.\n");
            exit(EXIT_FAILURE);
        }
    else
        {
            LOGD("success join threadPrinterSerial.\n");
        }
    /* ret = pthread_join(threadSocket, &threadPrinterResult); */
    /* if(ret!=0) */
    /*     { */
    /*         LOGE("threadSocket Printer join failed.\n"); */
    /*         exit(EXIT_FAILURE); */
    /*     } */
    /* else */
    /*     { */
    /*         LOGD("success join threadSocket.\n"); */
    /*     } */
    /* ret = pthread_join(threadServerForgoahead, &threadPCResult); */
    /* if(ret != 0)  */
    /*     { */
    /*         LOGE("threadServerForgoahead join failed.\n"); */
    /*         exit(EXIT_FAILURE); */
    /*     } */
    /* else */
    /*     { */
    /*         LOGD("success join threadServerForgoahead.\n"); */
    /*     } */

    close(SerialReadfd);
    close(SerialWritefd);
    LOGD("pthreadSerial PC returned %s\n", (char *)threadPCResult);
    LOGD("pthreadSerial Printer returned %s\n", (char *)threadPrinterResult);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) 
{
    int ret = 0;
    struct serialConf sc;
    struct sigaction act;
    char RecordFileNumASC[20];

    //ReadRecordFileNum(NULL);
    // time_t timer = time(NULL);
    // struct tm *timesample;
    // timesample = localtime(&timer);
    // strftime(&RecordFileNumASC[0], 20,"%F-%H-%M-%S", timesample);
    // sscanf(&RecordFileNumASC[0],"%s \n", ctime(&timer));
    // printf("%s\n", RecordFileNumASC);
    //LOGD("I'am start......\n");
    // return 0;
    LOGInit();
    // LOGD("I'm reading config file......\n");
   
    GPIO_Init();
    ret = ConfigSerial(&sc);
    if(ret < 0) {
        LOGE("Read config file failed.\n");
        exit(EXIT_FAILURE);
    }
    //LOGD("I'm configing......\n");
    ret = SerialInit(&sc);
    if(ret < 0) {
        LOGE("Serial port initialization failed.\n");
        exit(EXIT_FAILURE);
    }
    LOGD("Config end!\n");
    /* #if 0 */
    /* FILE * file = fopen("/var/printer/1970-01-01000738","r"); */
    /* if (NULL == file) */
    /*   { */
    /*     LOGD("Failed to open file\n"); */
    /*     return 0; */
    /*   } */
    /* Message.data_length = fread(&Message.data[0], sizeof(char), 100, file); */
   
    /* do{ */
    /* int n =0; */
    // LOGD("data length is %d\n", Message.data_length);
    /* for (n=0;n<Message.data_length; ++n){ */
    /* printf("%x ", Message.data[n]); */
    /* SerialWrite(SerialWritefd, &Message.data[n], 1); */
    /* } */
    /* Message.data_length = fread(&Message.data[0], sizeof(char),100, file); */
    /* }while(0!=Message.data_length); */
    /* fclose(file); */
    /* return 0; */
    //#endif
    RecordFileInit();
    CreatQueue(0);
    // write()
/* SerialRead(SerialReadfd, &(Message.data[0]), sizeof(Message.data));     */
    LOGD("Record file initialize end!\n");

    // ReadRecordFileNum(&RecordFileNumASC[0]);
    /* LOGD("Read Record file num end!\n"); */
    //   CreatDataRecordFile(1);
    pthreadSerial("pthreadSerial");

    
    return ret;
}
