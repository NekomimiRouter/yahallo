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