module Core.VGAText;

import Core.Debug;

class Console
{
private static:
    const(uint) BaseAddress = 0x000B8000;
    const(ubyte) Width = 80;
    const(ubyte) Height = 25;

    ubyte sCursorX;
    ubyte sCursorY;
    ubyte sAttributes;

    ubyte* Cursor()
    {
        return cast(ubyte*)(BaseAddress + (((sCursorY * Width) + sCursorX) * 2));
    }
    void Advance()
    {
        ++sCursorX;
        if (sCursorX >= Width) MoveToNextLine();
    }

public static:
    void Initialize()
    {
        sCursorX = 0;
        sCursorY = 0;
        Clear(CreateAttributes(ConsoleColor.LightWhite, ConsoleColor.DarkBlack));
    }

    ubyte CreateAttributes(ConsoleColor pForeground, ConsoleColor pBackground) { return cast(ubyte)(pForeground | (pBackground << 4)); }
    void Attributes(ubyte pAttributes) { sAttributes = pAttributes; }
    ubyte Attributes() { return sAttributes; }

    void MoveTo(ubyte pX, ubyte pY)
    {
        sCursorX = pX % Width;
        sCursorY = pY % Height;
    }
    void MoveToOrigin() { MoveTo(0, 0); }
    void MoveToNextLine() { MoveTo(0, cast(ubyte)(sCursorY + 1)); }

    void Clear(ubyte pAttributes)
    {
        Attributes = pAttributes;
        MoveToOrigin();

        ubyte* cursor = Cursor;
        for (int index = 0, count = Width * Height; index < count; ++index, cursor += 2)
        {
            *cursor = ' ';
            *(cursor + 1) = Attributes;
        }
    }

    void WriteCharacter(char pCharacter)
    {
        if (pCharacter == '\n')
        {
            MoveToNextLine();
            Debug.WriteCharacter('\r');
            Debug.WriteCharacter('\n');
        }
        else
        {
            ubyte* cursor = Cursor;
            *cursor = pCharacter;
            *(cursor + 1) = Attributes;
            Advance();
            Debug.WriteCharacter(pCharacter);
        }
    }
    void WriteString(const(char*) pString,
                     uint pLength)
    {
        bool terminated = pLength == 0;
        for (uint index = 0; pString[index]; ++index)
        {
            WriteCharacter(pString[index]);
            if (!terminated)
            {
                --pLength;
                if (!pLength) break;
            }
        }
    }
    void WriteLine(const(char*) pLine)
    {
        WriteString(pLine, 0);
        if (sCursorX) MoveToNextLine();
        Debug.WriteCharacter('\r');
        Debug.WriteCharacter('\n');
    }

}

enum ConsoleColor : ubyte
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
}
