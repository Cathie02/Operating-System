global start
extern long_mode_start

section .text
bits 32
start:
    mov esp, stack_top ; set the stack pointer
    
    call check_multiboot ; check if the bootloader is Multiboot-compliant
    call check_cpuid ; check if the CPU supports CPUID
    call check_long_mode ; check if the CPU supports long mode

    call setup_page_tables ; set up the page tables
    call enable_paging ; enable paging

    lgdt [gdt64.pointer] ; load the GDT
    jmp gdt64.code_segment:long_mode_start ; jump to the long mode start

    hlt

check_multiboot: ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Boot-information-format
    cmp eax, 0x36d76289 ; MULTIBOOT2_BOOTLOADER_MAGIC
    jne .not_multiboot
    ret

.not_multiboot:
    mov al, "M"
    jmp error

check_cpuid:
    pushfd ; save the EFLAGS register
    pop eax ; get the EFLAGS register
    mov ecx, eax ; save the EFLAGS register
    xor eax, 1 << 21 ; flip the ID bit
    push eax ; save the modified EFLAGS register
    popfd ; set the EFLAGS register to the modified value
    pushfd ; get the modified EFLAGS register
    pop eax ; get the modified EFLAGS register
    push ecx ; restore the original EFLAGS register
    popfd ; restore the original EFLAGS register
    cmp eax, ecx ; compare the original and modified EFLAGS registers
    je .no_cpuid ; if they are equal, the CPU does not support CPUID
    ret

.no_cpuid:
    mov al, "C"
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid ; get the highest supported CPUID function
    cmp eax, 0x80000001 ; check if the CPU supports the extended CPUID functions
    jb .no_long_mode ; if not, the CPU does not support long mode
    mov eax, 0x80000001
    cpuid ; get the extended feature flags
    test edx, 1 << 29 ; check if the long mode is supported
    jz .no_long_mode ; if not, the CPU does not support long mode
    ret

.no_long_mode:
    mov al, "L"
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11 ; present, writable flag
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 0b11 ; present, writable flag
    mov [page_table_l3], eax

    mov ecx, 0 ; counter
.loop:
    mov eax, 0x200000 ; 2 MiB page
    mul ecx
    or eax, 0b10000011 ; present, writable, huge page flag
    mov [page_table_l2 + ecx * 8], eax

    inc ecx ; increment the counter
    cmp ecx, 512 ; check if thewhole table is mapped
    jne .loop;
    ret

enable_paging:
    mov eax, page_table_l4
    mov cr3, eax ; set the page table base register
    mov eax, cr4
    or eax, 1 << 5 ; enable the physical address extension bit
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xc0000080 ; EFER MSR
    rdmsr ; read the EFER MSR
    or eax, 1 << 8 ; enable long mode
    wrmsr ; write the EFER MSR

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31 ; enable paging
    mov cr0, eax
    ret

error:
    ; print "ERROR: " in VGA text mode
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte [0xb800a], al
    hlt

section .bss
align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
; ignore page_table_l1 because we can set l2 huge page flag
;page_table_l1:
;    resb 4096

stack_bottom:
    resb 16384 ; 16 KiB stack
stack_top:

section .rodata
gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code descriptor
.pointer:
    dw $ - gdt64 - 1 ; size of the GDT - 1
    dq gdt64 ; address of the GDT