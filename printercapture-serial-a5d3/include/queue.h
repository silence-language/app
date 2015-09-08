#ifndef __QUEUE_H_
#define __QUEUE_H_
#include <pthread.h>
#include <semaphore.h>
#define MAXELEMENTS 100
#define UNIT 21773
#define FileNameLength 40
typedef struct
{
  char FileName[FileNameLength];
  long unsigned int length;
}PrinterRecord;
typedef struct
{
  pthread_mutex_t lock;
  int Capacity;
  int Size;
  int Front;
  int Rear;
  PrinterRecord *Record[MAXELEMENTS];
}QueueRecord;
extern QueueRecord *FileQueue;
int CreatAndPushRecord(char *);
int IsEmpty(QueueRecord *queue);
int IsFull(QueueRecord *queue);
QueueRecord CreateQueue(int MaxElements);
void DisposeQueue(QueueRecord* queue);
int MakeEmpty(QueueRecord*);
int Enqueue(QueueRecord *queue, PrinterRecord* record);
PrinterRecord* Front(QueueRecord* queue);
PrinterRecord* Dequeue(QueueRecord *queue);
//PrinterRecord FrontAndDequeue(QueueRecord queue);

#endif
