; Multiboot 2 Header
section .multiboot_header align=8

MULTIBOOT2_MAGIC_NUMBER    equ 0xe85250d6  ; Magic number to identify Multiboot 2
MULTIBOOT_ARCHITECTURE_I386 equ 0          ; i386 protected mode (32-bit)

header_start:
    dd MULTIBOOT2_MAGIC_NUMBER             ; Multiboot 2 magic number
    dd MULTIBOOT_ARCHITECTURE_I386         ; i386 architecture (protected mode)
    dd header_end - header_start           ; Header length (calculated dynamically)
    dd 0x100000000 - (MULTIBOOT2_MAGIC_NUMBER + MULTIBOOT_ARCHITECTURE_I386 + (header_end - header_start)) 
                                            ; Checksum - sum of first four fields must equal 0

    ; TODO: Framebuffer tag for graphical mode (currently displaying a black screen)
    ; dd 0x5                                 ; Tag type (5 = framebuffer)
    ; dd 20                                  ; Size of this tag (20 bytes)
    ; dw 0                                   ; Framebuffer type (0 = any)
    ; dw 1024                                ; Desired width (set to 1024 pixels)
    ; dw 768                                 ; Desired height (set to 768 pixels)
    ; db 32                                  ; Bits per pixel (32 bits = true color)
    ; db 0                                   ; Reserved, must be zero
    ; dw 0                                   ; Reserved, must be zero

    ; Padding to ensure total header size is a multiple of 8 bytes
    dw 0                                   ; Padding (2 bytes)
    dw 0                                   ; Padding (2 bytes)
    dd 8                                   ; Padding (4 bytes)

header_end:
