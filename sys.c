/*
 * sys.c - Syscalls implementation
 */

#include <devices.h>
#include <errno.h>
#include <utils.h>

#include <io.h>

long sys_ni_syscall(void){
        return -ENOSYS;
}


// inlined becouse it is very small (1 if) at the moment
// otherway, this should not be inlined.
static __inline__ int check_valid_fd(int fd){

  if (fd==1) return 1;
 
  return 0;
}


int sys_write(int fd,char *buffer, int size)
{
  int ret;

  if (!check_valid_fd(fd)) return -EBADF;

  if (size <0) return -EINVAL;
  if (size==0) return 0;

  if (!access_ok(0,buffer,size)) return -EFAULT;

  // uncomment this line to cheat while running runjp for del 1 
  // it will avoid print the very long chain, while emulating
  // it has successfully done it. The porpouse is to see
  // the other tests error/results.
  //if (size > 1000) return size;

  //assume only console.. :)
  ret=sys_write_console(buffer,size);

  return ret;

}


