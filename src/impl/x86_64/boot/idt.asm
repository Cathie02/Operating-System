global idt_start
extern idt_flush

section .data
align 16

idt_pointer:
    dw 0                   ; Limit (will be set later)
    dq idt                 ; Base address of the IDT

section .text
bits 64

idt_start:
    ; Set up the IDT
    mov rax, idt_pointer
    lidt [rax]             ; Load the IDT descriptor

    ; Flush the IDT
    call idt_flush

    ; Halt the CPU
    hlt

section .data
align 16

idt:
    times 256 dq 0         ; Initialize IDT with 256 entries, each 8 bytes

idt_flush:
    ; Load the IDT
    mov rax, idt_pointer
    lidt [rax]             ; Load the IDT descriptor
    ret

section .bss
align 4096
stack_top:
    resb 8192              ; 8 KiB stack
stack_bottom:

;section .rodata
;gdt64:
;    dq 0
;.code_segment: equ $ - gdt64
