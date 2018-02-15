[global kload_gdt:function]
kload_gdt:
  mov eax, [esp + 4]
  lgdt [eax]

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  jmp 0x08:__long_jmp
__long_jmp:
size kload_gdt __long_jmp - kload_gdt
