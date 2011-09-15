// Exeptions 

//#include <types.h>
//#include <interrupt.h>
//#include <segment.h>
//#include <hardware.h>

#include <types.h>
#include <io.h>
#include <interrupt.h>
#include <exceptions.h>


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
#define CREATE_EXCEPTION(name,num)  void sys_excep_##name(){ do_excep(num); }


// Macro to get the hanlder name
#define NAME_HANDLER_EXCEPTION(name) void sys_excep_handler_##name()


// we inline do_excep for speed reasons.
// each handler will call this function to print an error message and them
// halt the OS
inline void do_excep(DWord n){
  printk("Ooops! Guru Meditation\n"); printk("Exceptcion occured: "); printk(&excep_errors[n]);
  while(1){};
}


// Create / Declare all exception routines
CREATE_EXCEPTION(division,0);
CREATE_EXCEPTION(debug,1);
CREATE_EXCEPTION(nmi,2);
CREATE_EXCEPTION(breakpoint,3);
CREATE_EXCEPTION(overflow,4);
CREATE_EXCEPTION(bound,5);
CREATE_EXCEPTION(opcode,6);
CREATE_EXCEPTION(fpu,7);
CREATE_EXCEPTION(double_fault,8);
CREATE_EXCEPTION(copr_overrun,9);
CREATE_EXCEPTION(invalid_tss,10);
CREATE_EXCEPTION(segement_not_present,11);
CREATE_EXCEPTION(stack_fault,12);
CREATE_EXCEPTION(gen_protection,13);
CREATE_EXCEPTION(page_fault,14);
CREATE_EXCEPTION(excep15,15);
CREATE_EXCEPTION(math,16);
CREATE_EXCEPTION(align_check,17);
CREATE_EXCEPTION(machine,18);
CREATE_EXCEPTION(simd,19);






