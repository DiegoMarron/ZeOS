/*
 * libc.c 
 */

#include <libc.h>
#include <unistd.h>

#include <stdio.h>

int errno;

/* Wrapper of  write system call*/

int write(int fd,char *buffer,int size)
{

  //OB	return 0;
  do_syscall_3(__NR_write,fd,buffer,size);



}

/*
int write2(int fd,char *buffer,int size)
{
  
  do_syscall_32(4,fd,buffer,size);
 
  if (__res < 0) printf("s:menor q zero");
  if (__res == 0) printf("s:igual q zero");
  if (__res > 0) printf("s:mayor q zero");
  
  return __res;

}

*/
