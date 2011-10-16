

#include <types.h>



char i_map[] = {'0','1','2','3','4','5','6','7','8','9',
                'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z'};

char * __itoa(int __val, char *__s, int __radix,int upper_case){

  int sign;
  char *stmp = __s;
  char x,*os=__s;

  // TODO: it returns upper case allways. lower case is a TODO
  // TODO2: should set errno variable!! 
  if (__radix > 36 || __radix <= 1)
     return 0;

  //check negative number
  if ( (sign=(__radix==10 && __val <0))) 
    __val=-__val;
 
  if ( __val==0){
    *stmp++='0';
    *stmp='\0';
    return __s;

  }

  while( __val > 0 ){
    *stmp = i_map[__val % __radix];
    stmp++;
    __val /= __radix;
  }

  if (sign) *stmp++ ='-';
  
  *stmp='\0';
  
  stmp--;
  while (stmp>=os)
  {
    x=*stmp;
    *stmp--=*os;
    *os++=x;
  }

  return __s;
}

