// Armv7Lib.h: Supplmental ARMv7 operations

UINTN
EFIAPI
ArmReadTpidrprw(VOID);

VOID EFIAPI ArmWriteTpidrprw(UINTN Value);

VOID EFIAPI ArmInvalidateBtac(VOID);
