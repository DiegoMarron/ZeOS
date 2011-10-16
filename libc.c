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
  do_syscall_3(__NR_write,fd,buffer,size);
}

int getpid(){
  do_syscall_0(__NR_getpid);
}

int fork() {
  do_syscall_0(__NR_fork);
}

void exit() {
  //do_syscall_0(__NR_exit);
  __asm__ volatile ("int $0x80"          \
		  :                      \
		  : "a" (__NR_exit));
}

int nice(int quantum) {
  do_syscall_1(__NR_nice,quantum);
}

int sem_init(int n_sem, unsigned int value){
  return -38;
}

int sem_wait(int n_sem){
  return -38;
}

int sem_signal(int n_sem){
  return -38;
}

int sem_destroy(int n_sem) {
  return -38;
}

int get_stats(int pid, struct stats *st){
  return -38;
}
