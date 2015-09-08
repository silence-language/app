#include "include/queue.h"
#include "include/general.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
QueueRecord *FileQueue = NULL;
int IsEmpty(QueueRecord* queue)
{
  return queue->Size==0;
}
int IsFull(QueueRecord* queue)
{
  return queue->Size==queue->Capacity;
}
int MakeEmpty(QueueRecord* queue)
{
  queue->Rear = 0;
  queue->Front = 1;
  queue->Size = 0;
  queue->Capacity = MAXELEMENTS;
}
static int Succ(int Value, QueueRecord *queue)
{
  if (++Value==queue->Capacity)
    {
      Value = 0;
    }
  return Value;
}
int Enqueue(QueueRecord *queue, PrinterRecord *Record)
{
    //LOGD("%s(%d):%s", __FILE__, __LINE__,__FUNCTION__);
    pthread_mutex_lock(&(queue->lock));
  if(IsFull(queue))
  {
      //LOGE("Full queue");
    pthread_mutex_unlock(&(queue->lock));
    return -1;
  }
  else
  {
    ++queue->Size;
    queue->Rear = Succ(queue->Rear, queue);
    queue->Record[queue->Rear] = Record;
    pthread_mutex_unlock(&(queue->lock));
  }
  
  //LOGD("%s(%d):%s", __FILE__, __LINE__,__FUNCTION__);
  return 0;
}
PrinterRecord* Dequeue(QueueRecord * queue)
{
    PrinterRecord * tmp;
    pthread_mutex_lock(&(queue->lock));
    if(IsEmpty(queue))
        {
            LOGD("Queue empty!\n");
            pthread_mutex_unlock(&(queue->lock));
            return NULL;
        }
    else
        {
            tmp = queue->Record[queue->Front];
            //free(queue->Record[queue->Front]->data);
            queue->Front = Succ(queue->Front, queue);
            --queue->Size;
            pthread_mutex_unlock(&(queue->lock));
        return tmp;
            //queue->Front = (queue->Front, queue);
        }
    
}
int CreatAndPushRecord(char* filename)
{
  PrinterRecord *mem;  
  LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
  mem = (PrinterRecord*)malloc(sizeof(PrinterRecord));
  LOGD("In %s(%d)\n", __FUNCTION__, __LINE__); 
 if (mem!=NULL)
  {
      strncpy(mem->FileName, filename,strlen(filename));//save the filename as a member of queue 
  LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
    if (-1 == Enqueue(FileQueue, mem))//push the member failed
    {
      free(mem);
  LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
      return -1;
    }
      LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
  }
  else
  {
    perror(NULL);
    LOGD("%s(%d)-%s:",__FILE__,__LINE__,__FUNCTION__);
    return -1;
  }
  return 0;

}
int CreatQueue(int MaxElements)
{
  FileQueue = (QueueRecord*)malloc(sizeof(QueueRecord));
   if (NULL==FileQueue)     
   {
       PRINTERR("Failed to creat queue!");
       exit(-1);
       // return -1;
   }
   else
   {
     MakeEmpty(FileQueue);
     pthread_mutex_init(&(FileQueue->lock), NULL);
     return 0;
   }
}
void DisposeQueue(QueueRecord *queue)
{
  free(queue);
}
