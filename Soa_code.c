
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);
void printscreen(int time_x, int time_y, int pid, int arrival_time, int remaining_time)
{
  printf("Time_slot:%d-%d, pid: %d, arrival-time:%d, remaining_time:%d\n",time_x,time_y,pid,arrival_time,remaining_time);
}
void selectQueue(LinkedQueue* qq,Process input)
{
  LinkedQueue* pt=qq;
  LinkedQueue* node=(LinkedQueue*)malloc(sizeof(LinkedQueue));
  node->proc=input;
  node->next=NULL;
  if(pt->next==NULL)
    pt->next=node;
  else{
  while(pt->next!=NULL)
  {
    if(pt->next->proc.process_id>node->proc.process_id)
    {
      node->next=pt->next;
      pt->next=node;
      break;
    }
    pt=pt->next;
    if(pt->next==NULL){
      pt->next=node;
      break;
    }
  }
  }
}
void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }
    printf("\n\n");
    int current_time=0,done=1,toRunq,start_time,last_arrive=0,empty=0,newfd=0;
    LinkedQueue* toRun=NULL;
    LinkedQueue* toPoint=NULL;
    LinkedQueue* RR=NULL;
    for(int i=0;i<proc_num;i++)
    {
      proc[i].completion_time=0;
      proc[i].service_time=0;
      if(proc[i].arrival_time>last_arrive)
        last_arrive=proc[i].arrival_time;
    }
    while(done)
    {
      //check whether refresh the queue
      if(((current_time % period) ==0)&&(current_time!=0))
      {
        if(toRun!=NULL)
        {
          outprint(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,toRun->proc.execution_time-toRun->proc.completion_time);
          printscreen(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,toRun->proc.execution_time-toRun->proc.completion_time);
          printf("%d %d %d\n",current_time,toRunq,toRun->proc.service_time);
          toRun->proc.service_time=0;
          toRun=NULL;
          toPoint=NULL;
          RR=NULL;
        }
        for(int i=queue_num-2;i>-1;i--)
        {
          while(!IsEmptyQueue(ProcessQueue[i]))
          {
            Process tmp=DeQueue(ProcessQueue[i]);
            selectQueue(ProcessQueue[queue_num-1],tmp);
          }
        }
      }
      //check whether a new process input
      for(int i=0;i<proc_num;i++)
      {
        if(current_time==proc[i].arrival_time){
          newfd=1;
          selectQueue(ProcessQueue[queue_num-1],proc[i]);
        }
      }
      //printf("finished checking\n");
      //choose which to run
      if(toRun==NULL){  //choose a new process
        for(int i=queue_num-1;i>-1;i--)
        {
          //printf("fault in first if\n");
          if(ProcessQueue[i]->next!=NULL)//something inside
          {
            //printf("fault in second if\n");
            if(ProcessQueue[i]->next->next==NULL) //only one
            {
              //printf("safe\n");
              toRun=ProcessQueue[i]->next;
              toRunq=i;
              RR=NULL;
            }
            else if(newfd || (RR==NULL)) //more than one and first RR
            {
              toRun=ProcessQueue[i]->next;
              toRunq=i;
              RR=NULL;
            }
            else //RRing but ignore new process's pid
              toRun=RR;
          start_time=current_time;
          newfd=0;
          break;
          }
        }
      }
      current_time++;
      //to run a process
      if(toRun)
      {
        toRun->proc.completion_time++;
        toRun->proc.service_time++;
        //check finished
        if(toRun->proc.completion_time==toRun->proc.execution_time)
        {
          outprint(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,0);
          printscreen(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,0);
          printf("%d %d %d\n",current_time,toRunq,toRun->proc.service_time);
          toPoint=ProcessQueue[toRunq];
          while(toPoint->next!=toRun)
            toPoint=toPoint->next;
          toPoint->next=toRun->next;
          //check whether in RRing
          if(ProcessQueue[toRunq]->next!=NULL)   //not empty
          {  
            if(ProcessQueue[toRunq]->next->next!=NULL) //still have to RR
            {
              if(toPoint->next==NULL)
                RR=ProcessQueue[toRunq]->next;
              else
                RR=toPoint->next;
            }
          }
          toRun=NULL;
          toPoint=NULL;
          continue;
        }
        //not finished check slice and allowment
        if(((toRun->proc.service_time%ProcessQueue[toRunq]->time_slice)==0)&&(toRun->proc.service_time!=0))
        {
          int drop=0;  //detect wheter dropped a queue;
          outprint(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,toRun->proc.execution_time-toRun->proc.completion_time);
          printscreen(start_time,current_time,toRun->proc.process_id,toRun->proc.arrival_time,toRun->proc.execution_time-toRun->proc.completion_time);
          printf("%d %d %d\n",current_time,toRunq,toRun->proc.service_time);
          if((toRun->proc.service_time==ProcessQueue[toRunq]->allotment_time)&&(toRunq>0))  //whether use out of allot_time and not the bottom queue
          {
            toPoint=ProcessQueue[toRunq];
            while(toPoint->next!=toRun)
            toPoint=toPoint->next;
            toPoint->next=toRun->next;
            Process tmp=toRun->proc;
            tmp.service_time=0;
            selectQueue(ProcessQueue[toRunq-1],tmp); //drop a queue
            drop=1;  //dropped
          }
          if(!drop)
            toPoint=toRun;
          //check whether in RRing
          if(ProcessQueue[toRunq]->next!=NULL)   //not empty
          {  
            if(ProcessQueue[toRunq]->next->next!=NULL) //still have to RR
            {
              if(toPoint->next==NULL)
                RR=ProcessQueue[toRunq]->next;
              else
                RR=toPoint->next;
            }
          }
          if(((current_time % period) ==0)&&(current_time!=0))  //check period
            toRun->proc.service_time=0;
          toRun=NULL;
          toPoint=NULL;
          continue;
        }
      }
      /*if(current_time==508)
      {
        QueuePrint(ProcessQueue[5]);
      }*/
      //check done
      empty=1;
      for(int i=0;i<queue_num;i++)
      {
        if(!IsEmptyQueue(ProcessQueue[i]))
          empty=0;
      }
      if(empty && (current_time>last_arrive))
        done=0;
      //current_time++;
    }
       //Test outprint function, it will output "Time_slot:1-2, pid:3, arrival-time:4, remaining_time:5" to output.loc file.
    //QueuePrint(ProcessQueue[5]);
    //outprint(1,2,3,4,5);

}

