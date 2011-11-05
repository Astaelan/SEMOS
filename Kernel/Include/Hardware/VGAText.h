#ifndef _VGATEXT_H
#define _VGATEXT_H

#include <types.h>

#define VGATEXT_ATTRIBUTE_DARK_BLACK					0
#define VGATEXT_ATTRIBUTE_DARK_BLUE						1
#define VGATEXT_ATTRIBUTE_DARK_GREEN					2
#define VGATEXT_ATTRIBUTE_DARK_CYAN						3
#define VGATEXT_ATTRIBUTE_DARK_RED						4
#define VGATEXT_ATTRIBUTE_DARK_MAGENTA					5
#define VGATEXT_ATTRIBUTE_DARK_YELLOW					6
#define VGATEXT_ATTRIBUTE_DARK_WHITE					7
#define VGATEXT_ATTRIBUTE_LIGHT_BLACK					8
#define VGATEXT_ATTRIBUTE_LIGHT_BLUE					9
#define VGATEXT_ATTRIBUTE_LIGHT_GREEN					10
#define VGATEXT_ATTRIBUTE_LIGHT_CYAN					11
#define VGATEXT_ATTRIBUTE_LIGHT_RED						12
#define VGATEXT_ATTRIBUTE_LIGHT_MAGENTA					13
#define VGATEXT_ATTRIBUTE_LIGHT_YELLOW					14
#define VGATEXT_ATTRIBUTE_LIGHT_WHITE					15

#define VGATEXT_ATTRIBUTES(f, b)						(f | (b << 4))

void VGAText_SetAttributes(CHAR pAttributes);
void VGAText_WriteChar(CHAR pCharacter);
void VGAText_WriteString(const PSTRING pString, UINT32 pLength);
void VGAText_WriteLine(const PSTRING pString);
void VGAText_MoveTo(BYTE pColumn, BYTE pRow);
void VGAText_MoveToTopLeft();
void VGAText_MoveToNextLine();
void VGAText_Clear(CHAR pAttributes);

#endif