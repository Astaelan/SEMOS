#ifndef _PIC_H
#define _PIC_H

#include <types.h>

#define PIC_IO_MASTER_BASE          0x20
#define PIC_IO_SLAVE_BASE           0xA0
#define PIC_IRQ_MASTER_BASE         0x20
#define PIC_IRQ_SLAVE_BASE          0x28

#define PIC_IO_COMMAND              0x00
#define PIC_IO_DATA                 0x01

#define PIC_IO_COMMAND_RESET        0x20

void PIC_Initialize();

#endif