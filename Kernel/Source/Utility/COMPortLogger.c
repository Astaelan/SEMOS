#include <Utility/COMPortLogger.h>
#include <PortIO.h>

#define COMPORTLOGGER_IO_BASE           0x2F8
#define COMPORTLOGGER_IO_DATA           (COMPORTLOGGER_IO_BASE + 0x00)
#define COMPORTLOGGER_IO_INTERRUPT      (COMPORTLOGGER_IO_BASE + 0x01)
#define COMPORTLOGGER_IO_FIFO           (COMPORTLOGGER_IO_BASE + 0x02)
#define COMPORTLOGGER_IO_LINEDATA       (COMPORTLOGGER_IO_BASE + 0x03)
#define COMPORTLOGGER_IO_MODEMDATA      (COMPORTLOGGER_IO_BASE + 0x04)
#define COMPORTLOGGER_IO_LINESTATUS     (COMPORTLOGGER_IO_BASE + 0x05)
#define COMPORTLOGGER_IO_MODEMSTATUS    (COMPORTLOGGER_IO_BASE + 0x06)
#define COMPORTLOGGER_IO_SCRATCH        (COMPORTLOGGER_IO_BASE + 0x07)


void COMPortLogger_Initialize()
{
	outb(COMPORTLOGGER_IO_INTERRUPT, 0x00);
	outb(COMPORTLOGGER_IO_LINEDATA, 0x80);
	outb(COMPORTLOGGER_IO_DATA, 0x03);
	outb(COMPORTLOGGER_IO_INTERRUPT, 0x00);
	outb(COMPORTLOGGER_IO_LINEDATA, 0x03);
	outb(COMPORTLOGGER_IO_FIFO, 0xC7);
	outb(COMPORTLOGGER_IO_MODEMDATA, 0x0B);
}

int COMPortLogger_IsTransmitEmpty()
{
	return inb(COMPORTLOGGER_IO_LINESTATUS) & 0x20;
}

void COMPortLogger_WriteData(BYTE b)
{
	// Empty loop, will allow us to timeout
	UINT32 i = 0;
	while (i < 1000 && (COMPortLogger_IsTransmitEmpty() == 0))
	{
		i++;
	}
	outb(COMPORTLOGGER_IO_DATA, b);
}

void COMPortLogger_WriteLine(const char * pString)
{
	COMPortLogger_WriteString(pString);
	// Now write the end line.
	COMPortLogger_WriteData((BYTE)'\r');
	COMPortLogger_WriteData((BYTE)'\n');
}

void COMPortLogger_WriteString(const char * pString)
{
	const char * iterator = pString;
	while (*iterator) 
	{
		COMPortLogger_WriteData(*iterator);
		++iterator;
	}
}