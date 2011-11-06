/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <mm.h>
#include <klib.h>
#include <io.h>
#include <semaphore.h>

sem_t __semaphores[SEM_MAX];
struct protected_task_struct task[NR_TASKS]  __attribute__((__section__(".data.task")));


rq_t rq;
int pid=1;


#define BOCHSHALT __asm__ __volatile__ (      \
   "pushl %ecx\n\t"            \
   "xor %ecx,%ecx\n"         \
   "0:\n\t"               \
   "jecxz 0b\n\t"            \
   "popl %ecx\n\t"            \
)


void init_sem(){
  int i;
  for(i=0;i<SEM_MAX;i++){
    __semaphores[i].owner =0;
    __semaphores[i].count=0;
    __semaphores[i].sleepers=0;
    __semaphores[i].init=-1;
    INIT_LIST_HEAD(&__semaphores[i].wait);
  }
}


void task_dump_frames(task_t *ts){
  int i;
  char str[12];

  for (i = 0; i < NUM_PAG_DATA; i++)
  {
    __itoa((int)ts->ph_frames[i],&str[0],10,0);
    printk("dump_frames: frame ->");
    printk(&str[0]);
    printk("\n");
    
  }
}

void task_cmp_stacks(task_t *_nt, task_t *_cur,int n){
  char str[12];
   int k1;
    int k2;
    int pd;
  
   for (pd=n;pd>0;pd--){
      k1=((union task_union*)_nt)->stack[KERNEL_STACK_SIZE - pd];
      k2=((union task_union*)_cur)->stack[KERNEL_STACK_SIZE - pd];

      if (k2 != k1){
	__itoa((int)pd,&str[0],10,0);
	printk("sys_fork: stack comp [");printk(&str[0]);
	__itoa((int)k1,&str[0],10,0);
	printk("] nt -> ");printk(&str[0]);
	__itoa((int)k2,&str[0],10,0);
	printk(" | current -> ");printk(&str[0]);

	printk("\n");
      }
    }
  }



void task_dump_tasks(int _top){
  int i;
  char str[12];

  if (_top > NR_TASKS) _top =NR_TASKS;

  for (i = 0; i < _top ; i++)
  {

    __itoa((int)&task[i],&str[0],10,0);
    printk("dump_frames: protec task addr -> ");
    printk(&str[0]);
    __itoa((int)&task[i].t.task,&str[0],10,0);
    printk(" |  task_struct addr ->");
    printk(&str[0]);
    printk("\n");
    
  }
}


void dump_regs()
{
   char str[12];
   unsigned int reg;
   
    
   asm volatile("movl %%eax,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg EAX: "); printk(&str[0]); printk("         ++\n");

   asm volatile("movl %%ebx,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg EBX: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%ecx,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg ECX: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%edx,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg EDX: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%edi,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg EDI: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%esi,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg ESI: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%ebp,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg EBP: "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%ds,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg DS:  "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%es,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg ES:  "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%fs,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg FS:  "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%gs,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg GS:  "); printk(&str[0]); printk("          ++\n");

   asm volatile("movl %%esp,%0" :"=r"(reg));
   __itoa(reg, &str[0],10,0);
   printk("* reg ESP: "); printk(&str[0]); printk("  ++\n");



}


void init_task0(void)
{

  int i;
  rq_t *rq = this_rq();

  rq->idle = ts_idle;
  ts_idle->t_rq = rq;
  ts_idle->t_pid = 0;
  ts_idle->t_cpu_time = 0;
  ts_idle->t_tics = 0;
  ts_idle->t_cs = 0;
  ts_idle->t_prio = ts_idle->t_dprio = MIN_PRIO;

  INIT_LIST_HEAD(&(ts_idle->t_queue));

  /*
  // Mark code used ph_pages 
  for (i = NUM_PAG_KERNEL; i < (NUM_PAG_CODE+NUM_PAG_DATA); i++){
    phys_mem[i] = USED_FRAME;
    }*/

  // Initializes paging for the process 0 adress space 
  //set_user_pages(&task[0].t.task);
  set_user_pages(ts_idle);
  set_cr3();

  rq->running = ts_idle;

  printk("task0 addr = ");
  printd((int)ts_idle);

  printk("| rq running queue = ");
  printd((int)&(this_rq()->actives.queue));

  printk("| rq  = ");
  printd((int)(this_rq()));


  rq_add_to_active(ts_idle,this_rq());
}


void init_tasks(void){

  int i;
  for(i=1;i<NR_TASKS;i++){
    task[i].t.task.t_pid = -1;
    task[i].t.task.t_prio = DEFAULT_PRIO;
    task[i].t.task.t_dprio = task[i].t.task.t_prio;
    task[i].t.task.t_tics = 0;
    task[i].t.task.t_cpu_time = 0;
    task[i].t.task.t_tics = 0;
    task[i].t.task.t_cs = 0; 
    task[i].t.task.t_rq = this_rq(); 
    
    INIT_LIST_HEAD(&task[i].t.task.t_queue);
    rq_add_to_free(&task[i].t.task,this_rq());
  }
  
}


void init_sched(void){

  wait_queue_init(&rq.actives);
  wait_queue_init(&rq.frees);
  
  rq.nr_running=0;
  rq.context_switchs=0;
  rq.idle = ts_idle; 
  rq.running = ts_idle;
  rq.next = ts_idle;

  init_tasks();
  init_task0();
  init_sem();

}



// rq->next should contain next task to switch.
void task_switch(int eoi){

  union task_union *tu;
  int i;
  unsigned long esp;
  unsigned long p_off;
  
  rq_t *_rq=this_rq();
    //current->t_rq;

  if (_rq->next==0) return;

  /*
#ifdef __ZEOS_DEBUG_
  printk("task_switch from pid = ");
  printd(current->t_pid);
  printk(" to new pid = ");
  printd(_rq->next->t_pid);
#endif  
  */
  _rq->context_switchs++;
  current->t_cs++;
  tu=TS2TU(_rq->next);

  //kernel stack
  tss.esp0=(DWord)&(tu->stack[KERNEL_STACK_SIZE]);

  p_off=PAG_LOG_INIT_DATA_P0;
  for(i=0; i<NUM_PAG_DATA;i++,p_off++){
    set_ss_pag(p_off,_rq->next->ph_frames[i]);
  }

  //pages changed...so flushing tlb!
  set_cr3();
  
  _rq->running=_rq->next;

  esp = (unsigned long)&(tu->stack[KERNEL_STACK_SIZE - 16]);
  __asm__ __volatile__("movl %0, %%esp": :"g" (esp));

  if(eoi == 1){
    __asm__ __volatile__("movb $0x20, %al \n"
			 "outb %al, $0x20");
  }

  __asm__ __volatile__("popl %ebx \n"
		       "popl %ecx \n"
		       "popl %edx \n"
		       "popl %esi \n"
		       "popl %edi \n"
		       "popl %ebp \n"
		       "popl %eax \n"
		       "popl %ds \n"
		       "popl %es \n"
		       "popl %fs \n"
		       "popl %gs \n"
		       "iret \n");
}


// finds next task to switch and put it into rq->next field.
// we need to take into account the special case where we 
// have only swap(idle) proc running. In this case, next will
// be null and swap(idle) proc dprio will be regenerated. 
// task_switch should take that into account.
//
void task_prepare_to_switch(rq_t *_rq){
 
  current->t_dprio = current->t_prio;
  
  if (_rq->actives.count > 1){
    list_del(&current->t_queue);
    list_add_tail(&current->t_queue,&_rq->actives.queue);
    _rq->next=list_head_to_task_struct(list_first(&_rq->actives.queue));
  }
}


// we are dividing schedule into 2 parts:
// - choose next task to switch
// - and the switch to new task
void schedule(){

  task_t *ts=current;

  ts->t_dprio--;
  ts->t_cpu_time++;

  if(!ts->t_dprio){
    task_prepare_to_switch(this_rq());
    task_switch(1);
  }
 
}



