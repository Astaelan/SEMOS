/ Special thanks to Sebastien De Greef for his help on this
.intel_syntax noprefix
.global IDT_Update
.global IDT_ISR_Dispatch
.global IDT_IRQ_Dispatch

.extern IDT_ISR_Handler
.extern IDT_IRQ_Handler

IDT_Update:
	cli
    mov eax, [esp]+4 
    lidt [eax]      
    ret 

IDT_ISR_Dispatch:
    pusha
    mov ax, ds
    push eax
 
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
    call IDT_ISR_Handler
 
	pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
	popa
    add esp, 8
    sti
    iret
 
IDT_IRQ_Dispatch:
    pusha
    mov ax, ds
    push eax
 
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
    call IDT_IRQ_Handler
 
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
 
    popa
    add esp, 8
    sti
    iret
