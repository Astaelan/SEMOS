#include <Hardware/PIC.h>
#include <Hardware/VGAText.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Hardware;

void PIC::Initialize()
{
	// ICW 1
	outb(MasterAddress + Register::CommandRegister, Command::ConfigModeCommand | Command::InitializeCommand); IOWAIT();
	outb(SlaveAddress + Register::CommandRegister, Command::ConfigModeCommand | Command::InitializeCommand); IOWAIT();

	// ICW 2
	outb(MasterAddress + Register::DataRegister, RemappedIRQOffset); IOWAIT();
	outb(SlaveAddress + Register::DataRegister, RemappedIRQOffset + 0x08); IOWAIT();

	// ICW 3
	outb(MasterAddress + Register::DataRegister, Line::MasterToSlaveLine); IOWAIT();
	outb(SlaveAddress + Register::DataRegister, Line::SlaveToMasterLine); IOWAIT();

	// ICW 4
	outb(MasterAddress + Register::DataRegister, Mode::Microprocessor8086Mode); IOWAIT();
	outb(SlaveAddress + Register::DataRegister, Mode::Microprocessor8086Mode); IOWAIT();

	outb(MasterAddress + Register::DataRegister, 0x00); IOWAIT();
	outb(SlaveAddress + Register::DataRegister, 0x00); IOWAIT();

	__asm volatile("sti");
}

void PIC::ResetIRQ(bool pSlave)
{
    if (pSlave) outb(SlaveAddress + Register::CommandRegister, Command::ResetCommand);
    outb(MasterAddress + Register::CommandRegister, Command::ResetCommand);
}