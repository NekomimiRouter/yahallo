#pragma once
#ifndef _EFIAPP_H_

#include <efi.h>
#include <efilib.h>

// Util for Stall
#ifndef _EFI_STALL_UTIL_
#define _EFI_STALL_UTIL_
#define SECONDS_TO_MICROSECONDS(x) x * 1000000
#endif

#include "ProcessorSupport.h"
#include "mem.h"
#include "VersionTable.h"

// Console Utility
VOID Tegra3ConsoleOutputFixup(VOID);

// SMC Utility
STATIC
UINT32
ArmCallSmcHelper(UINT32 R0, UINT32 R1, UINT32 R2, UINT32 R3);

VOID PerformNvExploit(VOID);

// Cache Utility
VOID CortexA15CachePrime(VOID);

// SMBIOS Utility
EFI_STATUS LaunchExploitByVersionTable(VOID);

#endif