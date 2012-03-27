#include "ns-eel.h"
#include <string.h>
#include <math.h>

void NSEEL_HOSTSTUB_EnterMutex()
{
}
void NSEEL_HOSTSTUB_LeaveMutex()
{
}

int main()
{
  printf("init\n");
  if (NSEEL_init())
  {
    printf("Error initializing EEL\n");
    return -1;
  }

  printf("alloc\n");
  NSEEL_VMCTX vm = NSEEL_VM_alloc();
  if (!vm)
  {
    printf("Error creating VM\n");
    return -1;
  }

  printf("reg\n");
  double *var_ret = NSEEL_VM_regvar(vm,"ret");

  if (var_ret) *var_ret = 0.0;
  
  printf("compile\n");
  char buf[1024];
  strncpy(buf,"ret = 12345678901234567890.0 % 1111111111.0;",sizeof(buf));

  // note that you shouldnt pass a readonly string directly, since it may need to
  // fudge with the string during the compilation (it will always restore it to the
  // original value though).
  NSEEL_CODEHANDLE ch = NSEEL_code_compile(vm,buf,0);

  if (ch)
  {
    int n;
    for (n = 0; n < 3; n++)
    {
      NSEEL_code_execute(ch);
      printf("pass(%d), ret=%.16f\n",n+1,var_ret ? *var_ret : 0.0);
    }

    NSEEL_code_free(ch);
  }
  else
  {
    char *err = NSEEL_code_getcodeerror(vm);

    printf("Error compiling code at '%s'\n",err?err:"????");
  }

  NSEEL_VM_free(vm);

  #ifdef __arm__
    puts(""); // the simulator needs a dummy line at the end
  #endif

  return 0;
}
