//#include <libc.h>
#include <stdio.h>
#include <testsuit.h>

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
    /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
    /* Uncomment next line to call the initial routine for the test cases */
    /* runjp(); */

  /*    
  char c1='h';
  char c2='o';
  write(1,&c1,1);
  write(1,&c2,1);
  */

  //printf("Hello from User mode prinft\n\0");

  test_suit1();
  

  while(1);


}
