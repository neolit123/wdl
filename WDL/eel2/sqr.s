/*
  sqr.s
  convert a 32bit unsigned int to 64bit double and return its square
*/

  .global main
  .type main, %function
  .align 2
main:
  stmfd sp!, {r2-r8, lr}
  ldr r0, =5
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
