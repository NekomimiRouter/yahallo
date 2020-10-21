// DeviceLut.c: Device Look-up table

#include "Include/Application.h"

// Hack version LUT
VERSION_TABLE_ENTRY gVersionEntries[255] = {
    {
        // Surface RT v3.31.500, Tegra 3, needs firmware fix-up
        // gST string "OemkS EFI Jan 24 2014 18:00:42"
        L"OemkS EFI Jan 24 2014 18:00:42",
        SurfaceRTExploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // Surface 2 v4.22.500, Tegra 4
        // gST string "Surface 2 EFI Sep 11 2014 00:32:29"
        L"Surface 2 EFI Sep 11 2014 00:32:29",
        Surface2Exploit,
        NULL,
    },
    {
        // Terminator
        NULL,
    },
};

EFI_STATUS LaunchExploitByVersionTable(VOID)
{
  EFI_STATUS           Status;
  PVERSION_TABLE_ENTRY pEntry = NULL;

  if (gST->FirmwareVendor != NULL) {
    Print(
        L"Your firmware (gST): %s, 0x%x\n", gST->FirmwareVendor,
        gST->FirmwareRevision);
    Print(L"Matching device\n");

    PVERSION_TABLE_ENTRY pLut = (PVERSION_TABLE_ENTRY)&gVersionEntries;

    do {
      if (StrStr(gST->FirmwareVendor, pLut->FirmwareRelease) != NULL) {
        pEntry = pLut;
        break;
      }
      pLut++;
    } while (pLut->FirmwareRelease != NULL);
  }
  else {
    Print(L"[WARN] Failed to read firmware release from EFI System Table\n");
  }

  if (pEntry == NULL) {
    // Fix the console anyway (because we don't know)
    Tegra3ConsoleOutputFixup();
    Print(L"Yahallo - Tegra 3/4 Secure Boot Unlock Utility\n");
    Print(L"[ERROR] Failed to find the device. It is probably not supported "
          L"yet\n");
    Print(
        L"Your firmware (gST): %s, 0x%x\n", gST->FirmwareVendor,
        gST->FirmwareRevision);
    Status = EFI_NOT_FOUND;
    goto exit;
  }
  else {
    // Run pre-fix up if exists
    if (pEntry->PreEntryFixup != NULL) {
      HACK_ENTRY pFixupEntry = (HACK_ENTRY)pEntry->PreEntryFixup;
      pFixupEntry();
    }

    // Go
    Print(L"Enter the device routine\n");
    HACK_ENTRY pHackEntry = (HACK_ENTRY)pEntry->EntryPoint;
    pHackEntry();
  }

exit:
  return Status;
}
