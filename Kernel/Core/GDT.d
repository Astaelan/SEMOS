module Core.GDT;

extern (C) 
{
    private struct GDTRegister
    {
        align (2) ushort Limit;
        align (4) uint Address;
    }

    public struct GDTDescriptor
    {
        align (2) ushort LimitLow;
        align (2) ushort AddressLow;
        align (1) ubyte AddressMid;
        align (1) ubyte Access;
        align (1) ubyte LimitHighAndFlags;
        align (1) ubyte AddressHigh;
    }

    void GDTUpdate(GDTRegister* pRegister);

    GDTRegister sRegister;
    GDTDescriptor sDescriptors[GDT.MaxDescriptors];
}

public static class GDT
{
    public const ubyte MaxDescriptors = 5;

    public const ubyte AccessedAccess = 0x01;
    public const ubyte ReadWriteAccess = 0x02;
    public const ubyte DirectionConformingAccess = 0x04;
    public const ubyte ExecutableAccess = 0x08;
    public const ubyte OneAccess = 0x10;

    public const ubyte Ring0Access = 0x00;
    public const ubyte Ring1Access = 0x20;
    public const ubyte Ring2Access = 0x40;
    public const ubyte Ring3Access = 0x60;
    public const ubyte PresentAccess = 0x80;

    public const ubyte ReadWriteOnePresentAccess = ReadWriteAccess | OneAccess | PresentAccess;

    public const ubyte Selector32BitFlag = 0x40;
    public const ubyte Granularity4KBFlag = 0x80;

    public const ubyte Selector32BitGranularity4KBFlags = Selector32BitFlag | Granularity4KBFlag;

    private static void SetDescriptor(ubyte pIndex,
                                      uint pAddress,
                                      uint pLimit,
                                      ubyte pAccess,
                                      ubyte pFlags)
    {
        sDescriptors[pIndex].AddressLow = cast(ushort)(pAddress & 0xFFFF);
        sDescriptors[pIndex].AddressMid = cast(ubyte)((pAddress >> 16) & 0xFF);
        sDescriptors[pIndex].AddressHigh = cast(ubyte)((pAddress >> 24) & 0xFF);
        sDescriptors[pIndex].LimitLow = cast(ushort)(pLimit & 0xFFFF);
        sDescriptors[pIndex].LimitHighAndFlags = cast(ubyte)(((pLimit >> 16) & 0x0F) | (pFlags & 0xF0));
        sDescriptors[pIndex].Access = pAccess;
    }

    public static void Initialize()
    {
        sRegister.Limit = cast(ushort)((GDTDescriptor.sizeof * MaxDescriptors) - 1);
        sRegister.Address = cast(uint)(&sDescriptors[0]);

        SetDescriptor(0, 0x00000000, 0x00000000, 0x00, 0x00);
        SetDescriptor(1, 0x00000000, 0xFFFFFFFF, ReadWriteOnePresentAccess | ExecutableAccess, Selector32BitGranularity4KBFlags);
        SetDescriptor(2, 0x00000000, 0xFFFFFFFF, ReadWriteOnePresentAccess, Selector32BitGranularity4KBFlags);
        SetDescriptor(3, 0x00000000, 0xFFFFFFFF, ReadWriteOnePresentAccess | ExecutableAccess | Ring3Access, Selector32BitGranularity4KBFlags);
        SetDescriptor(4, 0x00000000, 0xFFFFFFFF, ReadWriteOnePresentAccess | Ring3Access, Selector32BitGranularity4KBFlags);

        GDTUpdate(&sRegister);
    }
}
