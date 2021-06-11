// Devices.h: device-specific exploit routines
#ifndef _DEVICE_H_
#define _DEVICE_H_

typedef struct _SURFACE2_FW_OFFSET_INFO {
  // Non secure world offsets
  UINT32 UefiNsWorldBufferStartOffset;
  UINT32 UefiNsWorldBufferEndOffset;
  // Secure world offsets
  UINT32 UefiSecureWorldRuntimeFlag1Offset;
  UINT32 UefiSecureWorldRuntimeFlag2Offset;
  UINT32 UefiSecureWorldSetupModeStringOffset;
  UINT32 UefiSecureWorldSecureBootStringOffset;
} SURFACE2_FW_OFFSET_INFO, *PSURFACE2_FW_OFFSET_INFO;

VOID SurfaceRTExploit(VOID);
VOID Surface2Exploit(PSURFACE2_FW_OFFSET_INFO pFwInfo);

VOID Surface2_Fw_2_06_500_Exploit(VOID);
VOID Surface2_Fw_4_22_500_Exploit(VOID);

#endif
