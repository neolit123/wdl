// 64bit ?

void nseel_asm_1pdd(void)
{

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
void nseel_asm_sqr(void)
{
  /*    
    soft-float calls for 2x 64bit doubles are made using r0,r1,r2,r3    
    return is r0, r1
    
    find how doubles are packed. big endian = blah    
  */

  __asm__
  (
    //"stmfd sp!, {r0-r12, lr}\n"
    "bl __floatunsidf\n"
    "mov r3, r1\n"
    "mov r2, r0\n"
    "bl __muldf3\n"
    "mov pc, lr\n"
    //"ldmfd sp!, {r0-r12, pc}\n"
  );
}
void nseel_asm_sqr_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_abs(void)
{

}
void nseel_asm_abs_end(void) {}


//---------------------------------------------------------------------------------------------------------------
void nseel_asm_assign(void)
{
  char str[] = "nseel_asm_assign\0"; // should be on the stack
  __asm__
  (
    "ldr r0, [%0]\n"
    "bl puts\n"
    "mov pc, lr\n"
    :: "g" (str)
  );
}
void nseel_asm_assign_end(void) {}

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




void _asm_megabuf(void)
{

}

void _asm_megabuf_end(void) {}
