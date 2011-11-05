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
#include<stats.h>
#include<semaphore.h>


long sys_ni_syscall(void){
        return -ENOSYS;
}


// inlined becouse it is very small (1 if) at the moment
// otherway, this should not be inlined.
static __inline__ int check_valid_fd(int fd){

  if (fd==1) return 1;
 
  return 0;
}

#define K_WRITE_BUFF_SIZE 1024

int sys_write(int fd,char *buffer, int size)
{
  int ret;
  char k_buf[K_WRITE_BUFF_SIZE];
 
  int i = 0;
  unsigned int q = size / K_WRITE_BUFF_SIZE;
  unsigned int r = size % K_WRITE_BUFF_SIZE; 
  unsigned int b_jump;
  unsigned int total=0;

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
  b_jump=0;
  for(i = 0; i < q;++i){
    copy_from_user(buffer+b_jump, &k_buf, K_WRITE_BUFF_SIZE);
    ret=sys_write_console(&k_buf[0],K_WRITE_BUFF_SIZE);
    if (ret < 0) return ret;
    total += ret;
    b_jump += K_WRITE_BUFF_SIZE;
  }

  copy_from_user(buffer+b_jump, &k_buf, r);
  ret=sys_write_console(&k_buf[0],r);
  if (ret < 0) return ret;
  total += ret;

  return total;
  
}

///////////////////////////////////////////////////////

int sys_getpid(){
  return current->t_pid;
}

int sys_nice(int _q) {
  int old_q;
  if(_q <= 0) return -EINVAL;

  old_q = current->t_prio;
  current->t_prio = _q;
  return old_q;
}


int sys_fork(){

  task_t *nt,*cp;
  union task_union *ntu;
  
  int i;
  int ret=0;
  unsigned long p_off;

  rq_t *_rq=this_rq();

  //_rq = current->t_rq;

  //dump_regs();
  cp=current; 

  nt = task_get_free_slot(this_rq());
  if (!nt) return -ENOMEM;
   
  copy_data(cp, nt, KERNEL_STACK_SIZE*sizeof(int));

  // TODO: check if there's free pagusr space <- maybe redundant?
  // as we have the same number of phys pages as log pages..
  ret=mm_alloc_frames(nt);
  if (!ret) { 
    task_move_to_free(nt); 
    return -ENOMEM; 
  }

  /*
  //set_user_pages(nt);
  p_off = PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA;
  for (i = 0; i < NUM_PAG_DATA; i++)
  {
      set_ss_pag (p_off + i, nt->ph_frames[i]);
      
      copy_data( (void*)ts_idle->ph_frames[i], 
		 (void*)nt->ph_frames[i],
		PAGE_SIZE);
      
      del_ss_pag (p_off + i);
  }
*/

  copy_data( PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i, 
	     PAG_LOG_INIT_DATA_P0 + i,
	     NUM_PAG_DATA);


  p_off = PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA;
  for (i = 0; i < NUM_PAG_DATA; i++)
  {
      set_ss_pag (p_off + i, nt->ph_frames[i]);
      
      /*
      copy_data( (void*)ts_idle->ph_frames[i], 
		 (void*)nt->ph_frames[i],
		PAGE_SIZE);
      */

      copy_data( PAGE_SIZE*(PAG_LOG_INIT_DATA_P0 + i), 
		 PAGE_SIZE*(PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i),
		 PAGE_SIZE);

      del_ss_pag (p_off + i);
  }




  //task_dump_frames(nt);
  //task_dump_frames(ts_idle);

  nt->t_pid = task_next_pid();
  nt->t_cs = 0;
  nt->t_cpu_time = 0;
  nt->t_rq = _rq;

  //son gets 0 as a return value
  ntu=(union task_union*)nt;
  ntu->stack[KERNEL_STACK_SIZE -10] = 0;

  // flush tlb
  set_cr3();  

  //task_cmp_stacks(nt,current,17);

  // add new task to run queue
  INIT_LIST_HEAD(&(nt->t_queue));

  task_activate_task(nt);
  
  // return son pid
  return nt->t_pid;
}

int sys_sem_init(int n_sem, unsigned int value) {
  //return -ENOSYS;

  if ( (n_sem <0) || (n_sem >= SEM_MAX) || (value >= SEM_VALUE_MAX) ) return -EINVAL;
  if (__semaphores[n_sem].owner!=0) return -EBUSY;

  __semaphores[n_sem].count = value;
  __semaphores[n_sem].owner = current;
 
  return 0;
}


int sys_sem_destroy(int n_sem) {
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) return -EINVAL;
  if (!__semaphores[n_sem].owner) return -EINVAL;
  if (__semaphores[n_sem].owner != current ) return -EPERM;
  if (__semaphores[n_sem].sleepers >0) return -EBUSY;
  
  __semaphores[n_sem].owner=0;

  return 0;

}


int sys_sem_wait(int n_sem) {
  //return -ENOSYS;
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) return -EINVAL;
  if (current->t_pid == 0) return -EPERM;
  if (__semaphores[n_sem].owner==0) return -EINVAL;

  __semaphores[n_sem].count--;

  if (__semaphores[n_sem].count < 0){
    task_move_to_sem(__semaphores[n_sem].owner,n_sem);
  }
  return 0;
}


int sys_sem_signal(int n_sem) {
  //return -ENOSYS;
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) return -EINVAL;
  if (__semaphores[n_sem].owner==0) return -EINVAL;

  __semaphores[n_sem].count--;

  if (__semaphores[n_sem].count<=0){
    task_move_sem_to_active(__semaphores[n_sem].owner,n_sem);
  }

  return 0;
}



void sys_exit(){

  task_t *cur = current;
  int i;

  //printk("sys_exit\n");
  //while(1);
  
  if (!cur->t_pid) return;

  
  for(i=0;i<NUM_PAG_DATA;i++){
    free_frame(cur->ph_frames[i]);
  }
  
  for(i=0;i>SEM_MAX;i++){
    sys_sem_destroy(i);
  }
  
  cur->t_pid = -1;

  //todo force swtich to next task
  //printk("sys_exit  -> prep to switch\n");
  task_prepare_to_switch(this_rq());

  //task_move_to_free(cur);
  task_dactivate_task(cur);
  task_move_to_free(cur);

  task_switch(0);
}


int sys_get_stats(int _pid, struct stats *_st) {
  //return -ENOSYS;
  
  struct stats stats;
  if (_pid<0) return -EINVAL;
  if (_pid>pid) return -ESRCH;

  if (!_st) return -EFAULT;
  if (!access_ok(AC_WRITE,_st,sizeof(struct stats))) return -EFAULT;

  stats.tics = task[_pid].t.task.t_tics;
  stats.remaining_quantum = task[_pid].t.task.t_dprio;
  stats.cs = task[_pid].t.task.t_cs;   

  copy_to_user(&stats,_st,sizeof(struct stats));

  return 0;
 

}



