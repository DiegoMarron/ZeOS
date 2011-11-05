#include <utils.h>
#include <types.h>
#include <mm.h>
#include <mm_address.h>


void copy_data(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
}
/* Copia de espacio de usuario a espacio de kernel, devuelve 0 si ok y -1 si error*/
int copy_from_user(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
  return 0;
}
/* Copia de espacio de kernel a espacio de usuario, devuelve 0 si ok y -1 si error*/
int copy_to_user(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
  return 0;
}

/* access_ok: Checks if a user space pointer is valid
 * @type:  Type of access: %VERIFY_READ or %VERIFY_WRITE. Note that
 *         %VERIFY_WRITE is a superset of %VERIFY_READ: if it is safe
 *         to write to a block, it is always safe to read from it
 * @addr:  User space pointer to start of block to check
 * @size:  Size of block to check
 * Returns true (nonzero) if the memory block may be valid,
 *         false (zero) if it is definitely invalid
 */
int access_ok(int type, const void * addr, unsigned long size)
{
/*
  unsigned long addr_top = (unsigned long)addr + size;  

  if ( ((unsigned long)addr < L_USER_START) || ( addr_top > USER_ESP) )
    return 0;

  return 1;
*/

  unsigned long addr_top,p_start,p_end,pages;
  unsigned int i;
	
  if ((type != AC_READ) && (type != AC_WRITE) ) return 0;
  if ( (!addr) || (size==0)) return 0;

  addr_top = (unsigned long)addr + size;  
  
  // validate addr range
  if ( ((unsigned long)addr < L_USER_START) || ( addr_top > USER_ESP) ) return 0;

  p_end=ADDR2PAGE((unsigned long)addr_top);
  p_start=ADDR2PAGE((unsigned long)addr);
		  
  pages = p_end - p_start;
  pages++;

  // validate type of access
  for(i=0;i<pages;i++){
    if (pagusr_table[p_start+i].bits.rw < type) return 0;
  }
 
  return 1; 
	
}


