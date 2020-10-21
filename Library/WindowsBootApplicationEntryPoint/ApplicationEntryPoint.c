/** @file
  Entry point library instance to a Windows Boot Manager application.

Copyright (c) 2007 - 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 1996 - 2020, The ReactOS Project. All rights reserved.<BR>
Copyright (c) 2019 - 2020, Bingxing Wang and other project authors. All rights
reserved.<BR> SPDX-License-Identifier: GPL-2.0-only

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/WindowsBootApplicationEntryPoint.h>

#include <Library/ArmLib.h>
#include <Library/CacheMaintenanceLib.h>

#include "Arm/ArmV7Lib.h"

/**
  Entry point to UEFI Application.

  This function is the entry point for a UEFI Application. This function must
call ProcessLibraryConstructorList(), ProcessModuleEntryPointList(), and
ProcessLibraryDestructorList(). The return value from
ProcessModuleEntryPointList() is returned. If _gUefiDriverRevision is not zero
and SystemTable->Hdr.Revision is less than _gUefiDriverRevison, then return
EFI_INCOMPATIBLE_VERSION.

  @param  ImageHandle                The image handle of the UEFI Application.
  @param  SystemTable                A pointer to the EFI System Table.

  @retval  EFI_SUCCESS               The UEFI Application exited normally.
  @retval  EFI_INCOMPATIBLE_VERSION  _gUefiDriverRevision is greater than
SystemTable->Hdr.Revision.
  @retval  Other                     Return value from
ProcessModuleEntryPointList().

**/
EFI_STATUS
EFIAPI
_EfiModuleEntryPoint(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  if (_gUefiDriverRevision != 0) {
    //
    // Make sure that the EFI/UEFI spec revision of the platform is >= EFI/UEFI
    // spec revision of the application.
    //
    if (SystemTable->Hdr.Revision < _gUefiDriverRevision) {
      return EFI_INCOMPATIBLE_VERSION;
    }
  }

  //
  // Call constructor for all libraries.
  //
  ProcessLibraryConstructorList(ImageHandle, SystemTable);

  //
  // Call the module's entry point
  //
  Status = ProcessModuleEntryPointList(ImageHandle, SystemTable);

  //
  // Process destructor for all libraries.
  //
  ProcessLibraryDestructorList(ImageHandle, SystemTable);

  //
  // Return the return status code from the driver entry point
  //
  return Status;
}

/**
  Invokes the library destructors for all dependent libraries and terminates
  the UEFI Application.

  This function calls ProcessLibraryDestructorList() and the EFI Boot Service
Exit() with a status specified by Status.

  @param  Status  Status returned by the application that is exiting.

**/
VOID EFIAPI Exit(IN EFI_STATUS Status)

{
  ProcessLibraryDestructorList(gImageHandle, gST);

  gBS->Exit(gImageHandle, Status, 0, NULL);
}

/**
  Required by the EBC compiler and identical in functionality to
_ModuleEntryPoint().

  @param  ImageHandle  The image handle of the UEFI Application.
  @param  SystemTable  A pointer to the EFI System Table.

  @retval  EFI_SUCCESS               The UEFI Application exited normally.
  @retval  EFI_INCOMPATIBLE_VERSION  _gUefiDriverRevision is greater than
SystemTable->Hdr.Revision.
  @retval  Other                     Return value from
ProcessModuleEntryPointList().

**/
EFI_STATUS
EFIAPI
EfiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  return _EfiModuleEntryPoint(ImageHandle, SystemTable);
}

STATIC
VOID BlpArmSetExeceptionContext(IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  UINT32 VbarValue;

  VbarValue = FirmwareDescriptor->ExceptionState.Vbar;
  ArmWriteVBar(FirmwareDescriptor->ExceptionState.Vbar);
  ArmInstructionSynchronizationBarrier();
}

STATIC
VOID BlpArmSetThreadContext(IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  UINT32 TpidrprwValue;
  UINT32 SctlrValue;

  TpidrprwValue = FirmwareDescriptor->ExceptionState.IdSvcRW;
  ArmWriteTpidrprw(TpidrprwValue);
  ArmInstructionSynchronizationBarrier();

  SctlrValue = FirmwareDescriptor->ExceptionState.Control;
  ArmWriteSctlr(SctlrValue);

  ArmInvalidateTlb();
  ArmInvalidateBtac();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();
}

STATIC
VOID BlpArmSetPagingContext(IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  UINT32 TtbrValue;

  TtbrValue = FirmwareDescriptor->MmState.HardwarePageDirectory |
              FirmwareDescriptor->MmState.TTB_Config;
  ArmSetTTBR0((VOID *)TtbrValue);
  ArmInstructionSynchronizationBarrier();

  ArmInvalidateTlb();
  ArmInvalidateBtac();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();
}

STATIC
UINT32 BlpArmDisableInterrupts(IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  UINT32 OldInterruptState;

  OldInterruptState = FirmwareDescriptor->InterruptState;
  ArmDisableInterrupts();

  return OldInterruptState;
}

STATIC
VOID BlpArmEnableInterrupts(IN UINT32 OldInterruptState)
{
  if (OldInterruptState) {
    ArmEnableInterrupts();
  }
}

VOID BlpArmSwitchToFirmwareContext(
    IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  UINT32 OldInterruptState;

  OldInterruptState = BlpArmDisableInterrupts(FirmwareDescriptor);
  BlpArmSetPagingContext(FirmwareDescriptor);
  BlpArmSetThreadContext(FirmwareDescriptor);
  BlpArmSetExeceptionContext(FirmwareDescriptor);
  BlpArmEnableInterrupts(OldInterruptState);
}

STATIC
VOID BlpEfiMain(IN PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{
  EFI_SYSTEM_TABLE *SystemTable;
  EFI_HANDLE        ImageHandle;

  if (FirmwareDescriptor->SystemTable) {
    SystemTable = FirmwareDescriptor->SystemTable;
    ImageHandle = FirmwareDescriptor->ImageHandle;

    _EfiModuleEntryPoint(ImageHandle, SystemTable);
  }
}

/**
 Windows Boot Application's version of _ModuleEntryPoint.

 This function is the entry point for a Windows Boot Application. This function
 performs required processor state switch, then hands over control to
 EFI's entry point, see BlpEfiMain for details.

 @param BootAppParameters           Boot Application Params
 @param LibraryParameters           Library Params, remains unused.

 @retval STATUS_SUCCESS             The boot application exited normally.
 @retval STATUS_INVALID_PARAMETER   The boot application entry doesn't contain
valid app params.

**/
NTSTATUS
_ModuleEntryPoint(
    IN PBOOT_APPLICATION_PARAMETER_BLOCK BootAppParameters,
    IN PBL_LIBRARY_PARAMETERS LibraryParameters)
{
  PBL_LIBRARY_PARAMETERS  LibraryParams;
  PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor;
  UINT32                  ParamPointer;

  if (!BootAppParameters) {
    return STATUS_INVALID_PARAMETER;
  }

  LibraryParams      = LibraryParameters;
  ParamPointer       = (UINT32)BootAppParameters;
  FirmwareDescriptor = (PBL_FIRMWARE_DESCRIPTOR)(
      ParamPointer + BootAppParameters->FirmwareParametersOffset);

  BlpArmSwitchToFirmwareContext(FirmwareDescriptor);
  BlpEfiMain(FirmwareDescriptor);

  return STATUS_SUCCESS;
}
