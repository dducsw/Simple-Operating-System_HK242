// #ifdef MM_PAGING
/*
 * PAGING based Memory Management
 * Virtual memory module mm/mm-vm.c
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "mm.h"
#include "os-mm.h"
#include "string.h"

/*get_vma_by_num - get vm area by numID
 *@mm: memory region
 *@vmaid: ID vm area to alloc memory region
 *
 */
struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid) {
  if (mm == NULL)
    return NULL;

  struct vm_area_struct *pvma = mm->mmap;
  if (mm->mmap == NULL)
    return NULL;

  int vmait = pvma->vm_id;

  while (vmait < vmaid) {
    if (pvma == NULL)
      return NULL;

    pvma = pvma->vm_next;
    vmait = pvma->vm_id;
  }

  return pvma;
}

int __mm_swap_page(struct pcb_t *caller, int vicfpn, int swpfpn) {
  __swap_cp_page(caller->mram, vicfpn, caller->active_mswp, swpfpn);
  return 0;
}

/*get_vm_area_node - get vm area for a number of pages
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
struct vm_rg_struct *get_vm_area_node_at_brk(struct pcb_t *caller, int vmaid,
                                             int size, int alignedsz) {
  struct vm_rg_struct *newrg;
  /* TODO retrive current vma to obtain newrg, current comment out due to
   * compiler redundant warning*/
  // Get the virtual memory based on the provided ID
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (cur_vma == NULL)
    return NULL;

  // Allocated memory for the new virtual memory region
  newrg = malloc(sizeof(struct vm_rg_struct));
  // Calculate the start and end address of new region
  int start_address =
      cur_vma->sbrk; // start = break point (end of allocated heap) of VMA
  int end_address = start_address + size; // end = start + requested size

  /* TODO: update the newrg boundary
  // newrg->rg_start = ...
  // newrg->rg_end = ...
  */
  // Set the start and end address of new region
  newrg->rg_start = start_address;
  newrg->rg_end = end_address;

  return newrg;
}

/*validate_overlap_vm_area
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, int vmastart,
                             int vmaend) {
  struct vm_area_struct *vma = caller->mm->mmap;

  /* TODO validate the planned memory area is not overlapped */
  if (caller == NULL)
    return -1;
  // Get virtual memory area (vma) based on provided id
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  // Iterate through all vma based on process's mem map
  while (vma != NULL) {
    // Check if current vma is not the target vma
    if (vma->vm_id != vmaid) {
      if (vmaend > vma->vm_start && vmastart < vma->vm_end && vma != cur_vma) {
        return 1; // Memory area overlapped
      }
    }
    // Move to next vma in the list
    vma = vma->vm_next;
  }

  // No overlapped found
  return 0;
}

/*inc_vma_limit - increase vm area limits to reserve space for new variable
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@inc_sz: increment size
 *
 */
int inc_vma_limit(struct pcb_t *caller, int vmaid, int inc_sz) {
  // Allocate memory for a new virtual memory (vm) region
  struct vm_rg_struct *newrg = malloc(sizeof(struct vm_rg_struct));
  // Align increment size to the page size.
  int inc_amt = PAGING_PAGE_ALIGNSZ(inc_sz);
  // Calculate the number of pages required for the increment
  int incnumpage = inc_amt / PAGING_PAGESZ;
  // Get vm region node at break point for requested increment
  struct vm_rg_struct *area =
      get_vm_area_node_at_brk(caller, vmaid, inc_sz, inc_amt);
  // Get vm area (vma) based on provided id
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  // Store old end address of the vma before increment
  int old_end = cur_vma->vm_end;

  /*Validate overlap of obtained region */
  if (validate_overlap_vm_area(caller, vmaid, area->rg_start, area->rg_end) < 0)
    return -1; /*Overlap and failed allocation */

  /* TODO: Obtain the new vm area based on vmaid */
  // cur_vma->vm_end...
  //  inc_limit_ret...
  //  Increase the end address and break point of the vma by requested size
  cur_vma->vm_end += inc_sz;
  cur_vma->sbrk += inc_sz;

  // Map the new reversed vm space to physical RAM.
  if (vm_map_ram(caller, area->rg_start, area->rg_end, old_end, incnumpage,
                 newrg) < 0) {
    return -1; /* Map the memory to MEMRAM */
  }

  // Increase vma limit successful
  return 0;
}

// #endif
