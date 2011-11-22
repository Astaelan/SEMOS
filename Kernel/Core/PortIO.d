module Core.PortIO;

void outb(ushort pPort, ubyte pValue)
{
    asm
    {
        mov DX, pPort;
        mov AL, pValue;
        out DX, AL;
    }
}

void outw(ushort pPort, ushort pValue)
{
    asm
    {
        mov DX, pPort;
        mov AX, pValue;
        out DX, AX;
    }
}

void outl(ushort pPort, uint pValue)
{
    asm
    {
        mov DX, pPort;
        mov EAX, pValue;
        out DX, EAX;
    }
}

ubyte inb(ushort pPort)
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

ushort inw(ushort pPort)
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

uint inl(ushort pPort)
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
