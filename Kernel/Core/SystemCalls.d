module Core.SystemCalls;

import Core.Console;
import Core.MultiBoot;

public void StopInterrupts() { asm { cli; } }
public void StartInterrupts() { asm { sti; } }

private void Halt() { asm { hlt; } }

public void Panic(const(char*) pMessage)
{
    StopInterrupts();
    Console.Clear(Console.CreateAttributes(ConsoleColor.DarkBlack, ConsoleColor.LightRed));
    Console.WriteLine(pMessage);
    while (true) Halt();
}

private extern (C) void* sbrk(int pAdjustment)
{
    static uint blockIndex;
    MultiBoot.Block* block = MultiBoot.GetBlock(blockIndex);

    if (pAdjustment >= 0)
    {
        uint originalIndex = blockIndex;
        while ((block.Length - block.Used) < pAdjustment &&
               blockIndex < MultiBoot.BlockCount)
        {
            ++blockIndex;
            block = MultiBoot.GetBlock(blockIndex);
        }
        if (blockIndex >= MultiBoot.BlockCount)
        {
            blockIndex = originalIndex;
            Panic("Insufficient memory");
        }
        block.Used += pAdjustment;
        return cast(void*)(block.Address + (block.Used - pAdjustment));
    }
    block.Used += pAdjustment;
    if (block.Used == 0 && blockIndex > 0) --blockIndex;
    return null;
}
