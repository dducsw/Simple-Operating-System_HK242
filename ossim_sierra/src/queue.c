#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */
        if (proc == NULL) return;
        if (q->size == MAX_QUEUE_SIZE) return;
        else q->proc[q->size++] = proc;
}

struct pcb_t * dequeue(struct queue_t * q) {
        if (empty(q)) return NULL;
    
        struct pcb_t* pcb_return = q->proc[0];
    
        for (int i = 0; i < q->size - 1; i++) {
            q->proc[i] = q->proc[i + 1];
        }
    
        q->size--;
        return pcb_return;
    }
