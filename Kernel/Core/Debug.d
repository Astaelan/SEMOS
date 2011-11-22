module Core.Debug;

import Core.PortIO;

class Debug
{
public static:
    void Initialize() { COMWriter.Initialize(); }
    void WriteCharacter(char pCharacter) { COMWriter.WriteByte(pCharacter); }
    void WriteLine(const(char*) pLine) { COMWriter.WriteLine(pLine); }
}

private class COMWriter
{
private static:
    const(uint) BaseAddress = 0x000002F8;
    const(uint) WriteAttempts = 1000;

    enum Register : ubyte
    {
        DataRegister = 0x00,
        InterruptRegister = 0x01,
        FIFORegister = 0x02,
        LineDataRegister = 0x03,
        ModemDataRegister = 0x04,
        LineStatusRegister = 0x05,
        ModemStatusRegister = 0x06,
        ScratchRegister = 0x07
    }

    bool TransmitEmpty() { return (inb(BaseAddress + Register.LineStatusRegister) & 0x20) != 0; }

public static:
    void Initialize()
    {
        outb(BaseAddress + Register.InterruptRegister, 0x00);
        outb(BaseAddress + Register.LineDataRegister, 0x80);
        outb(BaseAddress + Register.DataRegister, 0x03);
        outb(BaseAddress + Register.InterruptRegister, 0x00);
        outb(BaseAddress + Register.LineDataRegister, 0x03);
        outb(BaseAddress + Register.FIFORegister, 0xC7);
        outb(BaseAddress + Register.ModemDataRegister, 0x0B);
    }

    void WriteByte(ubyte pByte)
    {
        uint attempts = WriteAttempts;
        while (attempts && !TransmitEmpty) --attempts;
        outb(BaseAddress + Register.DataRegister, pByte);
    }

    void WriteString(const(char*) pString)
    {
        for (uint index = 0; pString[index]; ++index)
        {
            WriteByte(pString[index]);
        }
    }

    void WriteLine(const(char*) pLine)
    {
        WriteString(pLine);
        WriteByte(0x0D);
        WriteByte(0x0A);
    }
}