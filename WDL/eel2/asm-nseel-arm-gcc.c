/*
  notes:
  - it might be better and more efficient to store all constants
  in the literal pool, but it seems less safer than the NSEEL_LDR_WORD method
  bellow
  - 2pp, 2ppd (etc..) are untested
*/

#define NSEEL_SIGN_MASK   0x80000000
#define NSEEL_NOP         "mov r0, r0\n"

#define NSEEL_STR_EXPAND(x) #x
#define NSEEL_STR(x) NSEEL_STR_EXPAND(x)

/* naked f. declarations */
#define NSEEL_NAKED __attribute__ ((naked))
#define NSEEL_DECLARE_NAKED(x) \
  void x(void) NSEEL_NAKED; \
  void x(void)
/* it seems that the tested compiler does not like empty naked functions */
#define NSEEL_DECLARE_NAKED_NOP(x) \
  NSEEL_DECLARE_NAKED(x) \
  { __asm__ ( NSEEL_NOP ); }

/*
  load a word into register using:
  e59fR004 	ldr	rR, [pc, #4]
  e51fR000 	ldr	rR, [pc, #0]
  e51fR004 	ldr	rR, [pc, #-4]
  etc...
*/
#if 1
  // note: for ARM mode only
  #define NSEEL_LDR_WORD(r, word) \
    ".word 0xe51f"NSEEL_STR(r)"000\n" \
    "add pc, pc, #0\n" \
    ".word "NSEEL_STR(word)"\n"
#else
  #define NSEEL_LDR_WORD(r, word) \
    "ldr r"NSEEL_STR(r)", ="NSEEL_STR(word)"\n"
#endif

/* compiler / library dependent, local functions */
#define NSEEL_OPT_LEVEL 3
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 4)
  #define NSEEL_OPTIMIZE_ATTR __attribute__((optimize(NSEEL_OPT_LEVEL)))
#else
  #define NSEEL_OPTIMIZE_ATTR
#endif
#define NSEEL_DECLARE_LOCAL_FP2(x) \
  double x(const double a, const double b) NSEEL_OPTIMIZE_ATTR; \
  double x(const double a, const double b)

NSEEL_DECLARE_LOCAL_FP2(nseel_add)
{
  return a + b;
}

NSEEL_DECLARE_LOCAL_FP2(nseel_sub)
{
  return a - b;
}

NSEEL_DECLARE_LOCAL_FP2(nseel_mul)
{
  return a * b;
}

NSEEL_DECLARE_LOCAL_FP2(nseel_div)
{
  return a / b;
}

/* the version for doubles does not work very well with the tested compiler */
NSEEL_DECLARE_LOCAL_FP2(nseel_invsqrt)
{
  const float a2 = a*0.5f;
  float y = (float)a;
  unsigned int i = *(unsigned int *)&y;

  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  return (double)(y*(1.5f - (a2*y*y)));
}

NSEEL_DECLARE_LOCAL_FP2(nseel_min)
{
  return (a < b) ? a : b;
}

NSEEL_DECLARE_LOCAL_FP2(nseel_max)
{
  return (a > b) ? a : b;
}

/* do we really need to move the sp for these ?*/
NSEEL_DECLARE_NAKED(nseel_asm_1pdd)
{
  __asm__
  (
    NSEEL_LDR_WORD(5, 0xdeadbeef)
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    // "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    // "add sp, sp, #64\n"
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
    // "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    // "add sp, sp, #64\n"
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
    // "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    // "add sp, sp, #64\n"
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
    // "sub sp, sp, #64\n"
    "mov r1, r0\n"
    "mov r0, r6\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    // "add sp, sp, #64\n"
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
    // "sub sp, sp, #64\n"
    "mov lr, pc\n"
    "mov pc, r5\n"
    // "add sp, sp, #64\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_1pp_end)

NSEEL_DECLARE_NAKED_NOP(nseel_asm_exec2)
NSEEL_DECLARE_NAKED_NOP(nseel_asm_exec2_end)

// "q3a" version single precision
NSEEL_DECLARE_NAKED(nseel_asm_invsqrt)
{
  __asm__
  (
    "ldr r1, [r0, #4]\n"
    "ldr r0, [r0, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #16]\n"   // nseel_invsqrt
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_invsqrt_end)


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

void nseel_asm_abs(void)
{

}
void nseel_asm_abs_end(void) {}


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


void nseel_asm_or(void)
{

}
void nseel_asm_or_end(void) {}

void nseel_asm_or_op(void)
{

}
void nseel_asm_or_op_end(void) {}


void nseel_asm_and(void)
{

}
void nseel_asm_and_end(void) {}

void nseel_asm_and_op(void)
{

}
void nseel_asm_and_op_end(void) {}

NSEEL_DECLARE_NAKED_NOP(nseel_asm_uplus)
NSEEL_DECLARE_NAKED_NOP(nseel_asm_uplus_end)

NSEEL_DECLARE_NAKED(nseel_asm_uminus)
{
  __asm__
  (
    "uminus_start:\n"
    "   ldr r1, [r0, #4]\n"
    "   ldr r0, [r0, #0]\n"
        NSEEL_LDR_WORD(2, NSEEL_SIGN_MASK)
    "   and r3, r0, r2\n"
    "   cmp r3, #0\n"
    "   bne uminus_unset\n"
    "   orr r0, r0, r2\n"
    "   b uminus_done\n"
    "uminus_unset:\n"
    "   bic r0, r0, r2\n"
    "uminus_done:\n"
    "   str r0, [r8, #0]\n"
    "   str r1, [r8, #4]\n"
    "   mov r0, r8\n"
    "   mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_uminus_end)

void nseel_asm_sign(void)
{

}
void nseel_asm_sign_end(void) {}




void nseel_asm_bnot(void)
{
}
void nseel_asm_bnot_end(void) {}


void nseel_asm_if(void)
{

}
void nseel_asm_if_end(void) {}


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


void nseel_asm_equal(void)
{

}
void nseel_asm_equal_end(void) {}
//

void nseel_asm_notequal(void)
{

}
void nseel_asm_notequal_end(void) {}



void nseel_asm_below(void)
{

}
void nseel_asm_below_end(void) {}


void nseel_asm_beloweq(void)
{

}
void nseel_asm_beloweq_end(void) {}


void nseel_asm_above(void)
{

}
void nseel_asm_above_end(void) {}

void nseel_asm_aboveeq(void)
{

}
void nseel_asm_aboveeq_end(void) {}

NSEEL_DECLARE_NAKED(nseel_asm_min)
{
  __asm__
  (
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #20]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_min_end)

NSEEL_DECLARE_NAKED(nseel_asm_max)
{
  __asm__
  (
    "ldr r3, [r0, #4]\n"
    "ldr r2, [r0, #0]\n"
    "ldr r1, [r6, #4]\n"
    "ldr r0, [r6, #0]\n"
    "mov lr, pc\n"
    "ldr pc, [r4, #24]\n"
    "str r0, [r8, #0]\n"
    "str r1, [r8, #4]\n"
    "mov r0, r8\n"
    "mov pc, lr\n"
  );
}
NSEEL_DECLARE_NAKED_NOP(nseel_asm_max_end)










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
