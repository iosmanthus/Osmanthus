
MULTIBOOT_HEADER_MAGIC  equ     0x1badb002
MULTIBOOT_PAGE_ALIGN    equ     1 << 0
MULTIBOOT_MEMORY_INFO   equ     1 << 1
MULTIBOOT_HEADER_FLAGS  equ     MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM      equ     -(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)
KERNEL_STACK_SIZE       equ     0x100000

[bits 32]
[global __start]
[extern kmain]

section .text

dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd MULTIBOOT_CHECKSUM

__start:
    cli
    mov esp, __kernel_stack_top
    and esp, 0x0fffffff0

    push ebp
    mov ebp, esp

    push ebx
    call kmain
__stop:
  hlt
  jmp __stop

section .bss
__kernel_stack_base:
    resb KERNEL_STACK_SIZE
__kernel_stack_top:
