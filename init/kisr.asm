;Copyright (c) 2018 Liming,Deng <myosmanthustree@gmail.com>
;Author: Liming Deng
;
;Permission is hereby granted, free of charge, to any person obtaining a copy of
;this software and associated documentation files (the "Software"), to deal in
;the Software without restriction, including without limitation the rights to
;use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
;the Software, and to permit persons to whom the Software is furnished to do so,
;subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
;FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
;COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
;IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
;CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


%macro ISR_ERRCODE 1
[global isr%1:function]
isr%1:
  cli
  push dword %1 ; Interrupt id
  jmp kisr_common_stub
.end
size isr%1 isr%1.end - isr%1
%endmacro

%macro ISR_NOERRCODE 1
[global isr%1:function]
isr%1:
  cli
  push dword 0  ; Dummy error code
  push dword %1
  jmp kisr_common_stub
.end
size isr%1 isr%1.end - isr%1
%endmacro

%macro IRQ 2
[global irq%1:function]
irq%1:
  cli
  push dword 0 ; dummy error code
  push dword %2 ; push interrupt id
  jmp kirq_common_stub
.end
size irq%1 irq%1.end - irq%1
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7

ISR_ERRCODE 8

ISR_NOERRCODE 9

ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14

ISR_NOERRCODE 15
ISR_NOERRCODE 16

ISR_ERRCODE 17

ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; System call
ISR_NOERRCODE 255

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

[extern kisr_handler]
kisr_common_stub:
  pusha

  mov ax, ds

  push eax

  mov ax, 0x10   ; Kernel data segment selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  push esp
  call kisr_handler
  add esp, 4

  pop ebx
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx
  mov ss, bx


  popa
  add esp, 8
  iret

[extern kirq_handler]
kirq_common_stub:
  pusha

  mov ax, ds

  push eax

  mov ax, 0x10   ; Kernel data segment selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  push esp
  call kirq_handler
  add esp, 4

  pop ebx
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx
  mov ss, bx


  popa
  add esp, 8
  iret
