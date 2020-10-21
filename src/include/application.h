#pragma once

// Sup types
#include "suptypes.h"

// Bootloader
#include "bl.h"
#include "context.h"

// Status enum
#include "ntstatus.h"

// Exports
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

#ifdef _ARM
#include <armintr.h>

#ifndef _ARM_INTRIN_SHORTCUT_
#define _ARM_INTRIN_SHORTCUT_
#define CP15_PMSELR 15, 0, 9, 12, 5 // Event Counter Selection Register
#define CP15_PMXEVCNTR 15, 0, 9, 13, 2 // Event Count Register
#define CP15_TPIDRURW                                                          \
  15, 0, 13, 0, 2 // Software Thread ID Register, User Read/Write
#define CP15_TPIDRURO                                                          \
  15, 0, 13, 0, 3 // Software Thread ID Register, User Read Only
#define CP15_TPIDRPRW                                                          \
  15, 0, 13, 0, 4 // Software Thread ID Register, Privileged Only
#define CP15_TTBR0 15, 0, 2, 0, 0
#define CP15_TLBIALL 15, 0, 8, 7, 0
#define CP15_BPIALL 15, 0, 7, 5, 6
#define CP15_SCTLR 15, 0, 1, 0, 0
#define CP15_VBAR 15, 0, 12, 0, 0
#define ArmInvalidateBTAC() _MoveToCoprocessor(0, CP15_BPIALL)
#define ArmDataMemoryBarrier() __dmb(_ARM_BARRIER_SY)
#define ArmDataSynchronizationBarrier() __dsb(_ARM_BARRIER_SY)
#define ArmInstructionSynchronizationBarrier() __isb(_ARM_BARRIER_SY)
#define ArmDisableInterrupts() _disable()
#define ArmEnableInterrupts() _enable()
#define ArmMoveToProcessor _MoveToCoprocessor
#endif

#endif
