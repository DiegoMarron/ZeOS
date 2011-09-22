

#ifndef __UNISTD_H__
#define __UNISTD_H__

extern int errno;

#define __NR_write  4


// do syscall return: puts the error code on errno
// and return -1 if an error ocurred
/*
#define _syscall_return(res)                            \
  if ((unsigned long)(res) >= (unsigned long)(-125)){    \
    errno = -(res);res =-1;                             \
  }                                                     \
  return res
*/

#define _syscall_return(res)                            \
  if (res < 0){    \
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

/*
#define do_syscall_32(num,arg1,arg2,arg3)                     \
  long __res;				                     \
  __asm__ volatile ("int $0x80"                              \
		  : "=a" (__res)                             \
		  : "0" (num),"b" ((long)(arg1)),	     \
                    "c" ((long)(arg2)), "d" ((long)(arg3))); \
*/


#endif

