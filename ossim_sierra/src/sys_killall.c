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

int __sys_killall(struct pcb_t *caller, struct sc_regs* regs)
{
    char proc_name[100];
    uint32_t data;

    //hardcode for demo only
    uint32_t memrg = regs->a1;
    
    /* TODO: Get name of the target proc */
    //proc_name = libread..
    int i = 0;
    data = 0;
    while(data != -1){
        libread(caller, memrg, i, &data);
        proc_name[i]= data;
        if(data == -1) proc_name[i]='\0';
        i++;
    }
    printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

    /* TODO: Traverse proclist to terminate the proc
     *       stcmp to check the process match proc_name
     */
    //caller->running_list
    //caller->mlq_ready_queu

    /* TODO Maching and terminating 
     *       all processes with given
     *        name in var proc_name
     */
    pthread_mutex_lock(&queue_lock);
    struct pcb_t *proc = dequeue(&running_list);
    while (proc != NULL){
        if (strcmp(proc->name, proc_name) == 0){
            printf("Terminating process %s with pid %d\n", proc->name, proc->pid);
            free(proc);
        } else {
            enqueue(&running_list, proc);
        }
        proc = dequeue(&running_list);
    }
    pthread_mutex_unlock(&queue_lock);

#ifdef MLQ_SCHED
    for (int prio = 0; prio < MAX_PRIO; prio++) {
        pthread_mutex_lock(&queue_lock);
        proc = dequeue(&mlq_ready_queue[prio]);
        while (proc != NULL) {
            if (strcmp(proc->name, proc_name) == 0) {
                printf("Terminating process %s with PID %d\n", proc->name, proc->pid);
                free(proc); 
            } else {
                enqueue(&mlq_ready_queue[prio], proc); 
            }
            proc = dequeue(&mlq_ready_queue[prio]);
        }
        pthread_mutex_unlock(&queue_lock);
    }
#else
    pthread_mutex_lock(&queue_lock);
    proc = dequeue(&ready_queue);
    while (proc != NULL) {
        if (strcmp(proc->name, proc_name) == 0) {
            printf("Terminating process %s with PID %d\n", proc->name, proc->pid);
            free(proc); 
        } else {
            enqueue(&ready_queue, proc);
        }
        proc = dequeue(&ready_queue);
    }
    pthread_mutex_unlock(&queue_lock);
#endif

    return 0; 
}
