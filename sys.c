/*
 * sys.c - Syscalls implementation
 */

#include <devices.h>
#include <errno.h>



// inlined becouse it is very small (1 if) at the moment
// otherway, this should not be inlined.
static __inline__ int check_valid_fd(int fd){
  if (fd==1) return 1;
 
  return 0;
}


int sys_write(int fd,char *buffer, int size)
{
  int ret;

  if (!check_valid_fd) return -EBADF;

  if ( !buffer || (size <=0) ) return -EINVAL;
 
  //assume only console.. :)
  ret=sys_write_console(buffer,size);

  return ret;

}


