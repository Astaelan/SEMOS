#include <Hardware/PIC.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Hardware;

void PIC::Initialize()
{
	// ICW 1
	outb(MasterCommandIOPort, Command::ConfigModeCommand | Command::InitializeCommand); IOWAIT();
	outb(SlaveCommandIOPort, Command::ConfigModeCommand | Command::InitializeCommand); IOWAIT();

	// ICW 2
	outb(MasterDataIOPort, RemappedIRQOffset); IOWAIT();
	outb(SlaveDataIOPort, RemappedIRQOffset + 0x08); IOWAIT();

	// ICW 3
	outb(MasterDataIOPort, Line::MasterToSlaveLine); IOWAIT();
	outb(SlaveDataIOPort, Line::SlaveToMasterLine); IOWAIT();

	// ICW 4
	outb(MasterDataIOPort, Mode::Microprocessor8086Mode); IOWAIT();
	outb(SlaveDataIOPort, Mode::Microprocessor8086Mode); IOWAIT();

	outb(MasterDataIOPort, 0x00); IOWAIT();
	outb(SlaveDataIOPort, 0x00); IOWAIT();

	__asm volatile("sti");
}

void PIC::ResetIRQ(bool pSlave)
{
    if (pSlave) outb(SlaveCommandIOPort, Command::ResetCommand);
    outb(MasterCommandIOPort, Command::ResetCommand);
}