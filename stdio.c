

#include <libc.h>
#include <stdio.h>



// printf :)
/*
int printf(const char *format, ...){


  return 0;
}
*/

int printf(char *string){

  
  int size=0;
  int ret;
  
  while (string[size]!='\0') { size++;}
  
  ret=write(STDOUT_FILENO,(char*)string,size);
  
  return ret;

}

