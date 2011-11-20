extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
}
#include <PortIO.h>
#include <Core/IDT.h>
#include <Hardware/PIC.h>
#include <Hardware/PIT.h>
#include <Hardware/VGAText.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

#define PIT_IO_BASE             0x40
#define PIT_IRQ_BASE            0x00

#define PIT_IO_DATA0            0x00
#define PIT_IO_DATA1            0x01
#define PIT_IO_DATA2            0x02
#define PIT_IO_COMMAND          0x03

#define PIT_IO_COMMAND_INIT     0x36

#define PIT_MAX_FREQUENCY       1193180


UINT32 gPITTimerTicks = 0;

static void PIT_InterruptHandler(IDT::Registers pRegisters)
{
    if(pRegisters.int_no) { }
    gPITTimerTicks++;
    if(gPITTimerTicks % 1000 == 0)
    {
	    //time_t t = time(NULL);
        //printf("PIT Tick: %d @ %s", gPITTimerTicks / 1000, ctime(&t));
    }
}

void PIT_Initialize(UINT32 pFrequency)
{
       // The value we send to the PIT is the value to divide it's input clock
       // (1193180 Hz) by, to get our required frequency. Important to note is
       // that the divisor must be small enough to fit into 16-bits.
       UINT32 divisor = PIT_MAX_FREQUENCY / pFrequency;

       // Send the command byte.
       outb(PIT_IO_BASE + PIT_IO_COMMAND, PIT_IO_COMMAND_INIT);

       // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
       UINT8 l = divisor & 0xFF;
       UINT8 h = (divisor >> 8) & 0xFF;

       // Send the frequency divisor.
       outb(PIT_IO_BASE + PIT_IO_DATA0, l);
       outb(PIT_IO_BASE + PIT_IO_DATA0, h);

       // Register our interrupt handler
       IDT::RegisterHandler(PIC_IRQ_MASTER_BASE + PIT_IRQ_BASE, &PIT_InterruptHandler);
}
