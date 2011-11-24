module Core.MultiBoot;

import Core.Console;
import Core.SystemCalls;

extern (C)
{
    extern char KernelStart;
    extern char KernelEnd;
}

public static class MultiBoot
{
    private const uint Magic = 0x2BADB002;
    private const uint MemoryMapAvailable = 1;
    private const uint MemoryMapConventional = (1024 * 640);
    private const uint MaxBlocks = 64;

    private static Header* sHeader;
    private static Block sBlocks[MaxBlocks];
    private static uint sBlockCount;

    private struct Header
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

    private struct Module
    {
        uint Start;
        uint End;
        const(char*) Name;
        uint Reserved;
    }

    private struct MemoryMap
    {
        uint Size;
        uint AddressLow;
        uint AddressHigh;
        uint LengthLow;
        uint LengthHigh;
        uint Type;
    }

    public struct Block
    {
        uint Address;
        uint Length;
        uint Used;
        uint Reserved;
    }

    public static void Initialize(uint pMagic, void* pData)
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

    public static Block* GetBlock(uint pIndex)
    {
        if (pIndex >= sBlockCount) return null;
        return &sBlocks[pIndex];
    }

    public static uint BlockCount() { return sBlockCount; }
}