#include "Include/Application.h"

VOID CortexA15CachePrime(VOID)
{
  UINTN PageSize = 4096;
  UINTN Base     = 0x83000000;

  for (UINTN Address = 0; Address < (8 * 1024 * 1024); Address += PageSize) {
    __builtin_prefetch((VOID *)(Base + Address), 1, 3);
    __builtin_prefetch((VOID *)(Base + Address), 0, 3);
  }

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();
}
