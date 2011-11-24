module Hardware.PIC;

import Core.IDT;
import Core.PortIO;
import Core.SystemCalls;

public static class PIC
{
    private const ushort IOPortMasterBase =  0x0020;
    private const ushort IOPortSlaveBase =   0x00A0;
    private const ushort MasterCommandPort = IOPortMasterBase;
    private const ushort MasterDataPort =    IOPortMasterBase + 1;
    private const ushort SlaveCommandPort =  IOPortSlaveBase;
    private const ushort SlaveDataPort =     IOPortSlaveBase + 1;

    private enum Command : ubyte
    {
        ConfigModeCommand = 0x01,
        SinglePICCommand = 0x02,
        Interval4Command = 0x04,
        LevelModeCommand = 0x08,
        InitializeCommand = 0x10,
        ResetCommand = 0x20
    }

    private enum Line : ubyte
    {
        MasterToSlaveLine = 0x04,
        SlaveToMasterLine = 0x02
    }

    private enum Mode : ubyte
    {
        Microprocessor8086Mode = 0x01,
        AutoEndOfInterruptMode = 0x02,
        MasterMode = 0x04,
        BufferedMode = 0x08,
        SpecialFullyNestedMode = 0x10
    }

    public static void Initialize()
    {
        outb(MasterCommandPort, Command.ConfigModeCommand | Command.InitializeCommand); iowait();
        outb(SlaveCommandPort, Command.ConfigModeCommand | Command.InitializeCommand); iowait();

        outb(MasterDataPort, IDT.BaseIRQInterrupt); iowait();
        outb(SlaveDataPort, IDT.BaseIRQInterrupt + 0x08); iowait();

        outb(MasterDataPort, Line.MasterToSlaveLine); iowait();
        outb(SlaveDataPort, Line.SlaveToMasterLine); iowait();

        outb(MasterDataPort, Mode.Microprocessor8086Mode); iowait();
        outb(SlaveDataPort, Mode.Microprocessor8086Mode); iowait();

        outb(MasterDataPort, 0x00); iowait();
        outb(SlaveDataPort, 0x00); iowait();

        StartInterrupts();
    }

    public static void ResetIRQ(bool pSlave)
    {
        if (pSlave) { outb(SlaveCommandPort, Command.ResetCommand); }
        outb(MasterCommandPort, Command.ResetCommand);
    }
}
