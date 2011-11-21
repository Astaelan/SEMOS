#include <Core/COMPortLogger.h>

#include <PortIO.h>

using namespace SEMOS::Core;

void COMPortLogger::Initialize()
{
	outb(BaseAddress + Register::InterruptRegister, 0x00);
	outb(BaseAddress + Register::LineDataRegister, 0x80);
	outb(BaseAddress + Register::DataRegister, 0x03);
	outb(BaseAddress + Register::InterruptRegister, 0x00);
	outb(BaseAddress + Register::LineDataRegister, 0x03);
	outb(BaseAddress + Register::FIFORegister, 0xC7);
	outb(BaseAddress + Register::ModemDataRegister, 0x0B);
}

bool COMPortLogger::IsTransmitEmpty()
{
	return (inb(BaseAddress + Register::LineStatusRegister) & 0x20) != 0;
}

void COMPortLogger::WriteByte(uint8_t pByte)
{
	uint32_t attempts = WriteAttempts;
	while (attempts && !IsTransmitEmpty()) --attempts;
	outb(BaseAddress + Register::DataRegister, pByte);
}

void COMPortLogger::WriteString(const char * pString)
{
	const char * iterator = pString;
	while (*iterator)
	{
		WriteByte(*iterator);
		++iterator;
	}
}

void COMPortLogger::WriteLine(const char * pLine)
{
	WriteString(pLine);
	WriteByte(0x0D);
	WriteByte(0x0A);
}
