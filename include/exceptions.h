
#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

//#include

// Exception names table
extern char *excep_errors[];


NAME_EXCEPTION(division);                                      
NAME_EXCEPTION(debug);                                        
NAME_EXCEPTION(nmi);                                             
NAME_EXCEPTION(breakpoint);                                   
NAME_EXCEPTION(overflow);                                           
NAME_EXCEPTION(bound);                                                
NAME_EXCEPTION(opcode);                                               
NAME_EXCEPTION(fpu);                                                        
NAME_EXCEPTION(double_fault);                                              
NAME_EXCEPTION(copr_overrun);                                             
NAME_EXCEPTION(invalid_tss);                                                  
NAME_EXCEPTION(segement_not_present);                            
NAME_EXCEPTION(stack_fault);                                                      
NAME_EXCEPTION(gen_protection);                                               
NAME_EXCEPTION(page_fault);                                                       
NAME_EXCEPTION(excep15);                                                           
NAME_EXCEPTION(math);                                                            
NAME_EXCEPTION(align_check);                                                     
NAME_EXCEPTION(machine);                                                           
NAME_EXCEPTION(simd);          



#endif // __EXCEPTIONS_H__

