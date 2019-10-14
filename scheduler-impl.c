#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

int compar(const void *a, const vois *b)
{
  return (*(int*)a - *(int*)b);
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
    int all_proc_done = 1, current_time = proc[0].arrival_time, current_proc = 0;
    int proc_exe_time[proc_num] = {0}, proc_q_no[proc_num] = {5};
    int timebuf;
    int base_pid[proc_num], base_n = 0, base_p = 0;
    while(all_proc_done != 0)
    {
      current_proc = 0;
      for(int chk_proc = 0; chk_proc < proc_num; current_proc++)
      {
        if(proc[chk_proc].arrival_time >= current_time && (proc[chk_proc].execution_time - proc_exe_time[chk_proc])!=0)
        {
          current_proc = (proc_q_no[chk_proc]>proc_q_no[current_proc])?chk_proc:current_proc;
        }
      }
      outprint(current_time, (timebuf = current_time + (ProcessQueue[proc_q_no[current_proc]]->time_slice < (proc[current_proc].execution_time - proc_exe_time[current_proc]))?ProcessQueue[proc_q_no[current_proc]]->time_slice:(proc[current_proc].execution_time - proc_exe_time)), proc[current_proc].process_id, proc[current_proc].arrival_time, proc[current_proc].execution_time - proc_exe_time[current_proc]);
      current_time = timebuf;
      proc_exe_time[current_proc] += (ProcessQueue[proc_q_no[current_proc]]->time_slice < (proc[current_proc].execution_time - proc_exe_time[current_proc]))?ProcessQueue[proc_q_no[current_proc]]->time_slice:(proc[current_proc].execution_time - proc_exe_time[current_proc]);
      proc_q_no[current_proc] -= (proc_q_no[current_proc]>0)?1:0;
      if(proc_q_no[current_proc] == 0)
      {
        base_pid[base_n] = proc[current_proc].process_id;
        base_n++;
        if(base_p>1)
          qsort(base_pid, base_n, sizeof(int), compar);
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
