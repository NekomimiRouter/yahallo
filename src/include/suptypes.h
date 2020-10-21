#pragma once

typedef long          NTSTATUS;
typedef unsigned long ULONG;
typedef unsigned long DWORD, *PDWORD, *LPDWORD;

#ifdef _ARM
typedef unsigned int size_t;
#define CONTROL_COPROCESSPOR_CP15 15
#endif