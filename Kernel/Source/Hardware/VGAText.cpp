extern "C" {
#include <stdio.h>
}
#include <Hardware/VGAText.h>
#include <Utility/COMPortLogger.h>

using namespace SEMOS::Hardware;
using namespace SEMOS::Utility;

uint8_t * VGAText::sBaseMemory = (uint8_t *)VGAText::BaseMemory;
uint8_t VGAText::sColumns = VGAText::DefaultColumns;
uint8_t VGAText::sRows = VGAText::DefaultRows;
uint8_t VGAText::sCursorColumn = 0;
uint8_t VGAText::sCursorRow = 0;
uint8_t VGAText::sAttributes = VGAText::DefaultAttributes;

void VGAText::Advance()
{
	++sCursorColumn;
	if (sCursorColumn >= sColumns)
	{
		MoveToNextLine();
		if (sCursorRow >= sRows) sCursorRow = 0;
	}
}

void VGAText::WriteCharacter(char pCharacter)
{
    if (pCharacter == '\n')
    {
        MoveToNextLine();
    	COMPortLogger::WriteByte('\r');
    	COMPortLogger::WriteByte('\n');
    }
    else
    {
	    uint8_t * cursor = GetCursor();
	    *cursor = pCharacter;
	    *(cursor + 1) = sAttributes;
	    Advance();
    	COMPortLogger::WriteByte(pCharacter);
    }
}

void VGAText::WriteString(const char * pString,
                          uint32_t pLength)
{
    const char * iterator = pString;
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

void VGAText::WriteLine(const char * pString)
{
	WriteString(pString, 0);
	if (sCursorColumn > 0) MoveToNextLine();
    COMPortLogger::WriteByte('\r');
    COMPortLogger::WriteByte('\n');
}

void VGAText::Clear(uint8_t pAttributes)
{
	SetAttributes(pAttributes);
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
