#include "Include/Application.h"

VOID CortexA15CachePrime(VOID)
{
  UINTN PageSize = 4096;
  UINTN Base     = 0x83000000;

  Print(L"CortexA15CachePrime: priming cache...\n");

  for (UINTN Address = 0; Address < (8 * 1024 * 1024); Address += PageSize) {
    if ((Address % (1 * 1024 * 1024)) == 0) {
      Print(L"CortexA15CachePrime: preloading 0x%08x\n", Base + Address);
    }

    __builtin_prefetch((VOID *)(Base + Address), 1, 3);
    __builtin_prefetch((VOID *)(Base + Address), 0, 3);
  }

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();

  Print(L"CortexA15CachePrime: Done.\n");
}
