#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__



#define KBD_MAXBUFF 10

struct kbd_info{
  unsigned int scancode;
  char kcode;
  char leds;
  char key_down;
};


// Keyboard Registers
#define KBD_STATUS_REG  0x64
#define KBD_DATA_REG    0x60

// Keyboard Status Registers (we only use OBF,
// but as they all are defines, will not use any 
// aditional space on code, so declare them all
#define KBD_STATUS_OBF    1
#define KBD_STATUS_IBF    2
#define KBD_STATUS_SYS    4
#define KBD_STATUS_A2     8
#define KBD_STATUS_INH    16
#define KBD_STATUS_MOBF   32
#define KBD_STATUS_TO     64
#define KBD_STATUS_PERR   128


void init_kbd();

#endif /* __KEYBOARD_H__ */
