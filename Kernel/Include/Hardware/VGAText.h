#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class VGAText
        {
        private:
            static const uint32_t BaseMemory = 0x000B8000;
            static const uint8_t DefaultColumns = 80;
            static const uint8_t DefaultRows = 25;
            static const uint8_t DefaultAttributes = 0x0F;

            static uint8_t * sBaseMemory;
            static uint8_t sColumns;
            static uint8_t sRows;
            static uint8_t sCursorColumn;
            static uint8_t sCursorRow;
            static uint8_t sAttributes;

            static uint8_t * GetCursor() { return sBaseMemory + (((sCursorRow * sColumns) + sCursorColumn) * 2); }
            static void Advance();

        public:
            static const uint8_t DarkBlack = 0x00;
            static const uint8_t DarkBlue = 0x01;
            static const uint8_t DarkGreen = 0x02;
            static const uint8_t DarkCyan = 0x03;
            static const uint8_t DarkRed = 0x04;
            static const uint8_t DarkMagenta = 0x05;
            static const uint8_t DarkYellow = 0x06;
            static const uint8_t DarkWhite = 0x07;
            static const uint8_t LightBlack = 0x08;
            static const uint8_t LightBlue = 0x09;
            static const uint8_t LightGreen = 0x0A;
            static const uint8_t LightCyan = 0x0B;
            static const uint8_t LightRed = 0x0C;
            static const uint8_t LightMagenta = 0x0D;
            static const uint8_t LightYellow = 0x0E;
            static const uint8_t LightWhite = 0x0F;

            static uint8_t CreateAttributes(uint8_t pForeground, uint8_t pBackground) { return pForeground | (pBackground << 4); }
            static void SetAttributes(uint8_t pAttributes) { sAttributes = pAttributes; }
            static void WriteCharacter(char pCharacter);
            static void WriteString(const char * pString, uint32_t pLength);
            static void WriteLine(const char * pLine);
            static void MoveTo(uint8_t pColumn, uint8_t pRow) { sCursorColumn = pColumn % sColumns; sCursorRow = pRow % sRows; }
            static void MoveToTopLeft() { MoveTo(0, 0); }
            static void MoveToNextLine() { MoveTo(0, sCursorRow + 1); }
            static void Clear(uint8_t pAttributes);
        };
    }
}
