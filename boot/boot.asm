; Copyright (c) 2018 Liming,Deng <myosmanthustree@gmail.com>
; Author: Liming Deng
;
; Permission is hereby granted, free of charge, to any person obtaining a copy of
; this software and associated documentation files (the "Software"), to deal in
; the Software without restriction, including without limitation the rights to
; use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
; the Software, and to permit persons to whom the Software is furnished to do so,
; subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
; FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
; COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
; IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
; CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

MULTIBOOT_HEADER_MAGIC  equ     0x1badb002
MULTIBOOT_PAGE_ALIGN    equ     1 << 0
MULTIBOOT_MEMORY_INFO   equ     1 << 1
MULTIBOOT_HEADER_FLAGS  equ     MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM      equ     -(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)
KERNEL_STACK_SIZE       equ     0x400000

[bits 32]
[global __start:function]
[extern kmain]
[extern __kernel_multiboot_info]

section .text

dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd MULTIBOOT_CHECKSUM

__start:
    cli
    mov esp, __kernel_stack_top
    and esp, 0x0fffffff0

    mov ebp , 0x0
    mov [__kernel_multiboot_info], ebx
    call kmain
    jmp __stop
.end

size __start __start.end - __start

__stop:
  hlt
  jmp __stop


section .bss
__kernel_stack_base:
    resb KERNEL_STACK_SIZE
__kernel_stack_top:
