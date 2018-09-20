
# Buffer Overflow Attack Example With C

The file `main.c` includes a brief example of a buffer overflow attack, which enables us to force our program to skip some lines of code. The approach is to find and change the return address from the stack. 

This example is prepared to motivate readers to be aware of basic vulnerabilities and to program better. Remember, C is a very powerful language but as *Bjarne Stroustrup* said *it makes it easy to shoot yourself in the foot*.

## Analysis of the program

Let us start by analyzing the assemly of our main program.
```
gcc -g main.c 
gdb a.out
(gdb) disass main
Dump of assembler code for function main:
   ...
   0x0000555555554705 <+15>:	mov    $0x20,%edi
   0x000055555555470a <+20>:	callq  0x5555555546aa <function>
   0x000055555555470f <+25>:	subl   $0x1,-0x4(%rbp)
   0x0000555555554713 <+29>:	subl   $0x2,-0x4(%rbp)
   0x0000555555554717 <+33>:	subl   $0x3,-0x4(%rbp)
   0x000055555555471b <+37>:	subl   $0x4,-0x4(%rbp)
   0x000055555555471f <+41>:	mov    -0x4(%rbp),%eax
   ...
```
We can see that our program will call our function, then it will continue to execute `subl` instructions.

Now we continue by analyzing the content of the stack when our function is called. 
```
gdb a.out
(gdb) break function
(gdb) run
(gdb) info frame
Stack level 0, frame at 0x7fffffffdca0:
 rip = 0x5555555546b5 in function (main.c:11); saved rip = 0x55555555470f
 called by frame at 0x7fffffffdcc0
 source language c.
 Arglist at 0x7fffffffdc90, args: param=32
 Locals at 0x7fffffffdc90, Previous frame's sp is 0x7fffffffdca0
 Saved registers:
  rbp at 0x7fffffffdc90, rip at 0x7fffffffdc98
```
We can see that the return address when our function ends is stored in `rip at 0x7fffffffdc98`. The program will return to execute the instructions continued from the address `saved rip = 0x55555555470f`, which points to the first instruction (`subl`).

## Approach

If we can access to the memory adress `0x7fffffffdc98` where `rip` is stored, we can change it with an address which will manipulate the program flow, and skip some lines in the code. 

We can use the address of a local variable which is pushed to the stack, `buffer1` in our case, to change the value of the return adress.
```
gdb a.out
(gdb) break function
(gdb) run
(gdb) p &buffer1
$1 = (char (*)[2]) 0x7fffffffdc86
```
By adding 18 to this address ( `0x7fffffffdc98 - 0x7fffffffdc86 = 18 , decimal`) we can point to the same memory address.
Lets say that we  want to skip 2 lines of code. Also we can note that each `subl` instruction is stored in 4 bytes, by the help of `disass main`. So we will add 8 to the value stored at our return address pointer.


Now we see that the main program printed 13 instead of 10.