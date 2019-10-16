#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

LinkedQueue* sort_queue(LinkedQueue* queue) //selection sort for queue
{
  if(queue->next==NULL)
    return queue;
  LinkedQueue *prev, *here, *min;
  min = queue->next;
  prev = queue;
  here = queue->next;
  while(here->next!=NULL)
  {
    if(min->proc.process_id > here->next->proc.process_id)
    {
      prev = here;
    }
  }
  prev->next = min->next;
  min->next = queue->next;
  queue->next = min;
  queue->next = sort_queue(queue->next);
  return queue;
}

//this part is going to simulate how it the MLFQ is going to work, instead of actually running it
void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

    int all_proc_done = 1, q_p[queue_num], q_max = queue_num - 1, q_n[queue_num], q_ts[queue_num], qpointer;//sch var
    Process current_proc;
    current_proc.process_id = 0;
    LinkedQueue *cproc_buf = NULL;
    memset(q_p, 1, queue_num);
    memset(q_n, 0, queue_num);
    for(int k=0;k<queue_num;k++)
      q_ts[k] = ProcessQueue[k]->time_slice;
    int current_time=0, started_time = 0;
    int proc_in[proc_num], proc_out[proc_num];
    memset(proc_in,0,proc_num*sizeof(int));
    memset(proc_out,1,proc_num*sizeof(int));
    //use service time to store execution time
    while(all_proc_done != 0)
    {
      //reset
      //choose and print
      for(int k=0;k<proc_num;k++)
      {
        //in q
        if(proc[k].arrival_time <= current_time && proc_in==0)
        {
          ProcessQueue[q_max] = AddTail(ProcessQueue[q_max], proc[k]);
          proc_in[k] = 1;
          q_n[q_max]++;
          ProcessQueue[q_max] = sort_queue(ProcessQueue[q_max]);
          LinkedQueue *tmp = Find(proc[k]);
          tmp->time_slice = ProcessQueue[q_max]->time_slice;
          tmp->allotment_time = ProcessQueue[q_max]->allotment_time;
        }
      }

      /*
      current process update
      */

      if(started_time==cproc_buf->time_slice)//?
      {
        for(qpointer=q_max;qpointer>=0;qpointer++)//look for top queue
        {
          if(ProcessQueue[qpointer]->next!=NULL)
          break;
        }
        if(cproc_buf->time_slice == ProcessQueue[qpointer]->time_slice)//unmoved process
        {
          cproc_buf->proc = current_proc;
          //rr
          if(cproc_buf->next != NULL)
          {
            if(cproc_buf->allotment_time == 0)
            {
              //move down
              LinkedList *tmp_buf = cproc_buf;
            }
            else
            {
              cproc_buf = cproc_buf->next;
            }
            current_proc = cproc_buf->proc;
            current_proc.service_time++;
            cproc_buf->proc = current_proc;
          }
        }
      }
      //outprint();
      /*
      if all proc done -> all_proc_done = 0
      */
      //out
      all_proc_done = 0;
      for(int k = 0; k<proc_num; k++)
      {
        all_proc_done += proc_out[k];
      }
      current_time++;
    }

}
