
#include <types.h>
#include <io.h>
#include <keyboard.h>
#include <interrupt.h>


extern void kbd_handler();


struct kbd_info kbd_i;
char kbd_buff[KBD_MAXBUFF];

extern char char_map[];

static __inline__ Byte kbd_data_read(){
  return inb(KBD_DATA_REG);
}

static __inline__ Byte kbd_status_read(){
  return inb(KBD_STATUS_REG);
}


/* Keyboard Routine

   Status (reg 0x64)flags on a PS/2 Compatible:
   MSb                                       LSb
   PERR | TO | MOBF | INH | A2 | SYS | IBF | OBF 

   OBF indicates if we can write data (1 buffer is full,
   0 buffer is empty), if buffer is full we can get the scancode
 */
void kbd_routine(){

  // If buffer is empty, just return. 
  if ( (kbd_status_read() & KBD_STATUS_OBF) == 0 ) return;

  kbd_i.scancode = kbd_data_read();
  kbd_i.kcode=kbd_i.scancode & 0x7f;

  if ( (kbd_i.scancode & 0x80) > 0){
    //key up
    // for the moment we are not interested on key release
  }else{
    //key down
    //kbd_buff[0]=char_map[2];
    kbd_buff[0]=char_map[kbd_i.kcode];

    printk(&kbd_buff[0]);
  }


}



void init_kbd(){
  setInterruptHandler (33, kbd_handler, 0);  // int 0x21
}
