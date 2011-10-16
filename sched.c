/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <mm.h>

struct protected_task_struct task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#define t_idle task[0]
#define ts_idle t_idle.t.task

rq_t rq;
int pid=1;


/*
static inline task_t* ts_task(int i){
  return *task[i].t.task;
}
*/



void init_task0(void)
{

  int i;
  rq_t *rq = this_rq();

  // attach idle proc to it's run queue
  rq->idle = &ts_idle;
  ts_idle.t_rq = rq;
  ts_idle.t_pid = 0;
  ts_idle.t_cpu_time = 0;
  ts_idle.t_tics = 0;

  // Mark code used ph_pages 
  for (i = NUM_PAG_CODE; i < (NUM_PAG_CODE+NUM_PAG_KERNEL); i++){
    phys_mem[i] = USED_FRAME;
  }

  //ph_start -> NUM_PAG_KERNEL
  
  // Initializes paging for the process 0 adress space 
  //set_user_pages(&task[0].t.task);
  set_user_pages(&ts_idle);
  set_cr3();

  rq_add_to_active(&ts_idle,this_rq());

}


void init_tasks(void){
  
  int i;
  for(i=0;i<NR_TASKS;i++){
    task[i].t.task.t_pid = -1;
    task[i].t.task.t_prio = DEFAULT_PRIO;
    task[i].t.task.t_tics = 0;
    task[i].t.task.t_cpu_time = 0;
    task[i].t.task.t_tics = 0;
 
    rq_add_to_free(&task[i].t.task,this_rq());
  }

}


void init_sched(void){

  //int i;

  INIT_LIST_HEAD(&rq.actives);
  INIT_LIST_HEAD(&rq.free);

  rq.nr_running=0;
  rq.idle = rq.running = 0;

  init_tasks();

  init_task0();

}

/*
inline task_t *list_head_to_task_struct(struct list_head *lh){
  return (task_t*)lh; 
}
*/

// TODO
task_t* task_get_free_slot(){
  return list_head_to_task_struct(list_first( &(this_rq()->free) ));
}

void task_free_slot(task_t *ts){
  list_add_tail(&(ts->t_queue), &(this_rq()->free) );
}

