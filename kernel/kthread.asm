;; Copyright (c) 2018 Liming,Deng <myosmanthustree@gmail.com>
;; Author: Liming Deng

;; Permission is hereby granted, free of charge, to any person obtaining a copy
;; of this software and associated documentation files (the "Software"), to deal
;; in the Software without restriction, including without limitation the rights
;; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;; copies of the Software, and to permit persons to whom the Software is
;; furnished to do so, subject to the following conditions:

;; The above copyright notice and this permission notice shall be included in
;; all copies or substantial portions of the Software.

;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;; SOFTWARE.
[global kthread_context_switch:function]
[extern scheduler_unlock]
kthread_context_switch:
  mov eax, [esp+4]
  mov [eax+0],  esp
  mov [eax+4],  ebp
  mov [eax+8],  ebx
  mov [eax+12], esi
  mov [eax+16], edi
  pushf
  pop ecx
  mov [eax+20], ecx

  mov eax, [esp+8]
  mov esp, [eax+0]
  mov ebp, [eax+4]
  mov ebx, [eax+8]
  mov esi, [eax+12]
  mov edi, [eax+16]
  mov eax, [eax+20]

  push eax
  popf
  call scheduler_unlock
  ret
.end
size kthread_context_switch kthread_context_switch.end - kthread_context_switch
