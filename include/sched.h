/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include<list.h>

#define NR_TASKS              10
#define KERNEL_STACK_SIZE     1024

//size of BM in number of shorts
#define TSK_BM_SIZE  (((NR_TASKS-1)/8)+sizeof(short))/sizeof(short);

// Task Prio
#define MAX_RT_PRIO      100
#define MAX_PRIO         (MAX_RT_PRIO + 40)
#define DEFAULT_PRIO     (MAX_RT_PRIO + 20)


#define KERNEL_ESP       (DWord) &task[0].t.stack[KERNEL_STACK_SIZE]


struct task_struct;
struct protected_task_struct;
struct runqueue;


struct runqueue{
  int nr_running;

  struct list_head actives;
  struct list_head expired;

  struct task_struct* idle;
  struct task_struct* running;
};
typedef struct runqueue rq_t;
extern rq_t* runqueue;


struct task_struct {
  struct list_head t_queue;

  int t_pid;
  int t_tics;
 
  int t_prio;

  rq_t *t_rq;
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

/////////////////////////////////////////////////////

//struct task_struct;
static inline struct task_struct * get_current(void)
{
        struct task_struct *current;
        __asm__("andl %%esp,%0; ":"=r" (current) : "0" (0xfffff000));
        return current;
 }

#define current get_current()



// Inicialitza les dades del proces inicial 
void init_task0(void);
void init_sched(void);



#endif  /* __SCHED_H__ */
