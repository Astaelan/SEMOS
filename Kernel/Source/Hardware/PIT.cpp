#include <Core/IDT.h>
#include <Hardware/PIC.h>
#include <Hardware/PIT.h>
#include <Hardware/VGAText.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

extern "C" {
void PITInterruptHandler(IDT::Registers pRegisters);
}

void PITInterruptHandler(IDT::Registers pRegisters)
{
    if(pRegisters.int_no) { }
}

void PIT::Initialize(uint32_t pFrequency)
{
       // The value we send to the PIT is the value to divide it's input clock
       // (1193180 Hz) by, to get our required frequency. Important to note is
       // that the divisor must be small enough to fit into 16-bits.
       uint32_t divisor = MaxFrequency / pFrequency;

       // Send the command byte.
       outb(BaseAddress + Register::CommandRegister, Command::Counter0InitializeCommand);

       // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
       uint8_t low = divisor & 0xFF;
       uint8_t high = (divisor >> 8) & 0xFF;

       // Send the frequency divisor.
       outb(BaseAddress + Register::Counter0Register, low);
       outb(BaseAddress + Register::Counter0Register, high);

       // Register our interrupt handler
       IDT::RegisterHandler(PIC::RemappedIRQOffset + BaseIRQ, &PITInterruptHandler);
}
