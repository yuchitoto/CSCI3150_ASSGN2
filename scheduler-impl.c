#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_queue.h"

typedef struct ProcessQ
{
  int proc_inbuf_no;
  Process some_process;
} ProcessQ;

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

int compar(const void *a, const vois *b)
{
  ProcessQ *pA = (ProcessQ*)a;
  ProcessQ *pB = (ProcessQ*)b;
  return (pA->some_process->process_id - pB->some_process->process_id);
}

//void base_rr()

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

    /*
       Test outprint function, it will output "Time_slot:1-2, pid:3, arrival-time:4, remaining_time:5" to output.loc file.
    */
    int all_proc_done = 1, current_time = proc[0].arrival_time, current_proc = 0, curS = 1;
    int proc_exe_time[proc_num] = {0}, proc_q_no[proc_num] = {5};//why initial 5?
    int timebuf;
    int base_n = 0, base_p = 0;
    ProcessQ base_pid[proc_num];
    while(all_proc_done != 0)
    {
      //reset
      if(current_time>curS*period)
      {
        curS++;
        for(int k=0;k<proc_num;k++)
          proc_q_no[k] = (proc_q_no[k]==-1)?-1:5;
      }
      current_proc = 0;
      for(int chk_proc = 0; chk_proc < proc_num; current_proc++) //infinite loop? what's the aim of this loop？what's the meaning of chk?
      {
        if(proc[chk_proc].arrival_time >= current_time && (proc[chk_proc].execution_time - proc_exe_time[chk_proc])!=0)
        {
          current_proc = (proc_q_no[chk_proc]>proc_q_no[current_proc])?chk_proc:current_proc;
        }
      }
      if(proc_q_no[current_proc]==0)  //what's the meaning of this if statement
      {
        current_proc = base_pid[base_p]->proc_inbuf_no;  //base_pid is not be initialed.(changed . to ->)
        base_p++;
        base_p = (base_p<base_n)?base_p:0;  //base_n is 0;
      }

      if(proc_q_no[current_proc]==0)
        current_proc = base_pid[base_p];  //wrong type;
      outprint(current_time, (timebuf = current_time + (ProcessQueue[proc_q_no[current_proc]]->time_slice < (proc[current_proc].execution_time - proc_exe_time[current_proc]))?ProcessQueue[proc_q_no[current_proc]]->time_slice:(proc[current_proc].execution_time - proc_exe_time[current_proc])), proc[current_proc].process_id, proc[current_proc].arrival_time, proc[current_proc].execution_time - proc_exe_time[current_proc]);
      current_time = timebuf;
      proc_exe_time[current_proc] += (ProcessQueue[proc_q_no[current_proc]]->time_slice < (proc[current_proc].execution_time - proc_exe_time[current_proc]))?ProcessQueue[proc_q_no[current_proc]]->time_slice:(proc[current_proc].execution_time - proc_exe_time[current_proc]);
      if(proc_exe_time[current_proc] == proc[current_proc].execution_time)  //what's the meaning of this whole if statement?
      {
        if(proc_q_no[current_proc]==0)
        {
          int b=0;
          for(int k=0;k<base_n;k++)  //base_n is 0
          {
            if(base_pid[k].some_process.process_id == proc[current_proc].process_id)
              b=1;
            if(b==1 && k+1<base_n)
            {
              base_pid[k] = base_pid[k+1];
            }
          }
          base_n--;
        }
        proc_q_no[current_proc] = -1;
      }
      else
        proc_q_no[current_proc] -= (proc_q_no[current_proc]>0)?1:0;  //time_slice is not equal to allowmenttime
      if(proc_q_no[current_proc] == 0)
      {
        base_pid[base_n].proc_inbuf_no = current_proc;
        base_pid[base_n].some_process = proc[current_proc];
        base_n++;
        if(base_p>1)
          qsort(base_pid, base_n, sizeof(ProcessQ), compar);
      }
      /*
      if all proc done -> all_proc_done = 0
      */
      all_proc_done = 0;
      for(int k = 0; k<proc_num; k++)
      {
        all_proc_done += proc[k].execution_time - proc_exe_time[k];
      }
    }
    outprint(1,2,3,4,5);

}
