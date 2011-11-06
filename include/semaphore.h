
#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H_


#include <list.h>
#include <sched.h>

#define SEM_VALUE_MAX 20
#define SEM_MAX 10

struct task_struct;

struct semaphore{
  struct task_struct *owner;
  int init;
  int count;
  int sleepers;
  struct list_head wait;
};

typedef struct semaphore sem_t;


extern sem_t __semaphores[SEM_MAX];



void init_sem();

static inline void task_move_to_sem(task_t* _task,int n_sem){
  //list_del(&_task->t_queue);
  task_dactivate_task(_task); 
  list_add_tail(&_task->t_queue, &__semaphores[n_sem].wait);
  __semaphores[n_sem].sleepers++;
}

static inline void task_move_sem_to_active(task_t* _task,int n_sem){
  list_del(&_task->t_queue);
  task_activate_task(_task);
  __semaphores[n_sem].sleepers--;
}
    


#endif
