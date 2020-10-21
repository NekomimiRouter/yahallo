// Smbios.c: firmware SMBIOS reader
// Source: https://blog.fpmurphy.com/2012/08/display-uefi-firmware-version-vendor-revision-and-build-date.html

#include "EFIApp.h"
#include "application.h"

#include <edk2/SmBios.h>

#define BUFSIZE 64

#define INVALID_HANDLE (UINT16)(-1)
#define DMI_INVALID_HANDLE 0x83
#define DMI_SUCCESS 0x00

STATIC SMBIOS_TABLE_ENTRY_POINT *    mSmbiosTable = NULL;
STATIC EDK2_SMBIOS_STRUCTURE_POINTER m_SmbiosStruct;
STATIC EDK2_SMBIOS_STRUCTURE_POINTER *mSmbiosStruct = &m_SmbiosStruct;

// Hack version LUT
VERSION_TABLE_ENTRY gVersionEntries[255] = {
    {
        // Surface RT v3.31.500, Tegra 3, needs firmware fix-up
        "v3.31.500",
        SurfaceRTExploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // Surface 2 v4.22.500, Tegra 4
        "v4.22.500",
        Surface2Exploit,
        NULL,
    },
    {
        // Terminator
        0,
    },
};

//
// Modified from EDK2 source code.  Copyright Intel Corporation.
//
CHAR8 *
Edk2LibGetSmbiosStringEdk2LibGetSmbiosString(EDK2_SMBIOS_STRUCTURE_POINTER *Smbios, UINT16 StringNumber)
{
  UINT16 Index;
  CHAR8 *String;

  ASSERT(Smbios != NULL);

  String = (CHAR8 *)(Smbios->Raw + Smbios->Hdr->Length);

  for (Index = 1; Index <= StringNumber; Index++) {
    if (StringNumber == Index) {
      return String;
    }
    for (; *String != 0; String++)
      ;
    String++;

    if (*String == 0) {
      Smbios->Raw = (UINT8 *)++String;
      return NULL;
    }
  }

  return NULL;
}

//
// Modified from EDK2 source code. Copyright Intel Corporation.
//
EFI_STATUS
Edk2LibGetSmbiosStructure(UINT16 *Handle, UINT8 **Buffer, UINT16 *Length)
{
  EDK2_SMBIOS_STRUCTURE_POINTER Smbios;
  EDK2_SMBIOS_STRUCTURE_POINTER SmbiosEnd;
  UINT8 *                       Raw;

  if (*Handle == INVALID_HANDLE) {
    *Handle = mSmbiosStruct->Hdr->Handle;
    return DMI_INVALID_HANDLE;
  }

  if ((Buffer == NULL) || (Length == NULL)) {
    Print(L"Invalid handle\n");
    return DMI_INVALID_HANDLE;
  }

  *Length       = 0;
  Smbios.Hdr    = mSmbiosStruct->Hdr;
  SmbiosEnd.Raw = Smbios.Raw + mSmbiosTable->TableLength;
  while (Smbios.Raw < SmbiosEnd.Raw) {
    if (Smbios.Hdr->Handle == *Handle) {
      Raw = Smbios.Raw;
      LibGetSmbiosString((SMBIOS_STRUCTURE_POINTER *) &Smbios, (UINT16)(-1));
      *Length = (UINT16)(Smbios.Raw - Raw);
      *Buffer = Raw;
      if (Smbios.Raw < SmbiosEnd.Raw) {
        *Handle = Smbios.Hdr->Handle;
      }
      else {
        *Handle = INVALID_HANDLE;
      }
      return DMI_SUCCESS;
    }
    LibGetSmbiosString((SMBIOS_STRUCTURE_POINTER *) &Smbios, (UINT16)(-1));
  }

  *Handle = INVALID_HANDLE;

  return DMI_INVALID_HANDLE;
}

EFI_STATUS LaunchExploitByVersionTable(VOID)
{
  EFI_GUID                      Guid = EFI_GLOBAL_VARIABLE;
  UINT16                        Handle;
  UINTN                         Index;
  UINT16                        Length;
  UINT8 *                       Buffer;
  EDK2_SMBIOS_STRUCTURE_POINTER SmbiosStruct;
  EFI_STATUS                    Status;

  CHAR8 *chVersion = NULL;
  CHAR8 *chRelease = NULL;
  PVERSION_TABLE_ENTRY pEntry    = NULL;

  Status =
      LibGetSystemConfigurationTable(&SMBIOSTableGuid, (VOID **)&mSmbiosTable);
  if ((Status != EFI_SUCCESS || mSmbiosTable == NULL) ||
      (CompareMem(mSmbiosTable->AnchorString, "_SM_", 4) != 0)) {
    // Should not happen
    Print(L"ERROR: SMBIOS table not found.\n");
    goto exit;
  }

  mSmbiosStruct->Raw = (UINT8 *)(UINTN)(mSmbiosTable->TableAddress);
  Handle = INVALID_HANDLE;
  Edk2LibGetSmbiosStructure(&Handle, NULL, NULL);

  // loop though the tables looking for a type 0 table.
  for (Index = 0; Index < mSmbiosTable->NumberOfSmbiosStructures; Index++) {
    if (Handle == INVALID_HANDLE) {
      break;
    }
    if (Edk2LibGetSmbiosStructure(&Handle, &Buffer, &Length) != DMI_SUCCESS) {
      break;
    }
    SmbiosStruct.Raw = Buffer;
    if (SmbiosStruct.Hdr->Type == 0) { // Type 0 - BIOS
      chVersion = LibGetSmbiosString((SMBIOS_STRUCTURE_POINTER *) &SmbiosStruct, 2);
      chRelease =
          LibGetSmbiosString((SMBIOS_STRUCTURE_POINTER *) &SmbiosStruct, 3);
      break;
    }
  }

  if (chVersion != NULL && chRelease != NULL) {
    PVERSION_TABLE_ENTRY pLut = (PVERSION_TABLE_ENTRY) &gVersionEntries;
    do {
      if (memcmp(chVersion, pLut->FirmwareVersion, strlena(chVersion)) == 0) {
        pEntry = pLut;
        break;
      }
      pLut++;
    } while (pLut->FirmwareVersion[0] != '\0');
  }

  if (pEntry == NULL) {
    // Fix the console anyway
    Tegra3ConsoleOutputFixup();
    Print(L"Yahallo - Tegra 3/4 Secure Boot Unlock Utility\n");
    Print(L"[ERROR] Failed to find the device. It is probably not supported yet\n");
    Status = EFI_NOT_FOUND;
    goto exit;
  }
  else {
    // Run pre-fix up if exists
    if (pEntry->PreEntryFixup != NULL) {
      HACK_ENTRY pFixupEntry = (HACK_ENTRY) pEntry->PreEntryFixup;
      pFixupEntry();

      HACK_ENTRY pHackEntry = (HACK_ENTRY) pEntry->EntryPoint;
      pHackEntry();
    }
  }

exit:
  return Status;
}
