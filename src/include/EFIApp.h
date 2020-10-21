#pragma once
#ifndef _EFIAPP_H_

#include <efi.h>
#include <efilib.h>

#include "ProcessorSupport.h"

// Util for Stall
#ifndef _EFI_STALL_UTIL_
#define _EFI_STALL_UTIL_
#define SECONDS_TO_MICROSECONDS(x) x * 1000000
#endif

// Memory Utility
VOID *EFIAPI ScanMem8(IN CONST VOID *Buffer, IN UINTN Length, IN UINT8 Value);

#define memchr(buf, ch, count) ScanMem8(buf, (UINTN)(count), (UINT8)ch)
#define memcmp(buf1, buf2, count) (int)(CompareMem(buf1, buf2, (UINTN)(count)))

static char *
twobyte_memmem(const unsigned char *h, size_t k, const unsigned char *n);
static char *
threebyte_memmem(const unsigned char *h, size_t k, const unsigned char *n);
static char *
fourbyte_memmem(const unsigned char *h, size_t k, const unsigned char *n);

#define _MAX(a, b) ((a) > (b) ? (a) : (b))
#define _MIN(a, b) ((a) < (b) ? (a) : (b))

#define BITOP(a, b, op)                                                        \
  ((a)[(size_t)(b) / (8 * sizeof *(a))] op(size_t) 1                           \
   << ((size_t)(b) % (8 * sizeof *(a))))

static char *twoway_memmem(
    const unsigned char *h, const unsigned char *z, const unsigned char *n,
    size_t l);

void *memmem(const void *haystack, size_t n, const void *needle, size_t m);

#endif