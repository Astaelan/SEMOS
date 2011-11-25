#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class Console
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
            enum Color
            {
                DarkBlack = 0x00,
                DarkBlue = 0x01,
                DarkGreen = 0x02,
                DarkCyan = 0x03,
                DarkRed = 0x04,
                DarkMagenta = 0x05,
                DarkYellow = 0x06,
                DarkWhite = 0x07,
                LightBlack = 0x08,
                LightBlue = 0x09,
                LightGreen = 0x0A,
                LightCyan = 0x0B,
                LightRed = 0x0C,
                LightMagenta = 0x0D,
                LightYellow = 0x0E,
                LightWhite = 0x0F
            };

            static uint8_t CreateAttributes(Color pForeground, Color pBackground) { return pForeground | (pBackground << 4); }
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