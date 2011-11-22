module Core.MultiBoot;

import Core.SystemCalls;
import Core.VGAText;

extern (C)
{
    extern char KernelStart;
    extern char KernelEnd;
}

class MultiBoot
{
private static:
    const(uint) Magic = 0x2BADB002;
    const(uint) MemoryMapAvailable = 1;
    const(uint) MemoryMapConventional = (1024 * 640);
    const(uint) MaxBlocks = 64;

    Header* sHeader;
    Block sBlocks[MaxBlocks];
    uint sBlockCount;

    struct Header
    {
        uint Flags;
        uint MemoryLower;
        uint MemoryUpper;
        uint BootDevice;
        const(char*) CommandLine;
        uint ModuleCount;
        const(Module*) Modules;
        uint Table0;
        uint Table1;
        uint Table2;
        uint Table3;
        uint MemoryMapLength;
        const(MemoryMap*) MemoryMaps;
    }

    struct Module
    {
        uint Start;
        uint End;
        const(char*) Name;
        uint Reserved;
    }

    struct MemoryMap
    {
        uint Size;
        uint AddressLow;
        uint AddressHigh;
        uint LengthLow;
        uint LengthHigh;
        uint Type;
    }

public static:
    struct Block
    {
        uint Address;
        uint Length;
        uint Used;
        uint Reserved;
    }

    void Initialize(uint pMagic, void* pData)
    {
        if (pMagic != Magic) Panic("Boot loader did not provide multiboot data");
        sHeader = cast(Header*)pData;
        sBlockCount = 0;

        uint kernelStart = cast(uint)&KernelStart;
        uint kernelEnd = cast(uint)&KernelEnd;
        uint kernelSize = kernelEnd - kernelStart;

        uint memoryMapSize = sHeader.MemoryMaps[0].Size + 4;
        uint memoryMapCount = sHeader.MemoryMapLength / memoryMapSize;

        for (uint memoryMapIndex = 0;
             memoryMapIndex < memoryMapCount;
             ++memoryMapIndex)
        {
            const MemoryMap* memoryMap = &sHeader.MemoryMaps[memoryMapIndex];
            if (memoryMap.Type == MemoryMapAvailable)
            {
                uint address = memoryMap.AddressLow;
                uint length = memoryMap.LengthLow;
                if (address + length <= MemoryMapConventional) continue;
                if (kernelStart >= address && kernelEnd <= (address + length))
                {
                    address += kernelSize;
                    length -= kernelSize;
                }
                sBlocks[sBlockCount].Address = address;
                sBlocks[sBlockCount].Length = length;
                sBlocks[sBlockCount].Used = 0;
                sBlocks[sBlockCount].Reserved = 0;
                ++sBlockCount;
            }
        }
    }

    Block* GetBlock(uint pIndex)
    {
        if (pIndex >= sBlockCount) return null;
        return &sBlocks[pIndex];
    }

    uint BlockCount() { return sBlockCount; }
}