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
    here = here->next;
  }
  prev->next = min->next;
  min->next = queue->next;
  queue->next = min;
  queue->next = sort_queue(queue->next);
  return queue;
}

//this part is going to simulate how it the MLGB is going to work, instead of actually running it
void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

    int all_proc_done = 1, q_max = queue_num - 1, qpointer = -1;//sch var
    Process current_proc;
    current_proc.process_id = 0;
    LinkedQueue *cproc_buf = NULL, *tmp_holder = NULL;
    int current_time=0, started_time = 0, executed_time = 0;
    int proc_in[proc_num];
    memset(proc_in,0,proc_num * sizeof(int));

    for(int k=0; k<queue_num; k++)
      ProcessQueue[k]->next = NULL;

    while(all_proc_done != 0)
    {
      //reset
      printf("Current time: %d\nCurrent process: %d\n", current_time, current_proc.process_id);
      if(current_time % period==0)
      {
        LinkedQueue *entry = ProcessQueue[q_max];
        for(int k = 0; k<q_max; k++)
        {
          while(entry->next != NULL)
            entry = entry->next;

          entry->next = ProcessQueue[k]->next;
          ProcessQueue[k]->next = NULL;
        }
        ProcessQueue[q_max] = sort_queue(ProcessQueue[q_max]);
      }

      //insert job
      for(int k=0;k<proc_num;k++)
      {
        //in q
        if(proc[k].arrival_time <= current_time && proc_in[k]==0)
        {
          ProcessQueue[q_max] = EnQueue(ProcessQueue[q_max], proc[k]);
          proc_in[k] = 1;
          ProcessQueue[q_max] = sort_queue(ProcessQueue[q_max]);
          LinkedQueue *tmp = Find(ProcessQueue[q_max], proc[k]);
          tmp->time_slice = ProcessQueue[q_max]->time_slice;
          tmp->allotment_time = ProcessQueue[q_max]->allotment_time;
        }
      }

      //choose job
      if(executed_time==0 || current_time%period==0)
      {
         int new_proc_l = -1;
         for(int k; k<queue_num; k++)
         {
           new_proc_l = (ProcessQueue[k]->next==NULL)?new_proc_l:k;
         }
         printf("new qpointer: %d\n", new_proc_l);
         if(new_proc_l != -1)
         {
           if(new_proc_l == qpointer)//rr
           {
             cproc_buf = tmp_holder;
             current_proc = cproc_buf->proc;
           }
           else
           {
             qpointer = new_proc_l;
             cproc_buf = ProcessQueue[qpointer]->next;
             current_proc = cproc_buf->proc;
           }
           started_time = current_time;
         }
      }

      current_time++;

      if(cproc_buf != NULL)
      {
        //update job content
        current_proc.execution_time--;
        executed_time++;
        cproc_buf->allotment_time--;

        //outprint when period, executed time = slice, execution time=0
        if(current_time % period == 0 && current_time != 0)
        {
          outprint(started_time, current_time, current_proc.process_id, current_proc.arrival_time, current_proc.execution_time);
          cproc_buf->proc = current_proc;
          for(int k=0;k<proc_num;k++)
            if(proc[k].process_id == current_proc.process_id)
              proc[k] = current_proc;
        }
        if(executed_time == cproc_buf->time_slice || cproc_buf->allotment_time == 0 || current_proc.execution_time == 0)
        {
          outprint(started_time, current_time, current_proc.process_id, current_proc.arrival_time, current_proc.execution_time);
          cproc_buf->proc = current_proc;
          for(int k=0;k<proc_num;k++)
            if(proc[k].process_id == current_proc.process_id)
              proc[k] = current_proc;
          executed_time = 0;
        }

        //mv job down
        if(cproc_buf->allotment_time == 0 && qpointer > 0)
        {
          tmp_holder = cproc_buf->next;
          LinkedQueue *entry = ProcessQueue[qpointer]->next;
          while(entry->next != cproc_buf)
          {
            entry = entry->next;
          }
          entry->next = cproc_buf->next;
          cproc_buf->next = NULL;
          ProcessQueue[qpointer - 1] = AddTail(ProcessQueue[qpointer - 1], current_proc);
          entry = Find(ProcessQueue[qpointer - 1], current_proc);
          entry->allotment_time = ProcessQueue[qpointer-1]->allotment_time;
          entry->time_slice = ProcessQueue[qpointer - 1]->time_slice;
          ProcessQueue[qpointer - 1] = sort_queue(ProcessQueue[qpointer - 1]);
          free(cproc_buf);
          cproc_buf = NULL;
        }
        else if(cproc_buf->allotment_time == 0 && qpointer == 0)
        {
          cproc_buf->allotment_time = ProcessQueue[0]->allotment_time;
          tmp_holder = cproc_buf->next;
        }
      }

      /*
      if all proc done -> all_proc_done = 0
      */
      all_proc_done = 0;
      for(int k = 0; k<proc_num; k++)
      {
        all_proc_done += proc[k].execution_time;
      }
    }

}
