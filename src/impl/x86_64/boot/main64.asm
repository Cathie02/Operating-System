global long_mode_start
extern kernel_main

section .text
bits 64

long_mode_start:
    ; Set up the stack pointer for 64-bit mode
    mov rax, stack_top
    mov rsp, rax

    ; load null into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_main ; call the kernel main function
    hlt

section .bss
align 4096
stack_top:
    resb 8192 ; 8 KiB stack
stack_bottom:
