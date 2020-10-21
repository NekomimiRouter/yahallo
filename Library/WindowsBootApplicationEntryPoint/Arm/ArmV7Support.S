#------------------------------------------------------------------------------
#
# Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
# Copyright (c) 2011 - 2014, ARM Limited. All rights reserved.
# Copyright (c) 2016, Linaro Limited. All rights reserved.
# Copyright (c) 2020, Bingxing Wang. All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
#------------------------------------------------------------------------------

#include <AsmMacroIoLib.h>

.set DC_ON, (0x1<<2)
.set IC_ON, (0x1<<12)
.set CTRL_M_BIT,  (1 << 0)
.set CTRL_C_BIT,  (1 << 2)
.set CTRL_B_BIT,  (1 << 7)
.set CTRL_I_BIT,  (1 << 12)


ASM_FUNC(ArmReadTpidrprw)
  mrc     p15, 0, r0, c13, c0, 4    @ read TPIDRPRW
  bx      lr

ASM_FUNC(ArmWriteTpidrprw)
  mcr     p15, 0, r0, c13, c0, 4    @ write TPIDRPRW
  bx      lr

ASM_FUNC(ArmInvalidateBtac)
  mcr     p15, 0, r0, c7, c5, 6     @Invalidate Branch predictor array
  bx      lr

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
