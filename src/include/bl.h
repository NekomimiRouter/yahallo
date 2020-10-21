// bl.h: Windows Boot Manager Types
// This file comes from the ReactOS project.

#pragma once

#include <efi.h>
#include <efilib.h>

/* DEFINES *******************************************************************/

#define BL_APPLICATION_FLAG_CONVERTED_FROM_EFI 0x01

#define BL_APP_ENTRY_SIGNATURE "BTAPENT"

#define BOOT_APPLICATION_SIGNATURE_1 'TOOB'
#define BOOT_APPLICATION_SIGNATURE_2 ' PPA'

#define BOOT_MEMORY_TRANSLATION_TYPE_PHYSICAL 0
#define BOOT_MEMORY_TRANSLATION_TYPE_VIRTUAL 1

#define BOOT_APPLICATION_VERSION 2
#define BL_MEMORY_DATA_VERSION 1
#define BL_RETURN_ARGUMENTS_VERSION 1
#define BL_FIRMWARE_DESCRIPTOR_VERSION 2

#define BL_RETURN_ARGUMENTS_NO_PAE_FLAG 0x40

#define BL_APPLICATION_ENTRY_FLAG_NO_GUID 0x01
#define BL_APPLICATION_ENTRY_BCD_OPTIONS_INTERNAL 0x02
#define BL_APPLICATION_ENTRY_WINLOAD 0x04
#define BL_APPLICATION_ENTRY_STARTUP 0x08
#define BL_APPLICATION_ENTRY_REBOOT_ON_ERROR 0x20
#define BL_APPLICATION_ENTRY_NTLDR 0x40
#define BL_APPLICATION_ENTRY_BCD_OPTIONS_EXTERNAL 0x80
#define BL_APPLICATION_ENTRY_WINRESUME 0x100
#define BL_APPLICATION_ENTRY_SETUPLDR 0x200
#define BL_APPLICATION_ENTRY_BOOTSECTOR 0x400
#define BL_APPLICATION_ENTRY_BOOTMGR 0x1000
#define BL_APPLICATION_ENTRY_DISPLAY_ORDER 0x800000
#define BL_APPLICATION_ENTRY_FIXED_SEQUENCE 0x20000000
#define BL_APPLICATION_ENTRY_RECOVERY 0x40000000

#define BL_CONTEXT_PAGING_ON 1
#define BL_CONTEXT_INTERRUPTS_ON 2

#define BL_MM_FLAG_USE_FIRMWARE_FOR_MEMORY_MAP_BUFFERS 0x01
#define BL_MM_FLAG_REQUEST_COALESCING 0x02

#define BL_MM_ADD_DESCRIPTOR_COALESCE_FLAG 0x01
#define BL_MM_ADD_DESCRIPTOR_TRUNCATE_FLAG 0x02
#define BL_MM_ADD_DESCRIPTOR_NEVER_COALESCE_FLAG 0x10
#define BL_MM_ADD_DESCRIPTOR_NEVER_TRUNCATE_FLAG 0x20
#define BL_MM_ADD_DESCRIPTOR_ALLOCATE_FLAG 0x1000
#define BL_MM_ADD_DESCRIPTOR_UPDATE_LIST_POINTER_FLAG 0x2000

#define BL_MM_INCLUDE_MAPPED_ALLOCATED 0x01
#define BL_MM_INCLUDE_MAPPED_UNALLOCATED 0x02
#define BL_MM_INCLUDE_UNMAPPED_ALLOCATED 0x04
#define BL_MM_INCLUDE_UNMAPPED_UNALLOCATED 0x08
#define BL_MM_INCLUDE_RESERVED_ALLOCATED 0x10
#define BL_MM_INCLUDE_BAD_MEMORY 0x20
#define BL_MM_INCLUDE_FIRMWARE_MEMORY 0x40
#define BL_MM_INCLUDE_TRUNCATED_MEMORY 0x80
#define BL_MM_INCLUDE_PERSISTENT_MEMORY 0x100
#define BL_MM_INCLUDE_FIRMWARE_MEMORY_2 0x200

#define BL_MM_INCLUDE_NO_FIRMWARE_MEMORY                                       \
  (BL_MM_INCLUDE_PERSISTENT_MEMORY | BL_MM_INCLUDE_TRUNCATED_MEMORY |          \
   BL_MM_INCLUDE_BAD_MEMORY | BL_MM_INCLUDE_RESERVED_ALLOCATED |               \
   BL_MM_INCLUDE_UNMAPPED_UNALLOCATED | BL_MM_INCLUDE_UNMAPPED_ALLOCATED |     \
   BL_MM_INCLUDE_MAPPED_UNALLOCATED | BL_MM_INCLUDE_MAPPED_ALLOCATED)

#define BL_MM_INCLUDE_ONLY_FIRMWARE_MEMORY                                     \
  (BL_MM_INCLUDE_FIRMWARE_MEMORY_2 | BL_MM_INCLUDE_FIRMWARE_MEMORY)

#define BL_MM_REQUEST_DEFAULT_TYPE 1
#define BL_MM_REQUEST_TOP_DOWN_TYPE 2

#define BL_MM_REMOVE_PHYSICAL_REGION_FLAG 0x40000000
#define BL_MM_REMOVE_VIRTUAL_REGION_FLAG 0x80000000

#define BL_LIBRARY_FLAG_NO_DISPLAY 0x01
#define BL_LIBRARY_FLAG_REINITIALIZE 0x02
#define BL_LIBRARY_FLAG_REINITIALIZE_ALL 0x04
#define BL_LIBRARY_FLAG_ZERO_HEAP_ALLOCATIONS_ON_FREE 0x10
#define BL_LIBRARY_FLAG_INITIALIZATION_COMPLETED 0x20
#define BL_LIBRARY_FLAG_NO_GRAPHICS_CONSOLE 0x800

#define BL_DISPLAY_GRAPHICS_FORCED_VIDEO_MODE_FLAG 0x01
#define BL_DISPLAY_GRAPHICS_FORCED_HIGH_RES_MODE_FLAG 0x02

#define BL_HT_VALUE_IS_INLINE 0x01

#define BL_FS_REGISTER_AT_HEAD_FLAG 1

#define BL_BLOCK_DEVICE_REMOVABLE_FLAG 0x01
#define BL_BLOCK_DEVICE_PRESENT_FLAG 0x02
#define BL_BLOCK_DEVICE_VIRTUAL_FLAG 0x04

#define BL_MEMORY_CLASS_SHIFT 28

#define BL_FILE_READ_ACCESS 0x01
#define BL_FILE_WRITE_ACCESS 0x02
#define BL_DIRECTORY_ACCESS 0x04
#define BL_UNKNOWN_ACCESS 0x10

#define BL_DEVICE_READ_ACCESS 0x01
#define BL_DEVICE_WRITE_ACCESS 0x02

#define BL_DEVICE_ENTRY_OPENED 0x01
#define BL_DEVICE_ENTRY_READ_ACCESS 0x02
#define BL_DEVICE_ENTRY_WRITE_ACCESS 0x04

#define BL_FILE_ENTRY_OPENED 0x01
#define BL_FILE_ENTRY_READ_ACCESS 0x02
#define BL_FILE_ENTRY_WRITE_ACCESS 0x04
#define BL_FILE_ENTRY_UNKNOWN_ACCESS 0x10
#define BL_FILE_ENTRY_DIRECTORY 0x10000

#define BL_ETFS_FILE_ENTRY_DIRECTORY 0x01

#define BL_IMG_VALID_FILE 0x01
#define BL_IMG_MEMORY_FILE 0x02
#define BL_IMG_REMOTE_FILE 0x04

#define BL_LOAD_IMG_VIRTUAL_BUFFER 0x01
#define BL_LOAD_IMG_EXISTING_BUFFER 0x04
#define BL_LOAD_IMG_UNKNOWN_BUFFER_FLAG 0x08
#define BL_LOAD_IMG_COMPUTE_SIGNATURE 0x10
#define BL_LOAD_IMG_COMPUTE_HASH 0x40000

#define BL_LOAD_PE_IMG_VIRTUAL_BUFFER BL_LOAD_IMG_VIRTUAL_BUFFER
#define BL_LOAD_PE_IMG_CHECK_MACHINE 0x02
#define BL_LOAD_PE_IMG_EXISTING_BUFFER BL_LOAD_IMG_EXISTING_BUFFER
#define BL_LOAD_PE_IMG_COMPUTE_HASH 0x10
#define BL_LOAD_PE_IMG_CHECK_SUBSYSTEM 0x80
#define BL_LOAD_PE_IMG_SKIP_RELOCATIONS 0x100
#define BL_LOAD_PE_IMG_CHECK_FORCED_INTEGRITY 0x200
#define BL_LOAD_PE_IMG_IGNORE_CHECKSUM_MISMATCH 0x10000
#define BL_LOAD_PE_IMG_VALIDATE_ORIGINAL_FILENAME 0x400000

#define BL_UTL_CHECKSUM_COMPLEMENT 0x10000
#define BL_UTL_CHECKSUM_ROTATE 0x20000
#define BL_UTL_CHECKSUM_NEGATE 0x40000
#define BL_UTL_CHECKSUM_UCHAR_BUFFER 0x01
#define BL_UTL_CHECKSUM_USHORT_BUFFER 0x02

/* ENUMERATIONS **************************************************************/

typedef enum _BL_COLOR {
  Black,
  Blue,
  Green,
  Cyan,
  Red,
  Magenta,
  Brown,
  LtGray,
  Gray,
  LtBlue,
  LtGreen,
  LtCyan,
  LtRed,
  LtMagenta,
  Yellow,
  White
} BL_COLOR,
    *PBL_COLOR;

typedef enum _BL_MENU_POLICY {
  MenuPolicyLegacy   = 0,
  MenuPolicyStandard = 1
} BL_MENU_POLICY;

typedef enum _BL_MEMORY_DESCRIPTOR_TYPE {
  BlMdPhysical,
  BlMdVirtual,
  BlMdTracker
} BL_MEMORY_DESCRIPTOR_TYPE;

typedef enum _BL_TRANSLATION_TYPE {
  BlNone,
  BlVirtual,
  BlPae,
  BlMax
} BL_TRANSLATION_TYPE;

typedef enum _BL_ARCH_MODE { BlProtectedMode, BlRealMode } BL_ARCH_MODE;

//
// Boot Device Types
//
typedef enum _BL_DEVICE_TYPE {
  DiskDevice            = 0,
  LegacyPartitionDevice = 2,
  SerialDevice          = 3,
  UdpDevice             = 4,
  BootDevice            = 5,
  PartitionDevice       = 6,
  LocateDevice          = 8,
} BL_DEVICE_TYPE;

//
// Local Device Types
//
typedef enum _BL_LOCAL_DEVICE_TYPE {
  LocalDevice       = 0,
  FloppyDevice      = 1,
  CdRomDevice       = 2,
  RamDiskDevice     = 3,
  FileDevice        = 5,
  VirtualDiskDevice = 6
} BL_LOCAL_DEVICE_TYPE;

//
// Partition types
//
typedef enum _BL_PARTITION_TYPE {
  GptPartition,
  MbrPartition,
  RawPartition,
} BL_PARTITION_TYPE;

//
// File Path Types
//
typedef enum _BL_PATH_TYPE { InternalPath = 3, EfiPath = 4 } BL_PATH_TYPE;

//
// Classes of Memory
//
typedef enum _BL_MEMORY_CLASS {
  BlLoaderClass = 0xD,
  BlApplicationClass,
  BlSystemClass
} BL_MEMORY_CLASS;

//
// Types of Memory
//
typedef enum _BL_MEMORY_TYPE {
  //
  // Loader Memory
  //
  BlLoaderMemory        = 0xD0000002,
  BlLoaderDeviceMemory  = 0xD0000004,
  BlLoaderHeap          = 0xD0000005,
  BlLoaderPageDirectory = 0xD0000006,
  BlLoaderReferencePage = 0xD0000007,
  BlLoaderRamDisk       = 0xD0000008,
  BlLoaderArchData      = 0xD0000009,
  BlLoaderData          = 0xD000000A,
  BlLoaderRegistry      = 0xD000000B,
  BlLoaderBlockMemory   = 0xD000000C,
  BlLoaderSelfMap       = 0xD000000F,

  //
  // Application Memory
  //
  BlApplicationReserved = 0xE0000001,
  BlApplicationData     = 0xE0000004,

  //
  // System Memory
  //
  BlConventionalMemory       = 0xF0000001,
  BlUnusableMemory           = 0xF0000002,
  BlReservedMemory           = 0xF0000003,
  BlEfiBootMemory            = 0xF0000004,
  BlConventionalZeroedMemory = 0xF000005,
  BlEfiRuntimeCodeMemory     = 0xF0000006,
  BlAcpiReclaimMemory        = 0xF0000008,
  BlAcpiNvsMemory            = 0xF0000009,
  BlDeviceIoMemory           = 0xF000000A,
  BlDevicePortMemory         = 0xF000000B,
  BlPalMemory                = 0xF000000C,
  BlEfiRuntimeDataMemory     = 0xF000000E,
} BL_MEMORY_TYPE;

typedef enum _BL_MEMORY_ATTR {
  //
  // Memory Caching Attributes
  //
  BlMemoryUncached             = 0x00000001,
  BlMemoryWriteCombined        = 0x00000002,
  BlMemoryWriteThrough         = 0x00000004,
  BlMemoryWriteBack            = 0x00000008,
  BlMemoryUncachedExported     = 0x00000010,
  BlMemoryValidCacheAttributes = BlMemoryUncached | BlMemoryWriteCombined |
                                 BlMemoryWriteThrough | BlMemoryWriteBack |
                                 BlMemoryUncachedExported,
  BlMemoryValidCacheAttributeMask = 0x000000FF,

  //
  // Memory Protection Attributes
  //
  BlMemoryWriteProtected   = 0x00000100,
  BlMemoryReadProtected    = 0x00000200,
  BlMemoryExecuteProtected = 0x00000400,
  BlMemoryValidProtectionAttributes =
      BlMemoryWriteProtected | BlMemoryReadProtected | BlMemoryExecuteProtected,
  BlMemoryValidProtectionAttributeMask = 0x0000FF00,

  //
  // Memory Allocation Attributes
  //
  BlMemoryLargePages                = 0x00010000,
  BlMemoryKernelRange               = 0x00020000,
  BlMemoryFixed                     = 0x00040000,
  BlMemoryBelow1MB                  = 0x00080000,
  BlMemoryValidAllocationAttributes = BlMemoryKernelRange | BlMemoryFixed |
                                      BlMemoryBelow1MB | BlMemoryLargePages,
  BlMemoryValidAllocationAttributeMask = 0x00FF0000,

  //
  // Memory Type Attributes
  //
  BlMemoryRuntime             = 0x01000000,
  BlMemoryCoalesced           = 0x02000000,
  BlMemoryUpdate              = 0x04000000,
  BlMemoryNonFirmware         = 0x08000000,
  BlMemoryPersistent          = 0x10000000,
  BlMemorySpecial             = 0x20000000,
  BlMemoryFirmware            = 0x80000000,
  BlMemoryValidTypeAttributes = BlMemoryRuntime | BlMemoryCoalesced |
                                BlMemoryUpdate | BlMemoryNonFirmware |
                                BlMemoryPersistent | BlMemorySpecial |
                                BlMemoryFirmware,
  BlMemoryValidTypeAttributeMask = 0xFF000000,
} BL_MEMORY_ATTR;

typedef struct _BL_LIBRARY_PARAMETERS {
  unsigned long LibraryFlags;
  unsigned long TranslationType;
  unsigned long MinimumAllocationCount;
  unsigned long MinimumHeapSize;
  unsigned long HeapAllocationAttributes;
  wchar_t *     ApplicationBaseDirectory;
  unsigned long DescriptorCount;
  wchar_t *     FontBaseDirectory;
} BL_LIBRARY_PARAMETERS, *PBL_LIBRARY_PARAMETERS;

/* This should eventually go into a more public header */
typedef struct _BOOT_APPLICATION_PARAMETER_BLOCK {
  /* This header tells the library what image we're dealing with */
  unsigned long Signature[2];
  unsigned long Version;
  unsigned long Size;
  unsigned long ImageType;
  unsigned long MemoryTranslationType;

  /* Where is the image located */
  unsigned __int64 ImageBase;
  unsigned long    ImageSize;

  /* Offset to BL_MEMORY_DATA */
  unsigned long MemoryDataOffset;

  /* Offset to BL_APPLICATION_ENTRY */
  unsigned long AppEntryOffset;

  /* Offset to BL_DEVICE_DESCRPIPTOR */
  unsigned long BootDeviceOffset;

  /* Offset to BL_FIRMWARE_PARAMETERS */
  unsigned long FirmwareParametersOffset;

  /* Offset to BL_RETURN_ARGUMENTS */
  unsigned long ReturnArgumentsOffset;
} BOOT_APPLICATION_PARAMETER_BLOCK, *PBOOT_APPLICATION_PARAMETER_BLOCK;

typedef struct _BL_MEMORY_DATA {
  unsigned long Version;
  unsigned long MdListOffset;
  unsigned long DescriptorCount;
  unsigned long DescriptorSize;
  unsigned long DescriptorOffset;
} BL_MEMORY_DATA, *PBL_MEMORY_DATA;

typedef struct _ARM_EXCEPTION_STATE {
  unsigned int Control;
  unsigned int Vbar;
  unsigned int Reserved;
  unsigned int Reserved2;
  unsigned int IdSvcRW;
} ARM_EXCEPTION_STATE, *PARM_EXCEPTION_STATE;

typedef struct _ARM_MM_STATE {
  char          MpExtensions;
  unsigned int  HardwarePageDirectory;
  unsigned int  TTB_Config;
  unsigned int  SoftwarePageDirectory;
  unsigned int *MappedHardwarePageDirectory;
} ARM_MM_STATE, *PARM_MM_STATE;

typedef struct _BL_FIRMWARE_DESCRIPTOR {
  unsigned long       Version;
  unsigned long       Unknown;
  EFI_HANDLE          ImageHandle;
  EFI_SYSTEM_TABLE *  SystemTable;
  ARM_EXCEPTION_STATE ExceptionState;
  ARM_MM_STATE        MmState;
  unsigned int        InterruptState;
} BL_FIRMWARE_DESCRIPTOR, *PBL_FIRMWARE_DESCRIPTOR;

typedef struct _BL_MEMORY_DESCRIPTOR {
  LIST_ENTRY ListEntry;
  union {
    struct {
      unsigned __int64 BasePage;
      unsigned __int64 VirtualPage;
    };
    struct {
      unsigned __int64 BaseAddress;
      unsigned __int64 VirtualAddress;
    };
  };
  unsigned __int64 PageCount;
  unsigned long    Flags;
  BL_MEMORY_TYPE   Type;
} BL_MEMORY_DESCRIPTOR, *PBL_MEMORY_DESCRIPTOR;

typedef struct _BL_BCD_OPTION {
  unsigned long Type;
  unsigned long DataOffset;
  unsigned long DataSize;
  unsigned long ListOffset;
  unsigned long NextEntryOffset;
  unsigned long Empty;
} BL_BCD_OPTION, *PBL_BCD_OPTION;

typedef struct _BL_APPLICATION_ENTRY {
  char          Signature[8];
  unsigned long Flags;
  EFI_GUID      EFI_GUID;
  unsigned long Unknown[4];
  BL_BCD_OPTION BcdData;
} BL_APPLICATION_ENTRY, *PBL_APPLICATION_ENTRY;

typedef struct _BL_LOADED_APPLICATION_ENTRY {
  unsigned long  Flags;
  EFI_GUID       EFI_GUID;
  PBL_BCD_OPTION BcdData;
} BL_LOADED_APPLICATION_ENTRY, *PBL_LOADED_APPLICATION_ENTRY;

typedef struct _BL_MENU_STATUS {
  union {
    struct {
      unsigned long AnyKey : 1;
      unsigned long AdvancedOptions : 1;
      unsigned long BootOptions : 1;
      unsigned long OemKey : 1;
      unsigned long Exit : 1;
      unsigned long Reserved : 27;
    };
    unsigned long AsULong;
  };
  unsigned long BootIndex;
  WCHAR         KeyValue;
} BL_MENU_STATUS, *PL_MENU_STATUS;

typedef enum _BL_BOOT_ERROR_STATUS {
  Reboot          = 1,
  Recover         = 2,
  RecoverOem      = 3,
  OsSelection     = 4,
  NextOs          = 5,
  TryAgain        = 6,
  AdvancedOptions = 7,
  BootOptions     = 8
} BL_BOOT_ERROR_STATUS;

typedef struct _BL_HARDDISK_DEVICE {
  unsigned long PartitionType;
  union {
    struct {
      unsigned long PartitionSignature;
    } Mbr;

    struct {
      EFI_GUID PartitionSignature;
    } Gpt;

    struct {
      unsigned long DiskNumber;
    } Raw;
  };
} BL_HARDDISK_DEVICE;

typedef struct _BL_ARCH_CONTEXT {
  BL_ARCH_MODE        Mode;
  BL_TRANSLATION_TYPE TranslationType;
  unsigned long       ContextFlags;
} BL_ARCH_CONTEXT, *PBL_ARCH_CONTEXT;

typedef struct _BL_MEMORY_DESCRIPTOR_LIST {
  LIST_ENTRY    ListHead;
  LIST_ENTRY *  First;
  LIST_ENTRY *  This;
  unsigned long Type;
} BL_MEMORY_DESCRIPTOR_LIST, *PBL_MEMORY_DESCRIPTOR_LIST;

typedef struct _BL_ADDRESS_RANGE {
  unsigned __int64 Minimum;
  unsigned __int64 Maximum;
} BL_ADDRESS_RANGE, *PBL_ADDRESS_RANGE;

typedef struct _BL_FILE_INFORMATION {
  unsigned __int64 Size;
  unsigned __int64 Offset;
  wchar_t *        FsName;
  unsigned long    Flags;
} BL_FILE_INFORMATION, *PBL_FILE_INFORMATION;

typedef struct _BL_BLOCK_DEVICE_INFORMATION {
  BL_LOCAL_DEVICE_TYPE Type;
  unsigned long        DeviceFlags;
  unsigned long        Unknown;
  BL_PARTITION_TYPE    PartitionType;
  unsigned long        BlockSize;
  unsigned long        Alignment;
  unsigned __int64     LastBlock;
  unsigned __int64     Offset;
  unsigned long        Block;
  struct {
    union {
      struct {
        unsigned long Signature;
      } Mbr;
      struct {
        EFI_GUID Signature;
      } Gpt;
    };
  } Disk;
} BL_BLOCK_DEVICE_INFORMATION, *PBL_BLOCK_DEVICE_INFORMATION;

typedef struct _BL_DEVICE_INFORMATION {
  BL_DEVICE_TYPE DeviceType;
  union {
    BL_BLOCK_DEVICE_INFORMATION BlockDeviceInfo;
  };
} BL_DEVICE_INFORMATION, *PBL_DEVICE_INFORMATION;

typedef struct _BL_BLOCK_DEVICE {
  BL_BLOCK_DEVICE_INFORMATION;
  unsigned __int64 StartOffset;
  EFI_BLOCK_IO *   Protocol;
  EFI_HANDLE       Handle;
} BL_BLOCK_DEVICE, *PBL_BLOCK_DEVICE;

typedef struct _BL_PROTOCOL_HANDLE {
  EFI_HANDLE Handle;
  void *     Interface;
} BL_PROTOCOL_HANDLE, *PBL_PROTOCOL_HANDLE;

typedef struct _BL_IMAGE_APPLICATION_ENTRY {
  PBL_APPLICATION_ENTRY AppEntry;
  void *                ImageBase;
  unsigned long         ImageSize;
} BL_IMAGE_APPLICATION_ENTRY, *PBL_IMAGE_APPLICATION_ENTRY;

typedef struct _BL_IMAGE_PARAMETERS {
  void *        Buffer;
  unsigned long ActualSize;
  unsigned long BufferSize;
} BL_IMAGE_PARAMETERS, *PBL_IMAGE_PARAMETERS;
