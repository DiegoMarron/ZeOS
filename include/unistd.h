

#ifndef __UNISTD_H__
#define __UNISTD_H__

extern int errno;

#define __NR_exit         1
#define __NR_fork         2
#define __NR_write        4
#define __NR_getpid       20
#define __NR_sem_init     21
#define __NR_sem_wait     22
#define __NR_sem_signal   23
#define __NR_sem_destroy  24
#define __NR_nice         34
#define __NR_get_stats    35



// do syscall return: puts the error code on errno
// and return -1 if an error ocurred

#define _syscall_return(res)                            \
  if ((unsigned long)(res) >= (unsigned long)(-125)){    \
    errno = -(res);res =-1;                             \
  }                                                     \
  return res


// Do system call which need 3 parameters
#define do_syscall_3(num,arg1,arg2,arg3)                     \
  long __res;				                     \
  __asm__ volatile ("int $0x80"                              \
		  : "=a" (__res)                             \
		  : "0" (num),"b" ((long)(arg1)),	     \
                    "c" ((long)(arg2)), "d" ((long)(arg3))); \
  _syscall_return(__res)


#define do_syscall_0(num)				     \
  long __res;				                     \
  __asm__ volatile ("int $0x80"                              \
		  : "=a" (__res)                             \
		  : "0" (num));  			     \
  _syscall_return(__res)



#define do_syscall_1(num,arg1)				     \
  long __res;				                     \
  __asm__ volatile ("int $0x80"                              \
		  : "=a" (__res)                             \
		  : "0" (num),"b" ((long)(arg1)));	     \
  _syscall_return(__res)









#endif

