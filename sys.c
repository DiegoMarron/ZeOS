/*
 * sys.c - Syscalls implementation
 */

#include <devices.h>
#include <errno.h>
#include <utils.h>
#include <sched.h>
#include <mm.h>
#include <io.h>

#include<klib.h>

long sys_ni_syscall(void){
        return -ENOSYS;
}


// inlined becouse it is very small (1 if) at the moment
// otherway, this should not be inlined.
static __inline__ int check_valid_fd(int fd){

  if (fd==1) return 1;
 
  return 0;
}


int sys_write(int fd,char *buffer, int size)
{
  int ret;

  if (!check_valid_fd(fd)) return -EBADF;

  if (size <0) return -EINVAL;
  if (size==0) return 0;

  if (!access_ok(0,buffer,size)) return -EFAULT;

  // uncomment this line to cheat while running runjp for del 1 
  // it will avoid print the very long chain, while emulating
  // it has successfully done it. The porpouse is to see
  // the other tests error/results.
  //if (size > 1000) return size;

  //assume only console.. :)
  ret=sys_write_console(buffer,size);

  return ret;

}

///////////////////////////////////////////////////////

int sys_getpid(){
  return current->t_pid;
}

int sys_nice(int _q) {
  /*
  int old_q = current->t_prio;
  current->t_prio = _q;
  return old_q;
  */
  return -38;
}




int sys_fork(){

  task_t *nt;
  union task_union *ntu;
  
  int i;
  int ret=0;
  unsigned long p_off;

  nt = task_get_free_slot();
  if (!nt) return -ENOMEM;

  copy_data(current, nt, KERNEL_STACK_SIZE*sizeof(unsigned long));

  // TODO: check if there's free pagusr space <- maybe redundant?
  // as we have the same number of phys pages as log pages..

  ret=mm_alloc_frames(nt);
  if (!ret) { task_free_slot(nt); return -ENOMEM; }

  p_off = PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA;
  for (i = 0; i < NUM_PAG_DATA; i++)
    {
      set_ss_pag (p_off + i, nt->ph_frames[i]);
      copy_data ((void *) (PAGE2ADDR((PAG_LOG_INIT_DATA_P0+i))),
		 (void *) (PAGE2ADDR((PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i))),
	      PAGE_SIZE);
      del_ss_pag (nt->ph_frames[i]);
    }

  nt->t_pid = task_next_pid();
  nt->t_tics = 0;
  nt->t_cpu_time = 0;

  // flush tlb
  set_cr3();  

  //son gets 0 as a return value
  ntu=(union task_union*)nt;
  ntu->stack[KERNEL_STACK_SIZE -10] = 0;

  // add new task to run queue
  rq_add_to_active(nt,this_rq());

  // return son pid
  return nt->t_pid;
}


void sys_exit(){

}


int sys_sem_init(int n_sem, unsigned int value) {
  return -ENOSYS;
}


int sys_sem_destroy(int n_sem) {
  return -ENOSYS;
}


int sys_sem_wait(int n_sem) {
  return -ENOSYS;
}


int sys_sem_signal(int n_sem) {
  return -ENOSYS;
}


int sys_get_stats(int pid, struct stats *st) {
  return -ENOSYS;

}



