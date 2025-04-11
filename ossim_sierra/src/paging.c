
#include "mem.h"
#include "cpu.h"
#include "loader.h"
#include <stdio.h>

int main() {
	struct pcb_t * ld = load("input/os_0_mlq_paging");
	struct pcb_t * proc = load("input/os_0_mlq_paging");
	unsigned int i;
	for (i = 0; i < proc->code->size; i++) {
		run(proc);
		run(ld);
	}
	dump();
	return 0;
}

