// VersionTable.h: a LUT for specific firmware and hacks
#pragma once

typedef struct _VERSION_TABLE_ENTRY {
  char  FirmwareVersion[255];
  void *EntryPoint;
  void *PreEntryFixup;
} VERSION_TABLE_ENTRY, *PVERSION_TABLE_ENTRY;

void SurfaceRTExploit();
void Surface2Exploit();

typedef void (*HACK_ENTRY)(void);
