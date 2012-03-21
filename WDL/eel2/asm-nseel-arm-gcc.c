/*
  notes:
  - it might be better and more efficient to store all constants
  in the literal pool, but it seems less safer than the NSEEL_LDR_WORD method
  bellow
  - 2pp, 2ppd (etc..) are untested
  
*/

#define NSEEL_NAKED __attribute__ ((naked))
#define NSEEL_NOP "mov r0, r0\n"

#define NSEEL_DECLARE_NAKED(x) \
  void x(void) NSEEL_NAKED; \
  void x(void)

#define NSEEL_DECLARE_NAKED_NOP(x) \
  NSEEL_DECLARE_NAKED(x) \
  { __asm__ ( NSEEL_NOP ); }

/*
for some reason this doesn't always work:
  ldr rR, =some_uint, where some_uint is 32bit hex directly or  
  ldr rR, =some_label, where some label is defined in a data section

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

NSEEL_DECLARE_NAKED(nseel_asm_1pdd)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_1pdd_end)

NSEEL_DECLARE_NAKED(nseel_asm_2pdd)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_2pdd_end)

NSEEL_DECLARE_NAKED(nseel_asm_2pdds)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r6, #0]\n"
    "str r1, [r6, #4]\n"
    "mov r0, r6\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_2pdds_end)

NSEEL_DECLARE_NAKED(nseel_asm_2pp)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "sub sp, sp, #64\n"
    "mov r1, r0\n"
    "mov r0, r6\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_2pp_end)

NSEEL_DECLARE_NAKED(nseel_asm_1pp)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_1pp_end)


//---------------------------------------------------------------------------------------------------------------

NSEEL_DECLARE_NAKED_NOP(nseel_asm_exec2)
NSEEL_DECLARE_NAKED_NOP(nseel_asm_exec2_end)

// slow
NSEEL_DECLARE_NAKED(nseel_asm_invsqrt)
{
  __asm__
  (
    "ldr r1, [r0, #4]\n"            // x
    "ldr r0, [r0, #0]\n"            //
    "mov lr, pc\n"
    "ldr pc, [r4, #16]\n"           // y = sqrt(x)
    "mov r2, r0\n"
    "mov r3, r1\n"
    NSEEL_LDR_WORD(0, 0x3ff00000)   // 1.0 = 0x3ff0000000000000
    "mov r1, #0\n"                  //
    "mov lr, pc\n"
    "ldr pc, [r4, #12]\n"           // 1.0 / y
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_invsqrt_end)

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_sqr)
{
  __asm__
  (
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "mov r2, r0\n"
    "mov r3, r1\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #8]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_sqr_end)
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
NSEEL_DECLARE_NAKED_NOP(nseel_asm_assign_end)

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_add)
{
  __asm__
  (
    "ldr r3, [r6, #4]\n"
    "ldr r2, [r6, #0]\n"
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #0]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_add_end)

NSEEL_DECLARE_NAKED(nseel_asm_add_op)
{

}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_add_op_end)


//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_sub)
{
  __asm__
  (
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #4]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_sub_end)

NSEEL_DECLARE_NAKED(nseel_asm_sub_op)
{

}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_sub_op_end)

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_mul)
{
  __asm__
  (
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #8]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_mul_end)

void nseel_asm_mul_op(void)
{

}
void nseel_asm_mul_op_end(void) {}

//---------------------------------------------------------------------------------------------------------------
NSEEL_DECLARE_NAKED(nseel_asm_div)
{
  __asm__
  (
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #12]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_div_end)

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

NSEEL_DECLARE_NAKED_NOP(_asm_megabuf_end)
