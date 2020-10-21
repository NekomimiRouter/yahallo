// ProcessorSupport.h: ARM specific processor support

#pragma once

#ifdef _ARM
#include <armintr.h>

#ifndef _ARM_INTRIN_SHORTCUT_
#define _ARM_INTRIN_SHORTCUT_
#define CP15_PMSELR 15, 0, 9, 12, 5    // Event Counter Selection Register
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

#ifndef _ARM_PROC_SUPPORT_H_
#define _ARM_PROC_SUPPORT_H_

#define CACHE_LINE 64

#define ID_MMFR0_SHARELVL_SHIFT 12
#define ID_MMFR0_SHARELVL_MASK 0xf
#define ID_MMFR0_SHARELVL_ONE 0
#define ID_MMFR0_SHARELVL_TWO 1

#define ID_MMFR0_INNERSHR_SHIFT 28
#define ID_MMFR0_INNERSHR_MASK 0xf
#define ID_MMFR0_OUTERSHR_SHIFT 8
#define ID_MMFR0_OUTERSHR_MASK 0xf

#define ID_MMFR0_SHR_IMP_UNCACHED 0
#define ID_MMFR0_SHR_IMP_HW_COHERENT 1
#define ID_MMFR0_SHR_IGNORED 0xf

typedef VOID (*ARM_V7_CACHE_OPERATION)(UINT32);

UINTN EFIAPI ArmReadCntFrq(VOID);

BOOLEAN
EFIAPI
ArmGetInterruptState(VOID);

/**
 * The size of the SMC arguments are different between AArch64 and AArch32.
 * The native size is used for the arguments.
 */
typedef struct {
  UINTN Arg0;
  UINTN Arg1;
  UINTN Arg2;
  UINTN Arg3;
  UINTN Arg4;
  UINTN Arg5;
  UINTN Arg6;
  UINTN Arg7;
} ARM_SMC_ARGS;

/**
  Trigger an SMC call
  SMC calls can take up to 7 arguments and return up to 4 return values.
  Therefore, the 4 first fields in the ARM_SMC_ARGS structure are used
  for both input and output values.
**/
VOID ArmCallSmc(IN OUT ARM_SMC_ARGS *Args);

VOID EFIAPI ArmEnableInstructionCache(VOID);

VOID EFIAPI ArmEnableDataCache(VOID);

VOID EFIAPI ArmEnableMmu(VOID);

VOID EFIAPI ArmDisableMmu(VOID);

VOID EFIAPI ArmDisableCachesAndMmu(VOID);

VOID EFIAPI ArmInvalidateTlb(VOID);

VOID EFIAPI ArmInvalidateDataCacheEntryBySetWay(UINTN SetWayFormat);
VOID EFIAPI ArmCleanInvalidateDataCacheEntryBySetWay(IN UINTN SetWayFormat);

VOID ArmV7AllDataCachesOperation(ARM_V7_CACHE_OPERATION DataCacheOperation);

VOID EFIAPI ArmInvalidateDataCache(VOID);
VOID EFIAPI ArmInvalidateInstructionCache(VOID);
VOID EFIAPI ArmCleanInvalidateDataCache(VOID);

/**
  Scans a target buffer for an 8-bit value, and returns a pointer to the
  matching 8-bit value in the target buffer.
  @param  Buffer  The pointer to the target buffer to scan.
  @param  Length  The count of 8-bit value to scan. Must be non-zero.
  @param  Value   The value to search for in the target buffer.
  @return The pointer to the first occurrence or NULL if not found.
**/
CONST VOID *EFIAPI
      InternalMemScanMem8(IN CONST VOID *Buffer, IN UINTN Length, IN UINT8 Value);

#endif

#endif
