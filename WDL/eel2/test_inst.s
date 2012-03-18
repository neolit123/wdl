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
    sub sp, sp, #4

  ldmfd  sp!, {pc}
