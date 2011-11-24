module Core.IDT;

import Core.PortIO;
import Hardware.PIC;

extern (C) void IDTUpdate(IDTRegister* pRegister);

private struct IDTRegister
{
    ushort Limit;
    uint Address;
}

private struct IDTRegisters
{
    uint ds;
    uint edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint int_no, err_code;
    uint eip, cs, eflags, useresp, ss;
};

extern (C) void IDTISRHandler(IDTRegisters pRegisters)
{
    IDT.Unschedule(cast(ushort)pRegisters.int_no);
    void function(IDTRegisters pRegisters) handler = IDT.GetHandler(cast(ushort)pRegisters.int_no);
    if (handler) handler(pRegisters);
}

extern (C) void IDTIRQHandler(IDTRegisters pRegisters)
{
    PIC.ResetIRQ(pRegisters.int_no > 8);
    IDT.Unschedule(cast(ushort)(IDT.BaseIRQInterrupt + pRegisters.int_no));
    void function(IDTRegisters pRegisters) handler = IDT.GetHandler(cast(ushort)(IDT.BaseIRQInterrupt + pRegisters.int_no));
    if (handler) handler(pRegisters);
}

public static class IDT
{
    public const ubyte BaseIRQInterrupt = 0x20;
    public const ushort MaxDescriptors = 256;

    public struct Descriptor
    {
        ushort AddressLow;
        ushort Selector;
        ubyte Zero;
        ubyte TypeAndFlags;
        ushort AddressHigh;
    }

    private enum Selector : ushort
    {
        PriviledgeLevelRing0Selector = 0x0000,
        PriviledgeLevelRing1Selector = 0x0001,
        PriviledgeLevelRing2Selector = 0x0002,
        PriviledgeLevelRing3Selector = 0x0003,
        TableIndexSelector = 0x0004,
        DescriptorIndexSelector = 0x0008
    };

    private enum Type : ubyte
    {
        Task386Gate32BitType = 0x05,
        Interrupt286Gate16BitType = 0x06,
        Trap286Gate16BitType = 0x07,
        Interrupt386Gate32BitType = 0x0E,
        Trap386Gate32BitType = 0x0F,
        PresentType = 0x80
    };

    private static IDTRegister sRegister;
    private static Descriptor sDescriptors[MaxDescriptors];
    private static bool sScheduled[MaxDescriptors];
    private static void function(IDTRegisters pRegisters) sHandlers[MaxDescriptors];

    private static void SetDescriptor(ushort pIndex,
                                      uint pAddress,
                                      Selector pSelector,
                                      Type pTypeAndFlags)
    {
        sDescriptors[pIndex].AddressLow = cast(ushort)(pAddress & 0xFFFF);
        sDescriptors[pIndex].AddressHigh = cast(ushort)((pAddress >> 16) & 0xFFFF);
        sDescriptors[pIndex].Selector = cast(ushort)pSelector;
        sDescriptors[pIndex].Zero = 0;
        sDescriptors[pIndex].TypeAndFlags = cast(ubyte)pTypeAndFlags;
    }
    private static void InitializeDescriptor(ushort pIndex,
                                             uint pAddress)
    {
        SetDescriptor(pIndex, pAddress, Selector.DescriptorIndexSelector, cast(Type)(Type.Interrupt386Gate32BitType | Type.PresentType));
    }

    public static void Schedule(ushort pInterrupt)
    {
        if (pInterrupt >= MaxDescriptors) return;
        sScheduled[pInterrupt] = true;
    }
    public static void Unschedule(ushort pInterrupt)
    {
        if (pInterrupt >= MaxDescriptors) return;
        sScheduled[pInterrupt] = false;
    }
    public static void WaitFor(ushort pInterrupt)
    {
        if (pInterrupt >= MaxDescriptors) return;
        while (sScheduled[pInterrupt]) { iowait(); }
    }
    public static void Register(ushort pInterrupt, void function(IDTRegisters pRegisters) pHandler)
    {
        if (pInterrupt >= MaxDescriptors) return;
        sHandlers[pInterrupt] = pHandler;
    }
    public static void function(IDTRegisters pRegisters) GetHandler(ushort pInterrupt)
    {
        if (pInterrupt >= MaxDescriptors) return null;
        return sHandlers[pInterrupt];
    }

    public static void Initialize()
    {
        sRegister.Limit = (Descriptor.sizeof * MaxDescriptors) - 1;
        sRegister.Address = cast(uint)(&sDescriptors[0]);

        // Reserved by CPU
        InitializeDescriptor(0x00, cast(uint)&IDTISR00);
        InitializeDescriptor(0x01, cast(uint)&IDTISR01);
        InitializeDescriptor(0x02, cast(uint)&IDTISR02);
        InitializeDescriptor(0x03, cast(uint)&IDTISR03);
        InitializeDescriptor(0x04, cast(uint)&IDTISR04);
        InitializeDescriptor(0x05, cast(uint)&IDTISR05);
        InitializeDescriptor(0x06, cast(uint)&IDTISR06);
        InitializeDescriptor(0x07, cast(uint)&IDTISR07);
        InitializeDescriptor(0x08, cast(uint)&IDTISR08);
        InitializeDescriptor(0x09, cast(uint)&IDTISR09);
        InitializeDescriptor(0x0A, cast(uint)&IDTISR0A);
        InitializeDescriptor(0x0B, cast(uint)&IDTISR0B);
        InitializeDescriptor(0x0C, cast(uint)&IDTISR0C);
        InitializeDescriptor(0x0D, cast(uint)&IDTISR0D);
        InitializeDescriptor(0x0E, cast(uint)&IDTISR0E);
        InitializeDescriptor(0x0F, cast(uint)&IDTISR0F);
        InitializeDescriptor(0x10, cast(uint)&IDTISR10);
        InitializeDescriptor(0x11, cast(uint)&IDTISR11);
        InitializeDescriptor(0x12, cast(uint)&IDTISR12);
        InitializeDescriptor(0x13, cast(uint)&IDTISR13);
        InitializeDescriptor(0x14, cast(uint)&IDTISR14);
        InitializeDescriptor(0x15, cast(uint)&IDTISR15);
        InitializeDescriptor(0x16, cast(uint)&IDTISR16);
        InitializeDescriptor(0x17, cast(uint)&IDTISR17);
        InitializeDescriptor(0x18, cast(uint)&IDTISR18);
        InitializeDescriptor(0x19, cast(uint)&IDTISR19);
        InitializeDescriptor(0x1A, cast(uint)&IDTISR1A);
        InitializeDescriptor(0x1B, cast(uint)&IDTISR1B);
        InitializeDescriptor(0x1C, cast(uint)&IDTISR1C);
        InitializeDescriptor(0x1D, cast(uint)&IDTISR1D);
        InitializeDescriptor(0x1E, cast(uint)&IDTISR1E);
        InitializeDescriptor(0x1F, cast(uint)&IDTISR1F);
        // Reserved for IRQ
        InitializeDescriptor(0x20, cast(uint)&IDTIRQ00);
        InitializeDescriptor(0x21, cast(uint)&IDTIRQ01);
        InitializeDescriptor(0x22, cast(uint)&IDTIRQ02);
        InitializeDescriptor(0x23, cast(uint)&IDTIRQ03);
        InitializeDescriptor(0x24, cast(uint)&IDTIRQ04);
        InitializeDescriptor(0x25, cast(uint)&IDTIRQ05);
        InitializeDescriptor(0x26, cast(uint)&IDTIRQ06);
        InitializeDescriptor(0x27, cast(uint)&IDTIRQ07);
        InitializeDescriptor(0x28, cast(uint)&IDTIRQ08);
        InitializeDescriptor(0x29, cast(uint)&IDTIRQ09);
        InitializeDescriptor(0x2A, cast(uint)&IDTIRQ0A);
        InitializeDescriptor(0x2B, cast(uint)&IDTIRQ0B);
        InitializeDescriptor(0x2C, cast(uint)&IDTIRQ0C);
        InitializeDescriptor(0x2D, cast(uint)&IDTIRQ0D);
        InitializeDescriptor(0x2E, cast(uint)&IDTIRQ0E);
        InitializeDescriptor(0x2F, cast(uint)&IDTIRQ0F);
        // Usable
        InitializeDescriptor(0x30, cast(uint)&IDTISR30);
        InitializeDescriptor(0x31, cast(uint)&IDTISR31);
        InitializeDescriptor(0x32, cast(uint)&IDTISR32);
        InitializeDescriptor(0x33, cast(uint)&IDTISR33);
        InitializeDescriptor(0x34, cast(uint)&IDTISR34);
        InitializeDescriptor(0x35, cast(uint)&IDTISR35);
        InitializeDescriptor(0x36, cast(uint)&IDTISR36);
        InitializeDescriptor(0x37, cast(uint)&IDTISR37);
        InitializeDescriptor(0x38, cast(uint)&IDTISR38);
        InitializeDescriptor(0x39, cast(uint)&IDTISR39);
        InitializeDescriptor(0x3A, cast(uint)&IDTISR3A);
        InitializeDescriptor(0x3B, cast(uint)&IDTISR3B);
        InitializeDescriptor(0x3C, cast(uint)&IDTISR3C);
        InitializeDescriptor(0x3D, cast(uint)&IDTISR3D);
        InitializeDescriptor(0x3E, cast(uint)&IDTISR3E);
        InitializeDescriptor(0x3F, cast(uint)&IDTISR3F);
        InitializeDescriptor(0x40, cast(uint)&IDTISR40);
        InitializeDescriptor(0x41, cast(uint)&IDTISR41);
        InitializeDescriptor(0x42, cast(uint)&IDTISR42);
        InitializeDescriptor(0x43, cast(uint)&IDTISR43);
        InitializeDescriptor(0x44, cast(uint)&IDTISR44);
        InitializeDescriptor(0x45, cast(uint)&IDTISR45);
        InitializeDescriptor(0x46, cast(uint)&IDTISR46);
        InitializeDescriptor(0x47, cast(uint)&IDTISR47);
        InitializeDescriptor(0x48, cast(uint)&IDTISR48);
        InitializeDescriptor(0x49, cast(uint)&IDTISR49);
        InitializeDescriptor(0x4A, cast(uint)&IDTISR4A);
        InitializeDescriptor(0x4B, cast(uint)&IDTISR4B);
        InitializeDescriptor(0x4C, cast(uint)&IDTISR4C);
        InitializeDescriptor(0x4D, cast(uint)&IDTISR4D);
        InitializeDescriptor(0x4E, cast(uint)&IDTISR4E);
        InitializeDescriptor(0x4F, cast(uint)&IDTISR4F);
        InitializeDescriptor(0x50, cast(uint)&IDTISR50);
        InitializeDescriptor(0x51, cast(uint)&IDTISR51);
        InitializeDescriptor(0x52, cast(uint)&IDTISR52);
        InitializeDescriptor(0x53, cast(uint)&IDTISR53);
        InitializeDescriptor(0x54, cast(uint)&IDTISR54);
        InitializeDescriptor(0x55, cast(uint)&IDTISR55);
        InitializeDescriptor(0x56, cast(uint)&IDTISR56);
        InitializeDescriptor(0x57, cast(uint)&IDTISR57);
        InitializeDescriptor(0x58, cast(uint)&IDTISR58);
        InitializeDescriptor(0x59, cast(uint)&IDTISR59);
        InitializeDescriptor(0x5A, cast(uint)&IDTISR5A);
        InitializeDescriptor(0x5B, cast(uint)&IDTISR5B);
        InitializeDescriptor(0x5C, cast(uint)&IDTISR5C);
        InitializeDescriptor(0x5D, cast(uint)&IDTISR5D);
        InitializeDescriptor(0x5E, cast(uint)&IDTISR5E);
        InitializeDescriptor(0x5F, cast(uint)&IDTISR5F);
        InitializeDescriptor(0x60, cast(uint)&IDTISR60);
        InitializeDescriptor(0x61, cast(uint)&IDTISR61);
        InitializeDescriptor(0x62, cast(uint)&IDTISR62);
        InitializeDescriptor(0x63, cast(uint)&IDTISR63);
        InitializeDescriptor(0x64, cast(uint)&IDTISR64);
        InitializeDescriptor(0x65, cast(uint)&IDTISR65);
        InitializeDescriptor(0x66, cast(uint)&IDTISR66);
        InitializeDescriptor(0x67, cast(uint)&IDTISR67);
        InitializeDescriptor(0x68, cast(uint)&IDTISR68);
        InitializeDescriptor(0x69, cast(uint)&IDTISR69);
        InitializeDescriptor(0x6A, cast(uint)&IDTISR6A);
        InitializeDescriptor(0x6B, cast(uint)&IDTISR6B);
        InitializeDescriptor(0x6C, cast(uint)&IDTISR6C);
        InitializeDescriptor(0x6D, cast(uint)&IDTISR6D);
        InitializeDescriptor(0x6E, cast(uint)&IDTISR6E);
        InitializeDescriptor(0x6F, cast(uint)&IDTISR6F);
        InitializeDescriptor(0x70, cast(uint)&IDTISR70);
        InitializeDescriptor(0x71, cast(uint)&IDTISR71);
        InitializeDescriptor(0x72, cast(uint)&IDTISR72);
        InitializeDescriptor(0x73, cast(uint)&IDTISR73);
        InitializeDescriptor(0x74, cast(uint)&IDTISR74);
        InitializeDescriptor(0x75, cast(uint)&IDTISR75);
        InitializeDescriptor(0x76, cast(uint)&IDTISR76);
        InitializeDescriptor(0x77, cast(uint)&IDTISR77);
        InitializeDescriptor(0x78, cast(uint)&IDTISR78);
        InitializeDescriptor(0x79, cast(uint)&IDTISR79);
        InitializeDescriptor(0x7A, cast(uint)&IDTISR7A);
        InitializeDescriptor(0x7B, cast(uint)&IDTISR7B);
        InitializeDescriptor(0x7C, cast(uint)&IDTISR7C);
        InitializeDescriptor(0x7D, cast(uint)&IDTISR7D);
        InitializeDescriptor(0x7E, cast(uint)&IDTISR7E);
        InitializeDescriptor(0x7F, cast(uint)&IDTISR7F);
        InitializeDescriptor(0x80, cast(uint)&IDTISR80);
        InitializeDescriptor(0x81, cast(uint)&IDTISR81);
        InitializeDescriptor(0x82, cast(uint)&IDTISR82);
        InitializeDescriptor(0x83, cast(uint)&IDTISR83);
        InitializeDescriptor(0x84, cast(uint)&IDTISR84);
        InitializeDescriptor(0x85, cast(uint)&IDTISR85);
        InitializeDescriptor(0x86, cast(uint)&IDTISR86);
        InitializeDescriptor(0x87, cast(uint)&IDTISR87);
        InitializeDescriptor(0x88, cast(uint)&IDTISR88);
        InitializeDescriptor(0x89, cast(uint)&IDTISR89);
        InitializeDescriptor(0x8A, cast(uint)&IDTISR8A);
        InitializeDescriptor(0x8B, cast(uint)&IDTISR8B);
        InitializeDescriptor(0x8C, cast(uint)&IDTISR8C);
        InitializeDescriptor(0x8D, cast(uint)&IDTISR8D);
        InitializeDescriptor(0x8E, cast(uint)&IDTISR8E);
        InitializeDescriptor(0x8F, cast(uint)&IDTISR8F);
        InitializeDescriptor(0x90, cast(uint)&IDTISR90);
        InitializeDescriptor(0x91, cast(uint)&IDTISR91);
        InitializeDescriptor(0x92, cast(uint)&IDTISR92);
        InitializeDescriptor(0x93, cast(uint)&IDTISR93);
        InitializeDescriptor(0x94, cast(uint)&IDTISR94);
        InitializeDescriptor(0x95, cast(uint)&IDTISR95);
        InitializeDescriptor(0x96, cast(uint)&IDTISR96);
        InitializeDescriptor(0x97, cast(uint)&IDTISR97);
        InitializeDescriptor(0x98, cast(uint)&IDTISR98);
        InitializeDescriptor(0x99, cast(uint)&IDTISR99);
        InitializeDescriptor(0x9A, cast(uint)&IDTISR9A);
        InitializeDescriptor(0x9B, cast(uint)&IDTISR9B);
        InitializeDescriptor(0x9C, cast(uint)&IDTISR9C);
        InitializeDescriptor(0x9D, cast(uint)&IDTISR9D);
        InitializeDescriptor(0x9E, cast(uint)&IDTISR9E);
        InitializeDescriptor(0x9F, cast(uint)&IDTISR9F);
        InitializeDescriptor(0xA0, cast(uint)&IDTISRA0);
        InitializeDescriptor(0xA1, cast(uint)&IDTISRA1);
        InitializeDescriptor(0xA2, cast(uint)&IDTISRA2);
        InitializeDescriptor(0xA3, cast(uint)&IDTISRA3);
        InitializeDescriptor(0xA4, cast(uint)&IDTISRA4);
        InitializeDescriptor(0xA5, cast(uint)&IDTISRA5);
        InitializeDescriptor(0xA6, cast(uint)&IDTISRA6);
        InitializeDescriptor(0xA7, cast(uint)&IDTISRA7);
        InitializeDescriptor(0xA8, cast(uint)&IDTISRA8);
        InitializeDescriptor(0xA9, cast(uint)&IDTISRA9);
        InitializeDescriptor(0xAA, cast(uint)&IDTISRAA);
        InitializeDescriptor(0xAB, cast(uint)&IDTISRAB);
        InitializeDescriptor(0xAC, cast(uint)&IDTISRAC);
        InitializeDescriptor(0xAD, cast(uint)&IDTISRAD);
        InitializeDescriptor(0xAE, cast(uint)&IDTISRAE);
        InitializeDescriptor(0xAF, cast(uint)&IDTISRAF);
        InitializeDescriptor(0xB0, cast(uint)&IDTISRB0);
        InitializeDescriptor(0xB1, cast(uint)&IDTISRB1);
        InitializeDescriptor(0xB2, cast(uint)&IDTISRB2);
        InitializeDescriptor(0xB3, cast(uint)&IDTISRB3);
        InitializeDescriptor(0xB4, cast(uint)&IDTISRB4);
        InitializeDescriptor(0xB5, cast(uint)&IDTISRB5);
        InitializeDescriptor(0xB6, cast(uint)&IDTISRB6);
        InitializeDescriptor(0xB7, cast(uint)&IDTISRB7);
        InitializeDescriptor(0xB8, cast(uint)&IDTISRB8);
        InitializeDescriptor(0xB9, cast(uint)&IDTISRB9);
        InitializeDescriptor(0xBA, cast(uint)&IDTISRBA);
        InitializeDescriptor(0xBB, cast(uint)&IDTISRBB);
        InitializeDescriptor(0xBC, cast(uint)&IDTISRBC);
        InitializeDescriptor(0xBD, cast(uint)&IDTISRBD);
        InitializeDescriptor(0xBE, cast(uint)&IDTISRBE);
        InitializeDescriptor(0xBF, cast(uint)&IDTISRBF);
        InitializeDescriptor(0xC0, cast(uint)&IDTISRC0);
        InitializeDescriptor(0xC1, cast(uint)&IDTISRC1);
        InitializeDescriptor(0xC2, cast(uint)&IDTISRC2);
        InitializeDescriptor(0xC3, cast(uint)&IDTISRC3);
        InitializeDescriptor(0xC4, cast(uint)&IDTISRC4);
        InitializeDescriptor(0xC5, cast(uint)&IDTISRC5);
        InitializeDescriptor(0xC6, cast(uint)&IDTISRC6);
        InitializeDescriptor(0xC7, cast(uint)&IDTISRC7);
        InitializeDescriptor(0xC8, cast(uint)&IDTISRC8);
        InitializeDescriptor(0xC9, cast(uint)&IDTISRC9);
        InitializeDescriptor(0xCA, cast(uint)&IDTISRCA);
        InitializeDescriptor(0xCB, cast(uint)&IDTISRCB);
        InitializeDescriptor(0xCC, cast(uint)&IDTISRCC);
        InitializeDescriptor(0xCD, cast(uint)&IDTISRCD);
        InitializeDescriptor(0xCE, cast(uint)&IDTISRCE);
        InitializeDescriptor(0xCF, cast(uint)&IDTISRCF);
        InitializeDescriptor(0xD0, cast(uint)&IDTISRD0);
        InitializeDescriptor(0xD1, cast(uint)&IDTISRD1);
        InitializeDescriptor(0xD2, cast(uint)&IDTISRD2);
        InitializeDescriptor(0xD3, cast(uint)&IDTISRD3);
        InitializeDescriptor(0xD4, cast(uint)&IDTISRD4);
        InitializeDescriptor(0xD5, cast(uint)&IDTISRD5);
        InitializeDescriptor(0xD6, cast(uint)&IDTISRD6);
        InitializeDescriptor(0xD7, cast(uint)&IDTISRD7);
        InitializeDescriptor(0xD8, cast(uint)&IDTISRD8);
        InitializeDescriptor(0xD9, cast(uint)&IDTISRD9);
        InitializeDescriptor(0xDA, cast(uint)&IDTISRDA);
        InitializeDescriptor(0xDB, cast(uint)&IDTISRDB);
        InitializeDescriptor(0xDC, cast(uint)&IDTISRDC);
        InitializeDescriptor(0xDD, cast(uint)&IDTISRDD);
        InitializeDescriptor(0xDE, cast(uint)&IDTISRDE);
        InitializeDescriptor(0xDF, cast(uint)&IDTISRDF);
        InitializeDescriptor(0xE0, cast(uint)&IDTISRE0);
        InitializeDescriptor(0xE1, cast(uint)&IDTISRE1);
        InitializeDescriptor(0xE2, cast(uint)&IDTISRE2);
        InitializeDescriptor(0xE3, cast(uint)&IDTISRE3);
        InitializeDescriptor(0xE4, cast(uint)&IDTISRE4);
        InitializeDescriptor(0xE5, cast(uint)&IDTISRE5);
        InitializeDescriptor(0xE6, cast(uint)&IDTISRE6);
        InitializeDescriptor(0xE7, cast(uint)&IDTISRE7);
        InitializeDescriptor(0xE8, cast(uint)&IDTISRE8);
        InitializeDescriptor(0xE9, cast(uint)&IDTISRE9);
        InitializeDescriptor(0xEA, cast(uint)&IDTISREA);
        InitializeDescriptor(0xEB, cast(uint)&IDTISREB);
        InitializeDescriptor(0xEC, cast(uint)&IDTISREC);
        InitializeDescriptor(0xED, cast(uint)&IDTISRED);
        InitializeDescriptor(0xEE, cast(uint)&IDTISREE);
        InitializeDescriptor(0xEF, cast(uint)&IDTISREF);
        InitializeDescriptor(0xF0, cast(uint)&IDTISRF0);
        InitializeDescriptor(0xF1, cast(uint)&IDTISRF1);
        InitializeDescriptor(0xF2, cast(uint)&IDTISRF2);
        InitializeDescriptor(0xF3, cast(uint)&IDTISRF3);
        InitializeDescriptor(0xF4, cast(uint)&IDTISRF4);
        InitializeDescriptor(0xF5, cast(uint)&IDTISRF5);
        InitializeDescriptor(0xF6, cast(uint)&IDTISRF6);
        InitializeDescriptor(0xF7, cast(uint)&IDTISRF7);
        InitializeDescriptor(0xF8, cast(uint)&IDTISRF8);
        InitializeDescriptor(0xF9, cast(uint)&IDTISRF9);
        InitializeDescriptor(0xFA, cast(uint)&IDTISRFA);
        InitializeDescriptor(0xFB, cast(uint)&IDTISRFB);
        InitializeDescriptor(0xFC, cast(uint)&IDTISRFC);
        InitializeDescriptor(0xFD, cast(uint)&IDTISRFD);
        InitializeDescriptor(0xFE, cast(uint)&IDTISRFE);
        InitializeDescriptor(0xFF, cast(uint)&IDTISRFF);

        IDTUpdate(&sRegister);
    }
}

extern (C)
{
    void IDTIRQ00();
    void IDTIRQ01();
    void IDTIRQ02();
    void IDTIRQ03();
    void IDTIRQ04();
    void IDTIRQ05();
    void IDTIRQ06();
    void IDTIRQ07();
    void IDTIRQ08();
    void IDTIRQ09();
    void IDTIRQ0A();
    void IDTIRQ0B();
    void IDTIRQ0C();
    void IDTIRQ0D();
    void IDTIRQ0E();
    void IDTIRQ0F();

    void IDTISR00();
    void IDTISR01();
    void IDTISR02();
    void IDTISR03();
    void IDTISR04();
    void IDTISR05();
    void IDTISR06();
    void IDTISR07();
    void IDTISR08();
    void IDTISR09();
    void IDTISR0A();
    void IDTISR0B();
    void IDTISR0C();
    void IDTISR0D();
    void IDTISR0E();
    void IDTISR0F();
    void IDTISR10();
    void IDTISR11();
    void IDTISR12();
    void IDTISR13();
    void IDTISR14();
    void IDTISR15();
    void IDTISR16();
    void IDTISR17();
    void IDTISR18();
    void IDTISR19();
    void IDTISR1A();
    void IDTISR1B();
    void IDTISR1C();
    void IDTISR1D();
    void IDTISR1E();
    void IDTISR1F();
/*
    void IDTISR20();
    void IDTISR21();
    void IDTISR22();
    void IDTISR23();
    void IDTISR24();
    void IDTISR25();
    void IDTISR26();
    void IDTISR27();
    void IDTISR28();
    void IDTISR29();
    void IDTISR2A();
    void IDTISR2B();
    void IDTISR2C();
    void IDTISR2D();
    void IDTISR2E();
    void IDTISR2F();
*/
    void IDTISR30();
    void IDTISR31();
    void IDTISR32();
    void IDTISR33();
    void IDTISR34();
    void IDTISR35();
    void IDTISR36();
    void IDTISR37();
    void IDTISR38();
    void IDTISR39();
    void IDTISR3A();
    void IDTISR3B();
    void IDTISR3C();
    void IDTISR3D();
    void IDTISR3E();
    void IDTISR3F();
    void IDTISR40();
    void IDTISR41();
    void IDTISR42();
    void IDTISR43();
    void IDTISR44();
    void IDTISR45();
    void IDTISR46();
    void IDTISR47();
    void IDTISR48();
    void IDTISR49();
    void IDTISR4A();
    void IDTISR4B();
    void IDTISR4C();
    void IDTISR4D();
    void IDTISR4E();
    void IDTISR4F();
    void IDTISR50();
    void IDTISR51();
    void IDTISR52();
    void IDTISR53();
    void IDTISR54();
    void IDTISR55();
    void IDTISR56();
    void IDTISR57();
    void IDTISR58();
    void IDTISR59();
    void IDTISR5A();
    void IDTISR5B();
    void IDTISR5C();
    void IDTISR5D();
    void IDTISR5E();
    void IDTISR5F();
    void IDTISR60();
    void IDTISR61();
    void IDTISR62();
    void IDTISR63();
    void IDTISR64();
    void IDTISR65();
    void IDTISR66();
    void IDTISR67();
    void IDTISR68();
    void IDTISR69();
    void IDTISR6A();
    void IDTISR6B();
    void IDTISR6C();
    void IDTISR6D();
    void IDTISR6E();
    void IDTISR6F();
    void IDTISR70();
    void IDTISR71();
    void IDTISR72();
    void IDTISR73();
    void IDTISR74();
    void IDTISR75();
    void IDTISR76();
    void IDTISR77();
    void IDTISR78();
    void IDTISR79();
    void IDTISR7A();
    void IDTISR7B();
    void IDTISR7C();
    void IDTISR7D();
    void IDTISR7E();
    void IDTISR7F();
    void IDTISR80();
    void IDTISR81();
    void IDTISR82();
    void IDTISR83();
    void IDTISR84();
    void IDTISR85();
    void IDTISR86();
    void IDTISR87();
    void IDTISR88();
    void IDTISR89();
    void IDTISR8A();
    void IDTISR8B();
    void IDTISR8C();
    void IDTISR8D();
    void IDTISR8E();
    void IDTISR8F();
    void IDTISR90();
    void IDTISR91();
    void IDTISR92();
    void IDTISR93();
    void IDTISR94();
    void IDTISR95();
    void IDTISR96();
    void IDTISR97();
    void IDTISR98();
    void IDTISR99();
    void IDTISR9A();
    void IDTISR9B();
    void IDTISR9C();
    void IDTISR9D();
    void IDTISR9E();
    void IDTISR9F();
    void IDTISRA0();
    void IDTISRA1();
    void IDTISRA2();
    void IDTISRA3();
    void IDTISRA4();
    void IDTISRA5();
    void IDTISRA6();
    void IDTISRA7();
    void IDTISRA8();
    void IDTISRA9();
    void IDTISRAA();
    void IDTISRAB();
    void IDTISRAC();
    void IDTISRAD();
    void IDTISRAE();
    void IDTISRAF();
    void IDTISRB0();
    void IDTISRB1();
    void IDTISRB2();
    void IDTISRB3();
    void IDTISRB4();
    void IDTISRB5();
    void IDTISRB6();
    void IDTISRB7();
    void IDTISRB8();
    void IDTISRB9();
    void IDTISRBA();
    void IDTISRBB();
    void IDTISRBC();
    void IDTISRBD();
    void IDTISRBE();
    void IDTISRBF();
    void IDTISRC0();
    void IDTISRC1();
    void IDTISRC2();
    void IDTISRC3();
    void IDTISRC4();
    void IDTISRC5();
    void IDTISRC6();
    void IDTISRC7();
    void IDTISRC8();
    void IDTISRC9();
    void IDTISRCA();
    void IDTISRCB();
    void IDTISRCC();
    void IDTISRCD();
    void IDTISRCE();
    void IDTISRCF();
    void IDTISRD0();
    void IDTISRD1();
    void IDTISRD2();
    void IDTISRD3();
    void IDTISRD4();
    void IDTISRD5();
    void IDTISRD6();
    void IDTISRD7();
    void IDTISRD8();
    void IDTISRD9();
    void IDTISRDA();
    void IDTISRDB();
    void IDTISRDC();
    void IDTISRDD();
    void IDTISRDE();
    void IDTISRDF();
    void IDTISRE0();
    void IDTISRE1();
    void IDTISRE2();
    void IDTISRE3();
    void IDTISRE4();
    void IDTISRE5();
    void IDTISRE6();
    void IDTISRE7();
    void IDTISRE8();
    void IDTISRE9();
    void IDTISREA();
    void IDTISREB();
    void IDTISREC();
    void IDTISRED();
    void IDTISREE();
    void IDTISREF();
    void IDTISRF0();
    void IDTISRF1();
    void IDTISRF2();
    void IDTISRF3();
    void IDTISRF4();
    void IDTISRF5();
    void IDTISRF6();
    void IDTISRF7();
    void IDTISRF8();
    void IDTISRF9();
    void IDTISRFA();
    void IDTISRFB();
    void IDTISRFC();
    void IDTISRFD();
    void IDTISRFE();
    void IDTISRFF();
}