/*
  the new glue code for arm isn't done yet.
  
  TODO...
  
  we are mostly porting ppc -> arm
  but we don't have as many regiters:
  
  r3 -> r0  
  r5 -> r5
  r14 -> r6
  r15 -> r7
  r16 -> r8
  r17 -> r9
  
  r4 reserved and used to pass a function table with some nseel_asm_* pointers around.
	should always be saved/restored on/from the stack when special calls are made.
*/

#ifndef _NSEEL_GLUE_ARM_H_
#define _NSEEL_GLUE_ARM_H_

#define GLUE_MAX_FPSTACK_SIZE 0 // no stack support
// 2^23 = 8388608. has to be tested though.
#define GLUE_MAX_JMPSIZE 8000000 // maximum relative jump size for this arch (if not defined, any jump is possible)

/*
	untested and i might have messed this one up.
	encoded immediate addr for 'b':
		enc(24bit) = ((target_offset - pc) >> 2) & 0xFFFFFF
		b_machine_format(32bit) = b_opcode <conc> enc
*/ 
#define GLUE_JMP_SET_OFFSET(endOfInstruction, offset) \
	(((int *)(endOfInstruction))[-1] = \
		(((offset) - (((int *)(endOfInstruction))[-1] + 8)) >> 2) & 0xFFFFFF)

/* 24bit relative jumps */
static const unsigned char GLUE_JMP_NC[] =
{
  0xea, 0x0, 0x0, 0x0 // b <offset>
};

static const unsigned char GLUE_JMP_IF_P1_Z[] =
{
  0xe3, 0x50, 0x0, 0x0, // cmp r0, #0
  0x0a, 0x0, 0x0, 0x0   // beq <offset> (zb = 0)
};

static const unsigned char GLUE_JMP_IF_P1_NZ[] =
{
  0xe3, 0x50, 0x0, 0x0, // cmp r0, #0
  0x1a, 0x0, 0x0, 0x0   // bne <offset> (zb != 0)
};

#define GLUE_MOV_PX_DIRECTVALUE_SIZE 8
static void GLUE_MOV_PX_DIRECTVALUE_GEN(void *b, INT_PTR v, int wv) 
{   
  static const unsigned int tab[3] = {
    0xe51f0004, // ldr r0, [pc, #-4]
    0xe51f6004, // ldr r6, [pc, #-4]
    0xe51f6004  // ldr r7, [pc, #-4]
  };
  
  *((unsigned int *)b) = tab[wv];
  b = ((unsigned int *)b) + 1;
  *(unsigned int *)b = v; // [pc, #-4]
}

// mov r5, lr
// str r5, [sp, #-4]
// sub sp, sp, #4
static const unsigned int GLUE_FUNC_ENTER[3] = { 0xe1a0500e, 0xe50d5004, 0xe24dd004 };

// ldr r5, [sp]
// add sp, sp, #4
// mov lr, r5
static const unsigned int GLUE_FUNC_LEAVE[3] = { 0xe59d5000, 0xe28dd004, 0xe1a0e005 };

/*
// mflr r5
// stwu r5, -8(r1)
const static unsigned int GLUE_FUNC_ENTER[2] = { 0x7CA802A6, 0x94A1FFF8 };

// lwz r5, 0(r1)
// addi r1, r1, 8
// mtlr r5
const static unsigned int GLUE_FUNC_LEAVE[3] = { 0x80A10000, 0x38210008, 0x7CA803A6 };
*/

#define GLUE_FUNC_ENTER_SIZE sizeof(GLUE_FUNC_ENTER)
#define GLUE_FUNC_LEAVE_SIZE sizeof(GLUE_FUNC_LEAVE)

// const static unsigned int GLUE_RET[]={0x4E800020}; // blr
static const unsigned int GLUE_RET[] = { 0xe1a0f00e }; // mov pc, lr

static int GLUE_RESET_WTP(unsigned char *out, void *ptr)
{
  // const static unsigned int GLUE_SET_WTP_FROM_R17=0x7E308B78; // mr r16 (dest), r17 (src)
  const static unsigned int GLUE_SET_WTP_FROM_R9 = 0xe1a08009; // mov r8, r9
  if (out) memcpy(out,&GLUE_SET_WTP_FROM_R9,sizeof(GLUE_SET_WTP_FROM_R9));
  return sizeof(GLUE_SET_WTP_FROM_R9);
}

/*
// stwu r3, -8(r1)
const static unsigned int GLUE_PUSH_P1[1]={ 0x9461FFF8};
*/

// str r0, [sp, #-8]
// sub sp, sp, #8
static const unsigned int GLUE_PUSH_P1[2] = { 0xe50d0004, 0xe24dd004 };


#define GLUE_POP_PX_SIZE 8
static void GLUE_POP_PX(void *b, int wv)
{
  /*
	static const unsigned int tab[3] ={
      0x80610000, // lwz r3, 0(r1)
      0x81c10000, // lwz r14, 0(r1)
      0x81e10000, // lwz r15, 0(r1)
  };
  ((unsigned int *)b)[0] = tab[wv];
  ((unsigned int *)b)[1] = 0x38210008; // addi r1,r1, 8
  */
  
  static const unsigned int tab[3] ={
      0xe59d0000, // ldr r0, [sp]
      0xe59d6000, // ldr r6, [sp]
      0xe59d7000, // ldr r7, [sp]
  };
  
  ((unsigned int *)b)[0] = tab[wv];
  ((unsigned int *)b)[1] = 0xe28dd008; // add sp, sp, #8
}

#define GLUE_SET_PX_FROM_P1_SIZE 4
static void GLUE_SET_PX_FROM_P1(void *b, int wv)
{
  /*
	static const unsigned int tab[3]={
    0x7c631b78, // never used: mr r3, r3
    0x7c6e1b78, // mr r14, r3
    0x7c6f1b78, // mr r15, r3
  };
  *(unsigned int *)b  = tab[wv];
  */  
  
  static const unsigned int tab[3]={
    0xe1a00000,  // mov r0, r0 = nop 
    0xe1a06000,  // mov r6, r0
    0xe1a07000,  // mov r7, r0
  };
  *(unsigned int *)b  = tab[wv];
}


// TODO: i forgot what register were used for the doubles...

// ---------

// lfd f2, 0(r3)
// stfdu f2, -8(r1)
static const unsigned int GLUE_PUSH_P1PTR_AS_VALUE[] = { 0xC8430000, 0xDC41FFF8 };

static int GLUE_POP_VALUE_TO_ADDR(unsigned char *buf, void *destptr)
{    
  // lfd f2, 0(r1)
  // addi r1,r1,8
  // GLUE_MOV_PX_DIRECTVALUE_GEN / GLUE_MOV_PX_DIRECTVALUE_SIZE (r3)
  // stfd f2, 0(r3)
  if (buf)
  {
    unsigned int *bufptr = (unsigned int *)buf;
    *bufptr++ = 0xC8410000;
    *bufptr++ = 0x38210008;    
    GLUE_MOV_PX_DIRECTVALUE_GEN(bufptr, (INT_PTR)destptr,0);
    bufptr += GLUE_MOV_PX_DIRECTVALUE_SIZE/4;
    *bufptr++ = 0xd8430000;
  }
  return 2*4 + GLUE_MOV_PX_DIRECTVALUE_SIZE + 4;
}

static int GLUE_COPY_VALUE_AT_P1_TO_PTR(unsigned char *buf, void *destptr)
{    
  // lfd f2, 0(r3)
  // GLUE_MOV_PX_DIRECTVALUE_GEN / GLUE_MOV_PX_DIRECTVALUE_SIZE (r3)
  // stfd f2, 0(r3)

  if (buf)
  {
    unsigned int *bufptr = (unsigned int *)buf;
    *bufptr++ = 0xc8430000;
    GLUE_MOV_PX_DIRECTVALUE_GEN(bufptr, (INT_PTR)destptr,0);
    bufptr += GLUE_MOV_PX_DIRECTVALUE_SIZE/4;
    *bufptr++ = 0xd8430000;
  }
  
  return 4 + GLUE_MOV_PX_DIRECTVALUE_SIZE + 4;
}


// ---------

static void GLUE_CALL_CODE(INT_PTR bp, INT_PTR cp, INT_PTR rt)
{
  static const double consttab[] = { 
    NSEEL_CLOSEFACTOR, 
    4503601774854144.0 /* 0x43300000, 0x80000000, used for integer conversion*/, 
  };
  
  // %2, %3 are a bit of an issue here.
  // hmm, also we don't use "0x4330000080000000" for truncation in the soft-float port

  __asm__
  (
    ".data\n"
    "_flist0:\n"
    " .word " \
  /* i wonder if we should define the table in c instead */

/* 0    */  "nseel_add          ," \
/* 4    */  "nseel_sub          ," \
/* 8    */  "nseel_mul          ," \
/* 12   */  "nseel_div          ," \
/* 16   */  "nseel_invsqrt      ," \
/* 20   */  "nseel_min          ," \
/* 24   */  "nseel_max          ," \
/* 28   */  "nseel_truncate     ," \
/* 32   */  "nseel_and          ," \
/* 36   */  "nseel_or           ," \
/* 40   */  "nseel_xor          ," \
/* 44   */  "nseel_shl          ," \
/* 48   */  "nseel_shr          ," \
/* 52   */  "nseel_mod          ," \
/* 56   */  "nseel_floor        ," \
/* 60   */  "nseel_ceil          " \
    " \n"
    ".text\n"
    "stmfd sp!, {r0-r12, lr}\n"
    "ldr r4, =_flist0\n"
    "mov r9, %1\n"
    "sub r9, r9, #8\n"
    "mov r5, lr\n"
    "mov pc, %0\n"
    "ldmia sp!, {r0-r12, pc}\n"
    "mov lr, r5\n"
    ".pool\n"
    :: "r" (cp), "r" (bp), "r" (rt), "r" (consttab)
  );
  
  /*  
  // we could have r18 refer to the current user-stack pointer, someday, perhaps
  __asm__(
          "subi r1, r1, 128\n"
          "stfd f31, 8(r1)\n"
          "stfd f30, 16(r1)\n"
          "stmw r13, 32(r1)\n"
          "mtctr %0\n"
          "mr r17, %1\n" 
          "mr r13, %2\n"
          "lfd f31, 0(%3)\n"
          "lfd f30, 8(%3)\n"
          "subi r17, r17, 8\n"
          "mflr r0\n" 
          "stw r0, 24(r1)\n"
          "bctrl\n"
          "lwz r0, 24(r1)\n"
          "mtlr r0\n"
          "lmw r13, 32(r1)\n"
          "lfd f31, 8(r1)\n"
          "lfd f30, 16(r1)\n"
          "addi r1, r1, 128\n"
            ::"r" (cp), "r" (bp), "r" (rt), "r" (consttab));
  */
  
};

// the callback should work without problems
INT_PTR *EEL_GLUE_set_immediate(void *_p, void *newv)
{
  INT_PTR *p = (INT_PTR *)_p;
 
  while (p[0] != 0xdeadbeef) p++;
  p[0] = (INT_PTR)newv;
  return (INT_PTR *)++p;
}

#define GLUE_SET_PX_FROM_WTP_SIZE sizeof(int)
static void GLUE_SET_PX_FROM_WTP(void *b, int wv)
{
  /*
	static const unsigned int tab[3]={
    0x7e038378, // mr r3, r16
    0x7e0e8378, // mr r14, r16
    0x7e0f8378, // mr r15, r16
  };
  *(unsigned int *)b = tab[wv];
  */  
  
  // looks like we write the return address conditionally here
  static const unsigned int tab[3]={
    0xe1a00008, // mov r0, r8
    0xe1a06008, // mov r6, r8
    0xe1a07008, // mov r7, r8
  };
  *(unsigned int *)b = tab[wv];
}

// TODO (everything bellow)

static int GLUE_POP_FPSTACK_TO_PTR(unsigned char *buf, void *destptr)
{
  // set r3 to destptr
  // stfd f1, 0(r3)
  if (buf)
  {
    unsigned int *bufptr = (unsigned int *)buf;
    GLUE_MOV_PX_DIRECTVALUE_GEN(bufptr, (INT_PTR)destptr,0);
    bufptr += GLUE_MOV_PX_DIRECTVALUE_SIZE/4;

    *bufptr++ = 0xD8230000; // stfd f1, 0(r3)
  }
  return GLUE_MOV_PX_DIRECTVALUE_SIZE + sizeof(int);
}
// --------

#define GLUE_POP_FPSTACK_SIZE 0
static const unsigned int GLUE_POP_FPSTACK[1] = { 0 }; // no need to pop, not a stack

static const unsigned int GLUE_POP_FPSTACK_TOSTACK[] = {
  0xdc21fff8, // stfdu f1, -8(r1)
};

static const unsigned int GLUE_POP_FPSTACK_TO_WTP[] = { 
  0xdc300008, // stfdu f1, 8(r16)
};

#define GLUE_PUSH_VAL_AT_PX_TO_FPSTACK_SIZE 4
static void GLUE_PUSH_VAL_AT_PX_TO_FPSTACK(void *b, int wv)
{
  static const unsigned int tab[3] = {
    0xC8230000, // lfd f1, 0(r3)
    0xC82E0000, // lfd f1, 0(r14)
    0xC82F0000, // lfd f1, 0(r15)
  };
  *(unsigned int *)b = tab[wv];
}

#define GLUE_POP_FPSTACK_TO_WTP_TO_PX_SIZE (sizeof(GLUE_POP_FPSTACK_TO_WTP) + GLUE_SET_PX_FROM_WTP_SIZE)
static void GLUE_POP_FPSTACK_TO_WTP_TO_PX(unsigned char *buf, int wv)
{
  memcpy(buf,GLUE_POP_FPSTACK_TO_WTP,sizeof(GLUE_POP_FPSTACK_TO_WTP));
  GLUE_SET_PX_FROM_WTP(buf + sizeof(GLUE_POP_FPSTACK_TO_WTP),wv); // ppc preincs the WTP, so we do this after
};

static unsigned int GLUE_POP_STACK_TO_FPSTACK[1] = { 0 }; // todo


static const unsigned int GLUE_SET_P1_Z[] =  { 0x38600000 }; // li r3, 0
static const unsigned int GLUE_SET_P1_NZ[] = { 0x38600001 }; // li r3, 1

// end of arm

#endif
