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

