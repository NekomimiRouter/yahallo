#include "Include/Application.h"

EFI_STATUS TegraSecureBootUnlockEntryPoint(
    EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  // Turn off watchdog timer, since this does take a while
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // Search the proper entry
  Status = LaunchExploitByVersionTable();

  // We won't let people escape here. They need to manually reset
  FinalizeApp();
  return Status;
}

VOID FinalizeApp(VOID)
{
  // Let people wait for stroke
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  CpuDeadLoop();
}
