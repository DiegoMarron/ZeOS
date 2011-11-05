// Exeptions 

//#include <types.h>
//#include <interrupt.h>
//#include <segment.h>
//#include <hardware.h>

#include <types.h>
#include <io.h>
#include <interrupt.h>
#include <exceptions.h>
#include <mm.h>
#include <klib.h>
//#include <sys.h>


void sys_exit();


// Array for all exception error messages
char *excep_errors[]={"Division by Zero", // 0
    "Debug Error", // 1
    "Non Mascarable Interrupt", // 2
    "Breakpoint Error", // 3
    "Overflow Error", //4
    "Bound Exception", //5
    "Invalid Opcode", //6
    "FPU not available", //7
    "Doble Fault", //8
    "Coprocessor Segment Overrun", //9
    "Invalid TSS", //10
    "Segment not present", //11
    "Stack Fault Exception", //12
    "General Protection Fault", //13
    "Page fault", //14
    "Exception 15 Error", //15
    "Math Error", //16
    "Alignment Check Error", //17    
    "Machine Check", // 18
    "SIMD Floating-Point Exception" }; //19


// Creates the exception routines.
// As they only print messages, all share the same code.
// When needs cheanges, we probably will add some more macros.
//
// * NOTE: do_excep is an inline funcion! so no jump from routine to do_exep
//         is done
#define CREATE_EXCEPTION(name,num)                             \
                      void sys_excep_handler_##name();  \
                      void sys_excep_##name(){ do_excep(num); }

// Macro to get the hanlder name
#define NAME_HANDLER_EXCEPTION(name) sys_excep_handler_##name


//#define DECLARE_HANDLER_EXCEPTION(name) extern void sys_excep_handler_##name()


// we inline do_excep for speed reasons.
// each handler will call this function to print an error message and them
// halt the OS
__inline__ void do_excep(DWord n){
  printk("Ooops! Guru Meditation\n"); printk("Exceptcion occured: "); printk(excep_errors[n]);
  while(1){};
}


// Create / Declare all exception routines
CREATE_EXCEPTION(division,0)
CREATE_EXCEPTION(debug,1)
CREATE_EXCEPTION(nmi,2)
CREATE_EXCEPTION(breakpoint,3)
CREATE_EXCEPTION(overflow,4)
CREATE_EXCEPTION(bound,5)
CREATE_EXCEPTION(opcode,6)
CREATE_EXCEPTION(fpu,7)
CREATE_EXCEPTION(double_fault,8)
CREATE_EXCEPTION(copr_overrun,9)
CREATE_EXCEPTION(invalid_tss,10)
CREATE_EXCEPTION(segement_not_present,11)
CREATE_EXCEPTION(stack_fault,12)
//CREATE_EXCEPTION(gen_protection,13)
//CREATE_EXCEPTION(page_fault,14)
CREATE_EXCEPTION(excep15,15)
CREATE_EXCEPTION(math,16)
CREATE_EXCEPTION(align_check,17)
CREATE_EXCEPTION(machine,18)
CREATE_EXCEPTION(simd,19)


void sys_excep_handler_page_fault();
void sys_excep_page_fault(void){       
  int pid = current->t_pid;
  char str[12];
  __itoa(pid,&str[0],10,0);
  printk("Ooops! Guru Meditation: Exception 14 occured - Page Fault -\n");
  printk("pid = ");
  printk(&str[0]);
  printk(" | trying to access addr: ");
  pid=get_cr2();
  __itoa(pid,&str[0],10,0);
  printk(&str[0]);
  printk("  \n");
  printk("\n Page Fault Exception\n");

  //dump_regs();

  if (current->t_pid !=0 ) sys_exit();


  while(1);

}


void sys_excep_handler_gen_protection();
void sys_excep_gen_protection(void){       
  int pid = current->t_pid;
  char str[12];
  __itoa(pid,&str[0],10,0);
  printk("Ooops! Guru Meditation: Exception 14 occured - Page Fault -\n");
  printk("pid = ");
  printk(&str[0]);
  printk(" | trying to access addr: ");
  pid=get_cr2();
  __itoa(pid,&str[0],10,0);
  printk(&str[0]);
  printk("  \n");
  printk("\n General fault protection\n");
 
  //if (current->t_pid !=0 ) sys_exit();

  dump_regs();

  while(1);

}




void init_except(){
  
  setInterruptHandler (0, NAME_HANDLER_EXCEPTION(division), 0);
  setInterruptHandler (1, NAME_HANDLER_EXCEPTION(debug), 3);
  setInterruptHandler (2, NAME_HANDLER_EXCEPTION(nmi) , 0);
  setInterruptHandler (3, NAME_HANDLER_EXCEPTION(breakpoint), 3);
  setInterruptHandler (4, NAME_HANDLER_EXCEPTION(overflow), 0);
  setInterruptHandler (5, NAME_HANDLER_EXCEPTION(bound), 0);
  setInterruptHandler (6, NAME_HANDLER_EXCEPTION(opcode), 0);
  setInterruptHandler (7, NAME_HANDLER_EXCEPTION(fpu), 0);
  setInterruptHandler (8, NAME_HANDLER_EXCEPTION(double_fault), 0);
  setInterruptHandler (9, NAME_HANDLER_EXCEPTION(copr_overrun) , 0);
  setInterruptHandler (10, NAME_HANDLER_EXCEPTION(invalid_tss), 0);
  setInterruptHandler (11, NAME_HANDLER_EXCEPTION(segement_not_present), 0);
  setInterruptHandler (12, NAME_HANDLER_EXCEPTION(stack_fault), 0);
  setInterruptHandler (13, NAME_HANDLER_EXCEPTION(gen_protection), 0);
  setInterruptHandler (14, NAME_HANDLER_EXCEPTION(page_fault), 0);
  //setInterruptHandler (14, excep_page_fault, 0);
  setInterruptHandler (16, NAME_HANDLER_EXCEPTION(math), 0);
  setInterruptHandler (17, NAME_HANDLER_EXCEPTION(align_check), 0); 
  setInterruptHandler (18, NAME_HANDLER_EXCEPTION(machine), 0);
  setInterruptHandler (19, NAME_HANDLER_EXCEPTION(simd), 0);
  
}




