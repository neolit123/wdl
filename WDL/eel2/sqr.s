/*
  sqr.s
  convert a 32bit unsigned int to 64bit double and return its square
  test other stuff as well...

  arm-elf-gcc sqr.s -g && arm-elf-run a.out
*/

  .global main
  .type main, %function
  .align 2
test:
  mul r0, r1, r1
  mov pc, lr
main:
  stmfd sp!, {r2-r8, lr}
  ldr r1, =5
  ldr r0, =test 
  mov lr, pc 
  mov pc, r0
  bl __floatunsidf
  mov r3, r1
  mov r2, r0
  bl __muldf3
  mov r2, r1
  mov r1, r0
  ldr r0, .L3
  bl printf
  ldmfd sp!, {r2-r8, pc}
.LC0:
  .ascii "%f\012\000"
.L3:
  .word .LC0
