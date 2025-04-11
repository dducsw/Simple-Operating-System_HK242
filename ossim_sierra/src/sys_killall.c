/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

 #include "common.h"
 #include "syscall.h"
 #include "stdio.h"
 #include "libmem.h"
 #include "queue.h"
 #include <stdlib.h>
 #include <string.h>
 
 int __sys_killall(struct pcb_t *caller, struct sc_regs *regs)
 {
     char proc_name[100];
     uint32_t data;
 
     // hardcode for demo only
     uint32_t memrg = regs->a1;
 
     /* TODO: Get name of the target proc */
     // proc_name = libread..
     int i = 0;
     data = 0;
     while (data != -1)
     {
         libread(caller, memrg, i, &data);
         proc_name[i] = data;
         if (data == -1)
             proc_name[i] = '\0';
         i++;
     }
     printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);
 
     /* TODO: Traverse proclist to terminate the proc
      *       stcmp to check the process match proc_name
      */
     // caller->running_list
     // caller->mlq_ready_queu
 
     /* TODO Maching and terminating
      *       all processes with given
      *        name in var proc_name
      */
 
     struct pcb_t *proc = caller->running_list;
     struct queue_t *mlq = caller->mlq_ready_queue;
     int count = 0;
     if (proc != NULL && strcmp(proc->path, proc_name) == 0)
     {
         printf("Process %s terminated\n", proc->path);
         free(proc->code->text);
         free(proc->code);
         free(proc);
         count++;
     }
     for (int q = 0; q < mlq->size; q++)
     {
         struct pcb_t *temp = mlq->proc[q];
         if (temp != NULL && strcmp(temp->path, proc_name) == 0)
         {
             printf("Process %s terminated\n", temp->path);
             free(temp->code->text);
             free(temp->code);
             free(temp);
             count++;
             mlq->proc[q] = NULL;
         }
     }
     printf("Total %d processes terminated\n", count);
     return 0;
}
 