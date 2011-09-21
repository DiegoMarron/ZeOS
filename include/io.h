/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

#define NUM_COLUMNS 80
#define NUM_ROWS    25



/** Screen functions **/
/**********************/

// PORT I/O
// byte zise IO
Byte inb (unsigned short port);
void outb(Byte __val, unsigned short __port);


// prints
void printc(char c);
void printk(char *string);
void printk_xy(int _x, int _y, char *string);

#endif  /* __IO_H__ */
