/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>

Gate idt[IDT_ENTRIES];
Register    idtR;

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','¡','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','ñ',
  '\0','º','\0','ç','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}
void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;
  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  set_idt_reg(&idtR);

  /*
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
  setInterruptHandler (16, NAME_HANDLER_EXCEPTION(math), 0);
  setInterruptHandler (17, NAME_HANDLER_EXCEPTION(align_check), 0); 
  setInterruptHandler (18, NAME_HANDLER_EXCEPTION(machine), 0);
  setInterruptHandler (19, NAME_HANDLER_EXCEPTION(simd), 0);
  */

  // Ints
  //setInterruptHandler (32, clock_handler, 0);  // int 0x20
  //setInterruptHandler (33, kbd_handler, 0);  // int 0x21

  //syscals
  //setInterruptHandler (128, syscall_handler, 0);  // int 0x80


}

