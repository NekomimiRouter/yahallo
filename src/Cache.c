#include "EFIApp.h"
#include "application.h"

VOID ArmV7DataCacheOperation(IN ARM_V7_CACHE_OPERATION DataCacheOperation)
{
  UINTN SavedInterruptState;

  SavedInterruptState = ArmGetInterruptState();
  ArmDisableInterrupts();

  ArmV7AllDataCachesOperation(DataCacheOperation);

  ArmDataSynchronizationBarrier();

  if (SavedInterruptState) {
    ArmEnableInterrupts();
  }
}

VOID EFIAPI ArmInvalidateDataCache(VOID)
{
  ArmDataSynchronizationBarrier();
  ArmV7DataCacheOperation(ArmInvalidateDataCacheEntryBySetWay);
}

VOID EFIAPI ArmCleanInvalidateDataCache(VOID)
{
  ArmDataSynchronizationBarrier();
  ArmV7DataCacheOperation(ArmCleanInvalidateDataCacheEntryBySetWay);
}

VOID CortexA15CachePrime(VOID)
{
  UINTN PageSize = 4096;
  UINTN Base     = 0x83000000;

  Print(L"CortexA15CachePrime: priming cache...\n");

  for (UINTN Address = 0; Address < (8 * 1024 * 1024); Address += PageSize) {
    if ((Address % (1 * 1024 * 1024)) == 0) {
      Print(L"CortexA15CachePrime: preloading 0x%08x\n", Base + Address);
    }

    __prefetch((VOID *)(Base + Address));
  }

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();

  Print(L"CortexA15CachePrime: Done.\n");
}
