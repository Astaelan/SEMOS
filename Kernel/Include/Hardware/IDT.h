#ifndef _IDT_H
#define _IDT_H

extern "C" {
#include <types.h>
}

typedef struct
{
   UINT32 ds;
   UINT32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
   UINT32 int_no, err_code;
   UINT32 eip, cs, eflags, useresp, ss;
} Registers;

typedef void (*IDTHandler)(Registers pRegisters);

void IDT_Initialize();
void IDT_ScheduleInterrupt(UINT8 pInterrupt);
void IDT_WaitForInterrupt(UINT8 pInterrupt);
void IDT_RegisterHandler(UINT8 pInterrupt, IDTHandler pHandler); 

#endif