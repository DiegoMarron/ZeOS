GETPID
test getpid: -

FORK 
fork father result: getpid/exit
fork child result: getpid/exit
fork inherit quantum: getpid/nice/exit  
fork father and children do not share quantum: getpid/nice/exit
fork reset stats: getstats/gepid/exit
fork out of range detection:sem_init/sem_signal/sem_wait/sem_destroy/exit
fork data structures recycling after exit: sem_init/sem_signal/sem_wait_sem_destroy/exit
fork data copy: inherit and do not share with father: getpid/sem_init/sem_signal/sem_wait_sem_destroy/exit
fork data copy: inherit and do not share with brothers: getpid/sem_init/sem_signal/sem_wait_sem_destroy/exit
fork process hierarchy creation: getpid/sem_init/sem_signal/sem_wait_sem_destroy/exit
TEST PHYSICAL MEMORY INITIALIZATION P0: exit

EXIT
exit(regular process): fork/sem_init/sem_destroy/nice
exit(task 0): nothing
exit deallocates memory: getpid/fork/sem_init/sem_wait/sem_signal/sem_destroy

PAGE FAULT
page fault(regular process): fork/sem_init/sem_signal/sem_wait/sem_destroy/get_stats
page fault(task 0): nothing

ROUND ROBIN
ROUND ROBIN:  fork/sem_init/sem_signal/sem_wait/sem_destroy/get_stats/nice
FULL QUANTUM AFTER CTX SWITCH: fork/nice/sem_wait/sem_signal/sem_init/sem_destroy/exit/getpid/get_stats

NICE
NICE(quantum parameter is negative): -
NICE(quantum parameter is 0): -
CHECK NICE RESULT: -
NICE EFFECTS ON SCHEDULING: fork/sem_init/sem_signal/sem_wait/sem_destroy/get_stats/nice

GET_STATS
GET_STATS(pid parameter is negative): -
GET_STATS(pid parameter does not exist): -
GET_STATS(address parameter is NULL): -
GET_STATS(address parameter in kernel address space): -
GET_STATS(address parameter outside user address space): -
GET_STATS(pid is current process): getpid
GET_STATS(process is ready): getpid/fork
GET_STATS(process is dead): fork/exit/sem_init/sem_signal/sem_wait/sem_destroy
GET_STATS(process is blocked): fork/exit/sem_init/sem_signal/sem_wait/sem_destroy
GET_STATS INCREASING TICKS FOR BLOCKED PROCESSES: fork/exit/sem_init/sem_signal/sem_wait/sem_destroy
GET_STATS INCREASING TICKS FOR RUNNING PROCESSES: getpid
TEST STATS OF P0: -


SEMAPHORES
SEM_INIT(invalid semaphores id: negative & out of range id): -
SEM_INIT(an already initialized semaphore): sem_destroy
SEM_DESTROY(invalid semaphore id: negative & out of range id): -
SEM_DESTROY(an uninitialized semaphore): -
SEM_WAIT ON TASK 0: getpid/sem_init/sem_signal/sem_destroy
SEM_SIGNAL(invalid semaphore id: negative & out of range id): -
SEM_SIGNAL(not initialized semaphore): -
TEST SEMAPHORE REUSE: sem_init/ sem_destroy 
SEM_WAIT(invalid semaphore id: negative & out of range id): getpid/fork
SEM_WAIT(not initialized semaphore): getpid/fork
SEM_SIGNAL(destroyed semaphore): sem_init/sem_destroy/exit
SEM_WAIT RETURN VALUE ON SUCCESS: getpid/fork/sem_init/sem_signal/sem_destroy/nice/exit
SEM_DESTROY(busy semaphore): sem_init/sem_wait/sem_signal/fork/exit/nice
SEM_DESTROY(busy semaphore):unblock sem_wait: sem_init/sem_signal/sem_wait/fork/exit/nice
SEM_DESTROY(not owner): getpid/fork/sem_init/sem_signal/sem_wait/exit
EXIT DESTROY OWNED SEMS: sem_init/sem_signal/sem_wait/fork/exit/nice
EXIT DO NOT DESTROY NOT OWNED SEMS: sem_init/sem_signal/sem_wait/fork/exit/nice
SYNCHRONIZING SEVERAL PROCESSES: sem_init/sem_signal/sem_wait/sem_destroy/fork/exit/get_stats
