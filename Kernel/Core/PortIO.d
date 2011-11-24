module Core.PortIO;

public void outb(ushort pPort, ubyte pValue)
{
    asm
    {
        mov DX, pPort;
        mov AL, pValue;
        out DX, AL;
    }
}

public void outw(ushort pPort, ushort pValue)
{
    asm
    {
        mov DX, pPort;
        mov AX, pValue;
        out DX, AX;
    }
}

public void outl(ushort pPort, uint pValue)
{
    asm
    {
        mov DX, pPort;
        mov EAX, pValue;
        out DX, EAX;
    }
}

public ubyte inb(ushort pPort)
{
    ubyte result = 0;
    asm
    {
        mov DX, pPort;
        in AL, DX;
        mov result, AL;
    }
    return result;
}

public ushort inw(ushort pPort)
{
    ushort result = 0;
    asm
    {
        mov DX, pPort;
        in AX, DX;
        mov result, AX;
    }
    return result;
}

public uint inl(ushort pPort)
{
    uint result = 0;
    asm
    {
        mov DX, pPort;
        in EAX, DX;
        mov result, EAX;
    }
    return result;
}

public void iowait() { outb(0x80, 0x00); }
