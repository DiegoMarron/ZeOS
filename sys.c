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
  if (!nt) {
    //printk("sys_fork: nt == NULL *****************\n");
    return -ENOMEM;
  }
  copy_data(cp, nt, KERNEL_STACK_SIZE*sizeof(int));

  // TODO: check if there's free pagusr space <- maybe redundant?
  // as we have the same number of phys pages as log pages..
  ret=mm_alloc_frames(nt);
  if (!ret) { 
    task_move_to_free(nt); 
    return -ENOMEM; 
  }


  p_off = PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA;
  for (i = 0; i < NUM_PAG_DATA; i++)
  {
      set_ss_pag (p_off + i, nt->ph_frames[i]);
      
      copy_data(PAGE_SIZE*(PAG_LOG_INIT_DATA_P0 + i), 
		PAGE_SIZE*(PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i),
		 PAGE_SIZE);

      del_ss_pag (p_off + i);
  }


  nt->t_pid = task_next_pid();
  nt->t_cs = 0;
  nt->t_cpu_time = 0;
  nt->t_rq = _rq;
  nt->t_dprio = nt->t_prio;
  nt->t_tics = 0;

  //son gets 0 as a return value
  ntu=(union task_union*)nt;
  ntu->stack[KERNEL_STACK_SIZE -10] = 0;

  // flush tlb
  set_cr3();  

  //task_cmp_stacks(nt,current,17);

  /*
#ifdef __ZEOS_DEBUG_
  printk("sys_fork: new pid -> ");
  printd(nt->t_pid);
#endif
  */
  // add new task to run queue
  INIT_LIST_HEAD(&(nt->t_queue));

  task_activate_task(nt);
  /*
#ifdef __ZEOS_DEBUG_
  printk("|  nt addr -> ");
  printd((int)nt);
#endif
  */

  // return son pid
  return nt->t_pid;
}

int sys_sem_init(int n_sem, unsigned int value) {
  //return -ENOSYS;

  /*
#ifdef __ZEOS_DEBUG_
  printk("sys_sem_init: n_sem = ");
  printd(n_sem);
#endif
  */

  if ( (n_sem <0) || (n_sem >= SEM_MAX)) return -EINVAL; 
  if ( (value <0) || (value >= SEM_VALUE_MAX) ) return -EINVAL;
  //  if (__semaphores[n_sem].owner!=0) return -EBUSY;
  if (__semaphores[n_sem].init==1) return -EBUSY;

  /*
#ifdef __ZEOS_DEBUG_
  printk(" | current->pid = ");
  printd(current->t_pid);
  if (current->t_pid == -1) printk("BUG!!! pid -1");
  if (current->t_pid < 0) printk("BUG!!! pid < 0");
#ifdef __ZEOS_DEBUG_
  */

  __semaphores[n_sem].count = value;
  __semaphores[n_sem].owner = current;
  __semaphores[n_sem].init = 1;

  /*
#ifdef __ZEOS_DEBUG_
  printk(" | sem owner = ");
  printd(__semaphores[n_sem].owner);
  printk("****\n");
#endif
  */
  return 0;
}


int sys_sem_destroy(int n_sem) {
  
  union task_union *tu;
  
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) {
    //printk("sys_sem_destroy: n_sem out of range ret EINVAL****\n");
    return -EINVAL;
  }
  /*
  if (__semaphores[n_sem].owner== NULL) {
    printk("sys_sem_destroy: owned == 0 ret EINVAL****\n");
    return -EINVAL;
    }*/
  if (__semaphores[n_sem].init != 1) {
    //printk("sys_sem_destroy: init not 1 ret EINVAL****\n");
    return -EINVAL;
    
  }  

  if (__semaphores[n_sem].owner != current ) {
    //    printk("sys_sem_destroy: owner != current  EPERM****\n");
    return -EPERM;
  }

  //if (__semaphores[n_sem].sleepers > 0)  return -EBUSY; 


  //printk("sys_sem_destroy: enter!!!****\n");
  //if (__semaphores[n_sem].sleepers >0) return -EBUSY;
  /*
#ifdef __ZEOS_DEBUG_
  printk("sys_sem_destroy:  owner = ");
  printd((int) __semaphores[n_sem].owner);
  printk(" | n_sem  = ");
  printd(n_sem);
  printk(" | sleepers = ");
  printd((int) __semaphores[n_sem].sleepers);
  printk("\n++++++++++++");
#ifdef __ZEOS_DEBUG_
  */
  __semaphores[n_sem].owner=0;
  //__semaphores[n_sem].count=0;
  //__semaphores[n_sem].sleepers=0;
  __semaphores[n_sem].init=-1;  

   while (__semaphores[n_sem].sleepers > 0) {
  //while(!list_empty(&__semaphores[n_sem].wait)){
     /*
#ifdef __ZEOS_DEBUG_
    printk(" | sys_sem_destroy: WHILE:sleepers ");
    printd(__semaphores[n_sem].sleepers);
#endif 
     */   
    tu = (union task_union*) list_first(&__semaphores[n_sem].wait);
    
    //printk("sys_sem_destroy: activating task = ");
    //printd(&tu->task);
    //printk("\n");
    
    tu->stack[KERNEL_STACK_SIZE-10] = -1;
    task_move_sem_to_active(&tu->task,n_sem);
    /*
#ifdef __ZEOS_DEBUG_
    printk("moved task pid = ");
    printd(tu->task.t_pid);
#endif
    */
  }

  
  INIT_LIST_HEAD(&(__semaphores[n_sem].wait));

  /*
#ifdef __ZEOS_DEBUG_
  printk("****\n");
#endif
  */
  return 0;
}


int sys_sem_wait(int n_sem) {
  //return -ENOSYS;

  /*
#ifdef __ZEOS_DEBUG_
  printk("sys_sem_wait current->pid= ");
  printd(current->t_pid);
#endif
  */

  if (current->t_pid == 0) {
    printk("****\n");

    return -EPERM;
  }
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) return -EINVAL;
  if (__semaphores[n_sem].init!=1) return -EINVAL;
  /*
#ifdef __ZEOS_DEBUG_
  printk(" | n_sem  = ");
  printd(n_sem);
  printk(" | __semaphores[n_sem].init = ");
  printd(__semaphores[n_sem].init);
  printk(" |  owner = ");
  printd((int) __semaphores[n_sem].owner);
#end
  */
//printk("");


  //if (__semaphores[n_sem].owner==0) return -EINVAL;

  __semaphores[n_sem].count--;

  if (__semaphores[n_sem].count < 0){
    ((union task_union*)current)->stack[KERNEL_STACK_SIZE-10] = 0;
    task_prepare_to_switch(this_rq());
    //task_move_to_sem(__semaphores[n_sem].owner,n_sem);
    task_move_to_sem(current,n_sem);
    /*
#ifdef __ZEOS_DEBUG_
    printk(" task pid = ");
    printd(__semaphores[n_sem].owner->t_pid);
    printk(" blocked...");
    printk("SWITCH!!!****\n");
#endif
    */
    task_switch(0);
  }

  printk("****\n");
  return 0;
}


int sys_sem_signal(int n_sem) {

  printk("sys_sem_signal current->pid= ");
  printd(current->t_pid);
  //return -ENOSYS;
  if ( (n_sem <0) || (n_sem >= SEM_MAX)  ) return -EINVAL;
  if (__semaphores[n_sem].init!=1){
    printk("sys_sem_signal: init 1\n");
    return -EINVAL;
  }
  //if (__semaphores[n_sem].owner==0) return -EINVAL;

  /*
#ifdef __ZEOS_DEBUG_
  printk(" | n_sem  = ");
  printd(n_sem);
  printk(" | __semaphores[n_sem].init = ");
  printd(__semaphores[n_sem].init);
  printk(" |  owner = ");
  printd((int) __semaphores[n_sem].owner);
#endif
  */
  __semaphores[n_sem].count++;
  
  if (__semaphores[n_sem].count<=0){
    //    task_move_sem_to_active(__semaphores[n_sem].owner,n_sem);
    task_move_sem_to_active((task_t*)list_first(&__semaphores[n_sem].wait) ,n_sem);
    /*
#ifdef __ZEOS_DEBUG_
    printk("task pid = ");
    printd(__semaphores[n_sem].owner->t_pid);
    printk("back to active...");
#endif
    */
  }

  /*
  if (__semaphores[n_sem].sleepers>0){
    task_move_sem_to_active(__semaphores[n_sem].owner,n_sem);
    printk("task pid = ");
    printd(__semaphores[n_sem].owner->t_pid);
    printk("back to active...");
    }*/


  return 0;
}



void sys_exit(){

  task_t *cur = current;
  int i;

  /*
#ifdef __ZEOS_DEBUG_
  printk("sys_exit: current->pid ");
  printd(current->t_pid);
#endif
  */

  if (cur->t_pid==0) return;
  //if (!cur->t_pid) return;

  
  for(i=0;i<NUM_PAG_DATA;i++){
    free_frame(cur->ph_frames[i]);
  }

 
  for(i=0;i<SEM_MAX;i++){
    sys_sem_destroy(i);
  }

  /*
  printk("!!!!!!!!!!!!!!!!!sys_exit cur->pid = ");
  printd(cur->t_pid);
  */
  
  cur->t_pid = -10;
  cur->t_prio = cur->t_dprio = 0;
  cur->t_tics = cur->t_cpu_time =0;

  //task_move_to_free(cur);

  //todo force swtich to next task
  printk("-> prep to switch\n");
  task_prepare_to_switch(this_rq());

  task_dactivate_task(cur);
  task_move_to_free(cur);


  task_switch(0);
}


int sys_get_stats(int _pid, struct stats *_st) {
  //return -ENOSYS;
  int i;
  task_t *ts=NULL;

  struct stats stats;
  if (_pid<0) return -EINVAL;
  if (_pid>pid) return -ESRCH;

  if (_st==NULL) return -EFAULT;
  if (!access_ok(AC_WRITE,_st,sizeof(struct stats))) return -EFAULT;

  /*
  stats.tics = task[_pid].t.task.t_tics;
  stats.remaining_quantum = task[_pid].t.task.t_dprio;
  stats.cs = task[_pid].t.task.t_cs;   
  */


  for(i=0;i<NR_TASKS;i++){
    if( task[i].t.task.t_pid == _pid) ts= &task[i].t.task;
  }
  if(!ts) return -ESRCH;
    
  stats.tics = ts->t_tics;
  stats.remaining_quantum = ts->t_dprio;
  stats.cs = ts->t_cs;   


  copy_to_user(&stats,_st,sizeof(struct stats));

  return 0;

}



