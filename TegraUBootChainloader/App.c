#include "Include/Application.h"

VOID EFIAPI CLdrpDiscoverGopHandles(VOID)
{
  EFI_HANDLE *GopHandles;
  UINTN       NumberGopHandles;
  EFI_STATUS  Status;

  Status = gBS->LocateHandleBuffer(
      ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &NumberGopHandles,
      &GopHandles);

  if (EFI_ERROR(Status)) {
    return;
  }

  for (UINTN Handle = 0; Handle < NumberGopHandles; Handle++) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *EfiGopProtocol = NULL;

    Status = gBS->HandleProtocol(
        GopHandles[Handle], &gEfiGraphicsOutputProtocolGuid,
        (VOID **)&EfiGopProtocol);

    if (EFI_ERROR(Status) || EfiGopProtocol == NULL) {
      continue;
    }

    Print(
        L"Handle %d: PixelFormat %d, ResH %d, ResV %d, Scanline %d\n", Handle,
        EfiGopProtocol->Mode->Info->PixelFormat,
        EfiGopProtocol->Mode->Info->HorizontalResolution,
        EfiGopProtocol->Mode->Info->VerticalResolution,
        EfiGopProtocol->Mode->Info->PixelsPerScanLine);

    if (EfiGopProtocol->Mode->Info->PixelFormat ==
            PixelBlueGreenRedReserved8BitPerColor ||
        EfiGopProtocol->Mode->Info->PixelFormat ==
            PixelRedGreenBlueReserved8BitPerColor) {
      Print(
          L"Handle %d: FrameBuffer base 0x%x, size 0x%x\n", Handle,
          EfiGopProtocol->Mode->FrameBufferBase,
          EfiGopProtocol->Mode->FrameBufferSize);
    }
  }
}

EFI_STATUS
ChainloaderEntryPoint(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status                      = EFI_SUCCESS;
  UINT32     RandomMemoryBottomReadValue = 0xFFFFFFFF;

  // Turn off watchdog timer, since this does take a while
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // Perform console fix-up
  Tegra3ConsoleOutputFixup();
  ArmDisableBranchPrediction();

  // Acquire some information about the GOP handle
  CLdrpDiscoverGopHandles();

  // Run exploit
  PerformNvTegra3Exploit();

  // Verify status
  ArmDisableCachesAndMmu();
  RandomMemoryBottomReadValue = *((UINT32 *)(EFI_PHYSICAL_ADDRESS)0x80000000);
  ArmEnableMmu();
  ArmEnableDataCache();
  ArmEnableInstructionCache();

  if (RandomMemoryBottomReadValue == 0xFFFFFFFF) {
    Print(L"[ERROR] Exploit failed.\n");
    Status = EFI_NOT_READY;
    goto exit;
  }
  else {
    Print(
        L"Random memory readout at TZ is 0x%x\n", RandomMemoryBottomReadValue);
  }

  // Read about MPIDR
  UINT32 MpIdr = ArmReadMpidr();
  Print(L"Current MPIDR: 0x%x\n", MpIdr);

  // Read U-Boot from file system
  EFI_HANDLE *FileSystemHandles;
  UINTN       NumberFileSystemHandles;
  VOID *      FileBuffer = NULL;
  UINTN       BufferSize = 0;

  Status = gBS->LocateHandleBuffer(
      ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL,
      &NumberFileSystemHandles, &FileSystemHandles);

  if (EFI_ERROR(Status)) {
    Print(L"Failed to acquire handles.\n");
    goto exit;
  }

  for (UINTN Handle = 0; Handle < NumberFileSystemHandles; Handle++) {
    EFI_FILE_PROTOCOL *              UBootFileProtocol = NULL;
    EFI_FILE_PROTOCOL *              EfiFileProtocol   = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *EfiSfsProtocol    = NULL;

    EFI_FILE_INFO *PayloadFileInformation;
    UINTN          PayloadFileInformationSize = 0;

    Status = gBS->HandleProtocol(
        FileSystemHandles[Handle], &gEfiSimpleFileSystemProtocolGuid,
        (VOID **)&EfiSfsProtocol);

    if (EFI_ERROR(Status) || EfiSfsProtocol == NULL) {
      continue;
    }

    Status = EfiSfsProtocol->OpenVolume(EfiSfsProtocol, &EfiFileProtocol);
    if (EFI_ERROR(Status) || EfiFileProtocol == NULL) {
      continue;
    }

    Status = EfiFileProtocol->Open(
        EfiFileProtocol, &UBootFileProtocol, L"u-boot-uefi.bin",
        EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status) || UBootFileProtocol == NULL) {
      continue;
    }

    // Read this file
    Status = UBootFileProtocol->GetInfo(
        UBootFileProtocol, &gEfiFileInfoGuid, &PayloadFileInformationSize,
        (VOID *)PayloadFileInformation);

    if (Status == EFI_BUFFER_TOO_SMALL) {
      Status = gBS->AllocatePool(
          EfiLoaderData, PayloadFileInformationSize,
          (VOID **)&PayloadFileInformation);

      if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate pool for file info: %r\n", Status);
        UBootFileProtocol->Close(UBootFileProtocol);
        continue;
      }

      SetMem((VOID *)PayloadFileInformation, PayloadFileInformationSize, 0xFF);

      Status = UBootFileProtocol->GetInfo(
          UBootFileProtocol, &gEfiFileInfoGuid, &PayloadFileInformationSize,
          (VOID *)PayloadFileInformation);

      if (EFI_ERROR(Status)) {
        Print(L"Failed to get file info: %r\n", Status);
        gBS->FreePool((VOID *)PayloadFileInformation);
        UBootFileProtocol->Close(UBootFileProtocol);
        continue;
      }
    }

    // Allocate buffer
    BufferSize = PayloadFileInformation->FileSize;
    Print(L"Pool size: 0x%x\n", BufferSize);
    Status = gBS->AllocatePool(EfiLoaderCode, BufferSize, &FileBuffer);
    if (EFI_ERROR(Status) || FileBuffer == NULL) {
      Print(L"Failed to allocate pool\n");
      continue;
    }

    // Read this file
    ZeroMem(FileBuffer, BufferSize);
    Status =
        UBootFileProtocol->Read(UBootFileProtocol, &BufferSize, FileBuffer);
    if (EFI_ERROR(Status)) {
      Print(L"Failed to read U-Boot\n");
      UBootFileProtocol->Close(UBootFileProtocol);
      continue;
    }

    Print(L"Successfully read U-Boot\n");
    UBootFileProtocol->Close(UBootFileProtocol);
    break;
  }

  EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);

  // Copy U-Boot to destination
  Print(L"Copy U-Boot\n");
  Print(L"Copy U-Boot\n");
  Print(L"Copy U-Boot\n");

  ArmDisableCachesAndMmu();
  ArmDisableInterrupts();
  CopyMem((VOID *)0x80110000, FileBuffer, BufferSize);

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  WriteBackInvalidateDataCache();
  InvalidateInstructionCache();

  ArmEnableMmu();
  ArmEnableDataCache();
  ArmEnableInstructionCache();
  ArmEnableInterrupts();

  Print(L"U-Boot copy complete\n");
  Print(L"Jump\n");

  Print(L"U-Boot copy complete\n");
  Print(L"Jump\n");

  Print(L"U-Boot copy complete\n");
  Print(L"Jump\n");

  // De-initialize
  ArmDisableInterrupts();
  ArmDisableAsynchronousAbort();
  ArmDisableCachesAndMmu();

  // Jump
  UBOOT_ENTRY pEntry = (void *)0x80110000;
  pEntry();
  gBS->RestoreTPL(Tpl);

exit:
  Print(L"Why we are here?\n");
  CpuDeadLoop();
  return Status;
}
