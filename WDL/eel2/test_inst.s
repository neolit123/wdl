/*

  arm-elf-gcc test_inst.s -s && arm-elf-objdump -d a.out > objdump.txt  

  addr at:
    ~ 8210: 0x..........
*/
	.global	main
	.type	main, %function
main:
  stmfd  sp!, {lr}
  # addr at:
  mov r8, r9
  ldmfd  sp!, {pc}
