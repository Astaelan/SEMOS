extern "C" {
#include <stdio.h>
}
#include <Hardware/VGAText.h>
#include <Utility/COMPortLogger.h>

uint8_t * SEMOS::Hardware::VGAText::sBaseMemory = (uint8_t *)SEMOS::Hardware::VGAText::BaseMemory;
uint8_t SEMOS::Hardware::VGAText::sColumns = SEMOS::Hardware::VGAText::DefaultColumns;
uint8_t SEMOS::Hardware::VGAText::sRows = SEMOS::Hardware::VGAText::DefaultRows;
uint8_t SEMOS::Hardware::VGAText::sCursorColumn = 0;
uint8_t SEMOS::Hardware::VGAText::sCursorRow = 0;
uint8_t SEMOS::Hardware::VGAText::sAttributes = SEMOS::Hardware::VGAText::DefaultAttributes;

void SEMOS::Hardware::VGAText::Advance()
{
	++sCursorColumn;
	if (sCursorColumn >= sColumns)
	{
		MoveToNextLine();
		if (sCursorRow >= sRows) sCursorRow = 0;
	}
}

void SEMOS::Hardware::VGAText::WriteCharacter(char pCharacter)
{
    if (pCharacter == '\n')
    {
        MoveToNextLine();
    	SEMOS::Utility::COMPortLogger::WriteByte('\r');
    	SEMOS::Utility::COMPortLogger::WriteByte('\n');
    }
    else
    {
	    uint8_t * cursor = GetCursor();
	    *cursor = pCharacter;
	    *(cursor + 1) = sAttributes;
	    Advance();
    	SEMOS::Utility::COMPortLogger::WriteByte(pCharacter);
    }
}

void SEMOS::Hardware::VGAText::WriteString(const char * pString,
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

void SEMOS::Hardware::VGAText::WriteLine(const char * pString)
{
	WriteString(pString, 0);
	if (sCursorColumn > 0) MoveToNextLine();
    SEMOS::Utility::COMPortLogger::WriteByte('\r');
    SEMOS::Utility::COMPortLogger::WriteByte('\n');
}

void SEMOS::Hardware::VGAText::Clear(uint8_t pAttributes)
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
