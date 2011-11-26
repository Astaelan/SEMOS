extern "C" {
#include <stdio.h>
}
#include <Core/COMPortLogger.h>
#include <Core/Console.h>
#include <Core/DeviceManager.h>

using namespace SEMOS::Core;

uint8_t* Console::sBaseMemory = (uint8_t*)Console::BaseMemory;
uint8_t Console::sColumns = Console::DefaultColumns;
uint8_t Console::sRows = Console::DefaultRows;
uint8_t Console::sCursorColumn = 0;
uint8_t Console::sCursorRow = 0;
uint8_t Console::sAttributes = Console::DefaultAttributes;

uint8_t Console::CreateAttributes(Color pForeground, Color pBackground) { return pForeground | (pBackground << 4); }

void Console::MoveTo(uint8_t pColumn, uint8_t pRow)
{
    sCursorColumn = pColumn % sColumns;
    sCursorRow = pRow % sRows;
}

void Console::MoveToTopLeft() { MoveTo(0, 0); }
void Console::MoveToNextLine() { MoveTo(0, sCursorRow + 1); }

void Console::Clear(uint8_t pAttributes)
{
	sAttributes = pAttributes;
	MoveToTopLeft();
	
	int32_t index = 0;
	int32_t count = sColumns * sRows;
	uint8_t * cursor = GetCursor();
	while (index < count)
	{
		*cursor = ' ';
		*(cursor + 1) = sAttributes;
		cursor += 2;
		++index;
	}
}

void Console::WriteCharacter(char pCharacter)
{
    if (pCharacter == '\n')
    {
        MoveToNextLine();
    	DeviceManager::COMPortLoggersWriteByte('\r');
    	DeviceManager::COMPortLoggersWriteByte('\n');
    }
    else
    {
	    uint8_t* cursor = GetCursor();
	    *cursor = pCharacter;
	    *(cursor + 1) = sAttributes;
	    Advance();
    	DeviceManager::COMPortLoggersWriteByte(pCharacter);
    }
}

void Console::WriteString(const char* pString,
                          uint32_t pLength)
{
    const char* iterator = pString;
    bool useLength = pLength > 0;
    while (*iterator)
	{
        WriteCharacter(*iterator);
        ++iterator;
        if (useLength)
        {
            --pLength;
            if (pLength == 0) break;
        }
    }
}

void Console::WriteLine(const char* pString)
{
	WriteString(pString, 0);
	if (sCursorColumn > 0) MoveToNextLine();
    DeviceManager::COMPortLoggersWriteByte('\r');
    DeviceManager::COMPortLoggersWriteByte('\n');
}

uint8_t* Console::GetCursor() { return sBaseMemory + (((sCursorRow * sColumns) + sCursorColumn) * 2); }

void Console::Advance()
{
	++sCursorColumn;
	if (sCursorColumn >= sColumns)
	{
		MoveToNextLine();
		if (sCursorRow >= sRows) sCursorRow = 0;
	}
}
