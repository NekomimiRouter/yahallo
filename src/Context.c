#include <efi.h>
#include <efilib.h>

#include "ProcessorSupport.h"
#include "application.h"

// Switch to "real" mode and never look back.
// And we do not need boot application context in this application.
void SwitchToRealModeContext(PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{

  // We are currently in the APPLICATION context.
  // We need to switch to the Firmware context FIRST otherwise things blow up.
  unsigned int InterruptState = FirmwareDescriptor->InterruptState;

  // Disable IRQ
  ArmDisableInterrupts();

  // First up, switch MM state
  unsigned long Value = FirmwareDescriptor->MmState.HardwarePageDirectory |
                        FirmwareDescriptor->MmState.TTB_Config;

  ArmMoveToProcessor(Value, CP15_TTBR0);
  ArmInstructionSynchronizationBarrier();

  ArmMoveToProcessor(0, CP15_TLBIALL);
  ArmInvalidateBTAC();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();

  // Next up, set the exception state
  ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.IdSvcRW, CP15_TPIDRPRW);
  ArmDataSynchronizationBarrier();

  ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.Control, CP15_SCTLR);
  ArmInvalidateBTAC();
  ArmDataSynchronizationBarrier();
  ArmInstructionSynchronizationBarrier();

  ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.Vbar, CP15_VBAR);
  ArmInstructionSynchronizationBarrier();

  // Restore IRQ if necessary
  if (InterruptState)
    ArmEnableInterrupts();
}
