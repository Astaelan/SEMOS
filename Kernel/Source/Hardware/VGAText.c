#include <Hardware/VGAText.h>

#define VGATEXT_MEMORY_BASE             0x000B8000
#define VGATEXT_DEFAULT_COLUMNS                 80
#define VGATEXT_DEFAULT_ROWS                    25
#define VGATEXT_DEFAULT_ATTRIBUTES      0x0F

PSTRING gVGATextBase = (PSTRING)VGATEXT_MEMORY_BASE;
BYTE    gVGATextColumns = VGATEXT_DEFAULT_COLUMNS;
BYTE    gVGATextRows = VGATEXT_DEFAULT_ROWS;
BYTE    gVGATextCursorColumn = 0;
BYTE    gVGATextCursorRow = 0;
CHAR    gVGATextAttributes = VGATEXT_DEFAULT_ATTRIBUTES;


PSTRING VGAText_Cursor()
{ return gVGATextBase + (((gVGATextCursorRow * gVGATextColumns) + gVGATextCursorColumn) * 2); }

void VGAText_Next()
{
	++gVGATextCursorColumn;
	if (gVGATextCursorColumn >= gVGATextColumns)
	{
		VGAText_MoveToNextLine();
		if (gVGATextCursorRow >= gVGATextRows) gVGATextCursorRow = 0;
	}
}

void VGAText_SetAttributes(CHAR pAttributes)
{ gVGATextAttributes = pAttributes; }

void VGAText_WriteChar(CHAR pCharacter)
{
    if (pCharacter == '\n') VGAText_MoveToNextLine();
    else
    {
	    PSTRING cursor = VGAText_Cursor();
	    *cursor = pCharacter;
	    *(cursor + 1) = gVGATextAttributes;
	    VGAText_Next();
    }
}

void VGAText_WriteString(const PSTRING pString,
                         UINT32 pLength)
{
    PSTRING iterator = pString;
    BOOL useLength = pLength > 0;
    while (*iterator)
	{
        VGAText_WriteChar(*iterator);
        ++iterator;
        if (useLength)
        {
            --pLength;
            if (pLength == 0) break;
        }
    }
}

void VGAText_WriteLine(const PSTRING pString)
{
	VGAText_WriteString(pString, 0);
	if (gVGATextCursorColumn > 0) VGAText_MoveToNextLine();
}

void VGAText_MoveTo(BYTE pColumn,
                    BYTE pRow)
{
	gVGATextCursorColumn = pColumn % gVGATextColumns;
	gVGATextCursorRow = pRow % gVGATextRows;
}

void VGAText_MoveToTopLeft()
{
	VGAText_MoveTo(0, 0);
}

void VGAText_MoveToNextLine()
{
	VGAText_MoveTo(0, gVGATextCursorRow + 1);
}

void VGAText_Clear(CHAR pAttributes)
{
	VGAText_SetAttributes(pAttributes);
	VGAText_MoveToTopLeft();
	
	INT32 index = 0;
	INT32 count = gVGATextColumns * gVGATextRows;
	PSTRING cursor = VGAText_Cursor();
	while (index < count)
	{
		*cursor = ' ';
		*(cursor + 1) = gVGATextAttributes;
		cursor += 2;
		++index;
	}
}
