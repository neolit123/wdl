/*
  soft-float calls for 2x 64bit doubles are made using r0,r1,r2,r3
  return is r0, r1

  find how doubles are packed. big endian = blah
*/


#define NSEEL_NAKED __attribute__ ((naked))

#define NSEEL_DECLARE_NAKED(x) \
  void x(void) NSEEL_NAKED; \
  void x(void)

/*
for some reason this doesn't always work:
  ldr rR, =some_uint

so to load a large value (32bit) into register R, we do:

    e59fR008 	ldr	rR, [pc, #8]
    e59fR004 	ldr	rR, [pc, #4]
    e51fR000 	ldr	rR, [pc, #0]
    e51fR004 	ldr	rR, [pc, #-4]
    e51fR008 	ldr	rR, [pc, #-8]

usage:
  NSEEL_LDR_WORD(0, 0xdeadbeef) => ldr r5, =0xdeadbeef

this part:
  add pc, pc, #0
updates the pc and skips the word, since for some large values it tries
to execute it (e.g. 0xfc8b007a), but for some it doesn't (e.g. 0x40000000) (?)
*/

#if 1
  // ARM mode
  #define NSEEL_LDR_WORD(r, word) \
    ".word 0xe51f"#r"000\n" \
    "add pc, pc, #0\n" \
    ".word "#word"\n"
#else
  #define NSEEL_LDR_WORD(r, uint) \
    "ldr r"#r", ="#uint"\n"
#endif



  /*__asm__(
    "addis r5, 0, 0xdead\n"
    "ori r5, r5, 0xbeef\n"
    "lfd f1, 0(r3)\n"
    "mtctr r5\n"
    "subi r1, r1, 64\n"
    "bctrl\n"
    "addi r1, r1, 64\n"
    "stfdu f1, 8(r16)\n"
    "mr r3, r16\n"
   :: );
   */

void nseel_asm_1pdd(void) NSEEL_NAKED;
void nseel_asm_1pdd(void)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "ldr r0, [r6, #0]\n" // erm ?
    "ldr r1, [r6, #4]\n"
    "sub sp, sp, #64\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    // "mov pc, lr\n"
  );
}

void nseel_asm_1pdd_end(void){}

void nseel_asm_2pdd(void)
{

};
void nseel_asm_2pdd_end(void){}

void nseel_asm_2pdds(void)
{

}
void nseel_asm_2pdds_end(void){}


void nseel_asm_2pp(void)
{

};
void nseel_asm_2pp_end(void){}

void nseel_asm_1pp(void)
{

};
void nseel_asm_1pp_end(void){}


//---------------------------------------------------------------------------------------------------------------



// do nothing, eh
void nseel_asm_exec2(void)
{
}
void nseel_asm_exec2_end(void) { }



void nseel_asm_invsqrt(void)
{

}
void nseel_asm_invsqrt_end(void) {}

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_sqr)
{
  __asm__
  (
    "stmdb sp!, {ip, lr}\n"
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "mov r2, r0\n"
    "mov r3, r1\n"    
    "mov lr, pc\n"
    "ldr pc, [r10, #12]\n"    
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "add r8, r8, #4\n"
    "mov r1, r8\n"
    "ldmia sp!, {ip, pc}\n"
    "mov pc, lr\n" // dummy for NSEEL_code_execute(...) -> printf(...) ?
  );
}

NSEEL_DECLARE_NAKED(nseel_asm_sqr_end) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_abs(void)
{

}
void nseel_asm_abs_end(void) {}

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_assign)
{
  __asm__
  (
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "str r0, [r6, #0]\n"
    "str r1, [r6, #4]\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED(nseel_asm_assign_end) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_add(void)
{

}
void nseel_asm_add_end(void) {}

void nseel_asm_add_op(void)
{

}
void nseel_asm_add_op_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_sub(void)
{

}
void nseel_asm_sub_end(void) {}

void nseel_asm_sub_op(void)
{

}
void nseel_asm_sub_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_mul(void)
{

}
void nseel_asm_mul_end(void) {}

void nseel_asm_mul_op(void)
{

}
void nseel_asm_mul_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_div(void)
{

}
void nseel_asm_div_end(void) {}

void nseel_asm_div_op(void)
{

}
void nseel_asm_div_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_mod(void)
{

}
void nseel_asm_mod_end(void) {}

void nseel_asm_shl(void)
{

}
void nseel_asm_shl_end(void) {}

void nseel_asm_shr(void)
{

}
void nseel_asm_shr_end(void) {}

void nseel_asm_mod_op(void)
{


}
void nseel_asm_mod_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_or(void)
{

}
void nseel_asm_or_end(void) {}

void nseel_asm_or_op(void)
{

}
void nseel_asm_or_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_and(void)
{

}
void nseel_asm_and_end(void) {}

void nseel_asm_and_op(void)
{

}
void nseel_asm_and_op_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_uplus(void) // this is the same as doing nothing, it seems
{
}
void nseel_asm_uplus_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_uminus(void)
{

}
void nseel_asm_uminus_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_sign(void)
{

}
void nseel_asm_sign_end(void) {}



//---------------------------------------------------------------------------------------------------------------
void nseel_asm_bnot(void)
{

}
void nseel_asm_bnot_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_if(void)
{

}
void nseel_asm_if_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_repeat(void)
{

}
void nseel_asm_repeat_end(void) {}

void nseel_asm_repeatwhile(void)
{

}
void nseel_asm_repeatwhile_end(void) {}


void nseel_asm_band(void)
{

}
void nseel_asm_band_end(void) {}

void nseel_asm_bor(void)
{

}
void nseel_asm_bor_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_equal(void)
{

}
void nseel_asm_equal_end(void) {}
//
//---------------------------------------------------------------------------------------------------------------
void nseel_asm_notequal(void)
{

}
void nseel_asm_notequal_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_below(void)
{

}
void nseel_asm_below_end(void) {}

//---------------------------------------------------------------------------------------------------------------
void nseel_asm_beloweq(void)
{

}
void nseel_asm_beloweq_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_above(void)
{

}
void nseel_asm_above_end(void) {}

void nseel_asm_aboveeq(void)
{

}
void nseel_asm_aboveeq_end(void) {}



void nseel_asm_min(void)
{

}
void nseel_asm_min_end(void) {}

void nseel_asm_max(void)
{

}

void nseel_asm_max_end(void) {}









void _asm_generic3parm(void)
{

}
void _asm_generic3parm_end(void) {}

void _asm_generic3parm_retd(void)
{

}
void _asm_generic3parm_retd_end(void) {}


void _asm_generic2parm(void) // this prob neds to be fixed for ppc
{

}
void _asm_generic2parm_end(void) {}


void _asm_generic2parm_retd(void)
{

}
void _asm_generic2parm_retd_end(void) {}

void _asm_generic1parm(void) // this prob neds to be fixed for ppc
{

}
void _asm_generic1parm_end(void) {}



void _asm_generic1parm_retd(void)
{

}
void _asm_generic1parm_retd_end(void) {}




NSEEL_DECLARE_NAKED(_asm_megabuf)
{

}

NSEEL_DECLARE_NAKED(_asm_megabuf_end) {}
