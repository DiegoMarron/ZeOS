/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include<list.h>
#include<mm_address.h>
//#include<semaphore.h>

#define NR_TASKS              10
#define KERNEL_STACK_SIZE     1024

//size of BM in number of shorts
#define TSK_BM_SIZE  (((NR_TASKS-1)/8)+sizeof(short))/sizeof(short);

// Task Prio
//#define MAX_RT_PRIO      100
#define MAX_RT_PRIO      100
#define MIN_PRIO         (MAX_RT_PRIO - 20)
#define MAX_PRIO         (MAX_RT_PRIO + 40)
#define DEFAULT_PRIO     (MAX_RT_PRIO + 20)


#define KERNEL_ESP       (DWord) &task[0].t.stack[KERNEL_STACK_SIZE]


struct task_struct;
struct protected_task_struct;
struct runqueue;

struct _wait_queue{
  int count;
  struct list_head queue;
};
typedef struct _wait_queue wait_queue_t;


struct runqueue{
  int nr_running;
  unsigned long context_switchs;

  //struct list_head actives;
  //struct list_head free;

  wait_queue_t actives;
  wait_queue_t frees;

  struct task_struct *idle;
  struct task_struct *running;

  struct task_struct *next;
 

};
typedef struct runqueue rq_t;
extern rq_t rq;


struct task_struct {
  struct list_head t_queue;

  int t_pid;
  int t_tics;
  int t_cpu_time;
  int t_prio;
  int t_dprio;

  unsigned int t_cs;

  rq_t *t_rq;

  int ph_frames[NUM_PAG_DATA];

};
typedef struct task_struct task_t;


union task_union {
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

struct protected_task_struct {
  unsigned long task_protect[KERNEL_STACK_SIZE];  /* This field protects the different task_structs, so any acces to this field will generate a PAGE FAULT exeption */
  union task_union t;
};
extern struct protected_task_struct task[NR_TASKS];

#define t_idle task[0]
//#define ts_idle t_idle.t.task


/////////////////////////////////////////////////////

//struct task_struct;
static inline struct task_struct * get_current(void)
{
        struct task_struct *current;
        __asm__("andl %%esp,%0; ":"=r" (current) : "0" (0xfffff000));
        return current;
 }

#define current get_current()

static inline task_t* get_idle(){
  return &task[0].t.task;
}

#define ts_idle get_idle()

extern int pid;
static inline int task_next_pid(){
  return pid++;
}

// Inicialitza les dades del proces inicial 
void init_task0(void);
void init_sched(void);



// defined as an inline function so we can
// benefit from compiler type check and avoid problems. 
static inline rq_t* this_rq(void){
  return &rq;
}


static inline task_t *list_head_to_task_struct(struct list_head *lh){
  return (task_t*)lh; 
}

static inline void rq_add_to_active(struct task_struct *_task, rq_t *_rq){
  list_add_tail(&_task->t_queue, &_rq->actives.queue);
  _rq->actives.count++;
}

static inline void rq_new_active(struct task_struct *_task, rq_t *_rq){
  list_add_tail(&_task->t_queue, &_rq->actives.queue);
  _rq->actives.count++;
}


static inline void rq_del_active(struct task_struct *_task, rq_t *_rq){
  list_del(&_task->t_queue); 
  _rq->actives.count--;
}


static inline void rq_add_to_free(struct task_struct *_task, rq_t *_rq){
  list_add_tail(&_task->t_queue, &_rq->frees.queue);
  _rq->frees.count++;
}

static inline void rq_del_free(struct task_struct *_task, rq_t *_rq){
  list_del(&_task->t_queue); 
  _rq->frees.count--;
}


static inline void task_activate_task(task_t* _task){
  rq_add_to_active(_task,this_rq());
  this_rq()->nr_running++;
}

static inline void task_activate_new(task_t* _task){
  rq_new_active(_task,this_rq());
  this_rq()->nr_running++;
}



static inline void task_dactivate_task(task_t* _task){
  rq_del_active(_task,this_rq());
  this_rq()->nr_running--;
}


static inline void task_move_to_free(task_t* _task){
  rq_add_to_free(_task,this_rq());
}


static inline int rq_current_is_idle(){
  return ( !this_rq()->nr_running && (this_rq()->actives.count) );
}

static inline union task_union* TS2TU(task_t *ts){
  return (union task_union*)ts;
}

static inline void wait_queue_init(wait_queue_t* _wq){
  INIT_LIST_HEAD(&(_wq->queue));
  _wq->count=0;
}

static inline task_t* task_get_free_slot(rq_t *_rq){
  task_t *tf;

  if(list_empty(&(_rq->frees.queue))) return 0;

  tf=list_head_to_task_struct( list_first(&(_rq->frees.queue) ));
 
  rq_del_free(tf,_rq);
  return tf;
}



void schedule();
void task_switch();
void task_prepare_to_switch(rq_t *_rq);


void dump_regs();



#endif  /* __SCHED_H__ */
