#include "EFIApp.h"
#include "application.h"

// This is the actual entrypoint.
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0
// compatibility)
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      KeyEvent;

  InitializeLib(ImageHandle, SystemTable);

  // Turn off watchdog timer, since this does take a while
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // Search the proper entry
  Status = LaunchExploitByVersionTable();

  Print(L"[INFO] Press any key to exit\n");
  gST->ConIn->Reset(gST->ConIn, FALSE);
  gST->BootServices->WaitForEvent(1, &gST->ConIn->WaitForKey, &KeyEvent);
  return Status;
}
