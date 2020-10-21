#pragma once

#include "bl.h"
#include "suptypes.h"

#ifdef _ARM
// Switch to specific context on AArch32 (ARMv7A) platform.
// Reverse engineered from developermenu.efi ARMv7A
void SwitchToRealModeContext(PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor);
#endif
