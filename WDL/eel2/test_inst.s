/*

  arm-elf-gcc test_inst.s -s && arm-elf-objdump -d a.out > objdump.txt  

  addr at:
                          stmfd  sp!, {lr}
    ~ 8210: 0x..........
                          ldmfd  sp!, {pc}  
*/
	.global	main
	.type	main, %function  
main:
  stmfd  sp!, {lr}

  # addr at:
  str r0, [r6, #0]

  ldmfd  sp!, {pc}
  
.L2:
  .word 0xfc8b007a
