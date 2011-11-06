/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <exceptions.h>

#include <klib.h>
#include <io.h>
#include <keyboard.h>
#include <sched.h>

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


void clock_handler();
void syscall_handler();
void kbd_handler();

void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;

  // in exceptions.c
  init_except();

  // Ints
  setInterruptHandler (32, clock_handler, 0);  // int 0x20
  //setInterruptHandler (33, kbd_handler, 0);  // int 0x21

  init_kbd();

  //syscals
  setTrapHandler (128, syscall_handler, 3);  // int 0x80
  

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  set_idt_reg(&idtR);

}


/*
  HH:MM:SS <- 8 characters
 */

// 64 bits
unsigned int tics=0;
int up_secs=0, up_mins=0, up_hours=0;

char time_map[60][2]={ "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10",
                    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
                    "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
                    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
		    "51", "52", "53", "54", "55", "56", "57", "58", "59"   };


void clock_routine(){

  char _time_buff[9]={'0','0',':','0','0',':','0','0','\0'};
  int freq = 200;

  // just increment tic, if it reaches the max 2^64
  // when incremented by 1, automatically it will reset to 0
  tics++;
  current->t_tics++;
  
  if ((tics % freq) == 0){
    up_secs++;
    if (up_secs > 59) { up_secs=0;up_mins++; }
    if (up_mins > 59) { up_mins=0;up_hours++; }
    if (up_hours > 60) { up_hours=0;tics=0; }
  }

  _time_buff[6]=time_map[up_secs][0];
  _time_buff[7]=time_map[up_secs][1];
    
  _time_buff[3]=time_map[up_mins][0];
  _time_buff[4]=time_map[up_mins][1];

  
  _time_buff[0]=time_map[up_hours][0];
  _time_buff[1]=time_map[up_hours][1];

  //printk_xy(72,0,&_time_buff[0]);
  
 
  schedule();
  
}



