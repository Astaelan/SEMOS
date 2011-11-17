#include <Utility/COMPortLogger.h>

#include <PortIO.h>

void SEMOS::Utility::COMPortLogger::Initialize()
{
	outb(InterruptIOPort, 0x00);
	outb(LineDataIOPort, 0x80);
	outb(DataIOPort, 0x03);
	outb(InterruptIOPort, 0x00);
	outb(LineDataIOPort, 0x03);
	outb(FIFOIOPort, 0xC7);
	outb(ModemDataIOPort, 0x0B);
}

bool SEMOS::Utility::COMPortLogger::IsTransmitEmpty()
{
	return (inb(LineStatusIOPort) & 0x20) != 0;
}

void SEMOS::Utility::COMPortLogger::WriteByte(uint8_t pByte)
{
	uint32_t attempts = WriteAttempts;
	while (attempts && !IsTransmitEmpty()) --attempts;
	outb(DataIOPort, pByte);
}

void SEMOS::Utility::COMPortLogger::WriteString(const char * pString)
{
	const char * iterator = pString;
	while (*iterator)
	{
		WriteByte(*iterator);
		++iterator;
	}
}

void SEMOS::Utility::COMPortLogger::WriteLine(const char * pLine)
{
	WriteString(pLine);
	WriteByte(0x0D);
	WriteByte(0x0A);
}
