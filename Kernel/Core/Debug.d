module Core.Debug;

import Core.PortIO;

public static class Debug
{
    public static void Initialize() 
    {
        COMLogger.Initialize(); 
    }

    public static void WriteCharacter(char pCharacter)
    {
        COMLogger.WriteByte(pCharacter); 
    }

    public static void WriteString(string pString)
    {
        COMLogger.WriteString(pString);
    }

    public static void WriteLine(string pLine)
    {
        COMLogger.WriteLine(pLine); 
    }
}

private static class COMLogger
{
    private const uint MaxWriteWait = 1000;

    private const ushort IOPortBase =      0x2F8;
    private const ushort DataPort =        IOPortBase;
    private const ushort InterruptPort =   IOPortBase + 1;
    private const ushort FIFOPort =        IOPortBase + 2;
    private const ushort LineDataPort =    IOPortBase + 3;
    private const ushort ModemDataPort =   IOPortBase + 4;
    private const ushort LineStatusPort =  IOPortBase + 5;
    private const ushort ModemStatusPort = IOPortBase + 6;
    private const ushort ScratchPort =     IOPortBase + 7;

    private static bool IsTransmitEmpty() { return ((inb(LineStatusPort) & 0x20) != 0); }

    shared static void Initialize()
    {
        outb(InterruptPort, 0x00);
        outb(LineDataPort,  0x80);
        outb(DataPort,      0x03);
        outb(InterruptPort, 0x00);
        outb(LineDataPort,  0x03);
        outb(FIFOPort,      0xC7);
        outb(ModemDataPort, 0x0B);
    }

    public static void WriteByte(ubyte pByte)
    {
        uint wait = MaxWriteWait;
        while (wait && !IsTransmitEmpty) { --wait; }
        outb(DataPort, pByte);
    }

    public static void WriteString(string pString)
    {
        for (uint index = 0; index < pString.length; ++index)
        {
            WriteByte(pString[index]);
        }
    }

    public static void WriteLine(string pString)
    {
        WriteString(pString);
        WriteByte(cast(byte)'\r');
        WriteByte(cast(byte)'\n');
    }
}