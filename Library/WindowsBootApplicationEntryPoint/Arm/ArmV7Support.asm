//------------------------------------------------------------------------------
//
// Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
// Copyright (c) 2011 - 2014, ARM Limited. All rights reserved.
// Copyright (c) 2020, Bingxing Wang. All rights reserved.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//
//------------------------------------------------------------------------------


    INCLUDE AsmMacroExport.inc
    PRESERVE8

DC_ON           EQU     ( 0x1:SHL:2 )
IC_ON           EQU     ( 0x1:SHL:12 )
CTRL_M_BIT      EQU     (1 << 0)
CTRL_C_BIT      EQU     (1 << 2)
CTRL_B_BIT      EQU     (1 << 7)
CTRL_I_BIT      EQU     (1 << 12)


 RVCT_ASM_EXPORT ArmReadTpidrprw
  mrc     p15, 0, r0, c13, c0, 4   ; read TPIDRPRW
  bx      lr

 RVCT_ASM_EXPORT ArmWriteTpidrprw
  mcr     p15, 0, r0, c13, c0, 4   ; write TPIDRPRW
  bx      lr

 RVCT_ASM_EXPORT ArmInvalidateBtac
  mcr     p15, 0, r0, c7, c5, 6    ; Invalidate Branch predictor array
  bx      lr

 END
