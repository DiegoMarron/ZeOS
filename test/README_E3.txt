Nom pantalla    = DISPLAY
Nom teclat      = KEYBOARD

Flags:  O_RDONLY=1, O_WRONLY=2, O_RDWR=(O_RDONLY|O_WRONLY)=3, O_CREAT=4

UNLINK

UNLINK(open a file after removing it) -> open/close
UNLINK(path is NULL) -> NOTHING
UNLINK(path in kernel address space) -> NOTHING
UNLINK(path outside user address space) -> NOTHING
UNLINK(path with nametoolong) -> NOTHING
UNLINK(path of a non-existing file) -> NOTHING
UNLINK(file is being used by me) -> open/close
UNLINK(file occupying the whole disk space) -> open/write/close
UNLINK(shared file after closing it) -> open/fork/close/exit/sem_init/sem_wait/sem_signal/sem_destroy

WRITE
     
WRITE(fd is negative) -> NOTHING
WRITE(fd is closed) -> NOTHING
WRITE(fd is RDONLY) -> NOTHING
WRITE(buffer is NULL) -> NOTHING
WRITE(size is 0) -> open/close/unlink
WRITE(write 4 bytes to a file) -> open/close/unlink
WRITE(writing in a file after reopening it) -> open/read/close/unlink
WRITE(write several blocks to a file) -> open/read/close/unlink
WRITE(write 1 block to a file) -> open/read/close/unlink
WRITE(buffer to be written contains '\0') -> open/read/close/unlink
WRITE(size to be written is smaller than strlen(buffer)) -> open/read/close/unlink
WRITE(fd greater than MAX FDs) -> NOTHING
WRITE(buffer in kernel address space) -> NOTHING
WRITE(buffer outside user address space) -> NOTHING
WRITE(writing until disk is full) -> open/close/unlink

READ

READ(process 0 reads from keyboard) -> read
READ(fd is negative) -> getpid/fork
READ(fd is closed) -> getpid/fork
READ(fd is WRONLY) -> getpid/fork
READ(read from fd==0  on a null pointer buffer) -> getpid/fork
READ(read 1 char from keyboard (fd == 0)) -> getpid/fork/write
READ(read 4 char from keyboard (fd == 0)) -> getpid/fork/write
READ(read 1 char from a file with 1024 bytes) -> getpid/fork/open/write/close/unlink
READ(read 1 block (256 bytes) from a file) -> getpid/fork/open/write/close/unlink
READ(read 3 blocks (256*3 bytes) from a file) -> getpid/fork/open/write/close/unlink
READ(read 3 blocks (256*3 bytes) from a file 2 blocks long) -> getpid/fork/open/write/close/unlink
READ(read 1 char at the end of a file) -> getpid/fork/open/write/close/unlink
READ(access to non consecutive blocks) -> getpid/fork/open/write/close/unlink
READ(sharing read/write pointer: read after writing on a new file) -> getpid/fork/open/write/close/unlink
READ(independent read/write pointers using opens) -> getpid/fork/open/write/close/unlink
READ(empty file) -> getpid/fork/open/write/close/unlink
READ(read from keyboard (fd == 0) more bytes than keyboard buffer capacity) -> getpid/fork/open/write/close/unlink
READ(sharing read/write pointer after fork) -> getpid/fork/open/write/sem_init/sem_wait/sem_signal/sem_destroy/close/unlink
READ(fd greater than MAX FDs) -> getpid/fork
READ(read from fd==0 on buffer in kernel address space) -> getpid/fork
READ(read from fd==0 on an buffer outside user address space ) -> getpid/fork

OPEN

OPEN(path is NULL, ...) -> close
OPEN(path is in kernel address space, ...) -> close
OPEN(path is a very long filename, ...) -> close
OPEN(path is a very long filename and O_CREAT flag is enabled, ...) -> close
OPEN(a nonexistant file, ...) -> close
OPEN(a nonexistant file, O_CREAT) -> close
OPEN(keyboard with unknownflags) -> close
OPEN(keyboard with incorrect write mode) -> close
OPEN(console with incorrect read mode) -> close
OPEN(console with incorrect read+write mode) -> close
OPEN(keyboard with incorrect read+write mode) -> close
OPEN(opening already existing keyboard) -> close
OPEN(creating already existing keyboard) -> close
OPEN(keyboard and console with the right modes) -> close
OPEN(path is outside of user address space, ...) -> close
OPEN(More than the Maximum number of file descriptors) -> close

DUP

DUP(file descriptor has a negative value) -> close
DUP(file descriptor is already closed ) -> close
DUP(an opened file descriptor(keyboard)) -> close
DUP(Maximum number of file descriptors) -> close
DUP(a newly created file and check that the file pointer is shared) -> open, read, write, close, unlink
DUP(file descriptor is out of range) -> close


CLOSE

CLOSE(file descriptor is a negative value) -> NOTHING
CLOSE(file descriptor is out of range) -> NOTHING
CLOSE(file descriptor is already closed) -> NOTHING
CLOSE(a copy of keyboard's file descriptor) -> dup
