ENTRY(__start)
SECTIONS
{
  . = 0x100000;
  PROVIDE(__kernel_begin_addr = . );
  .text :
  {
    *(.text)
    . = ALIGN(4096);
}

.rodata :
{
  *(.rodata)
  . = ALIGN(4096);
}

.data :
{
  *(.data)
  . = ALIGN(4096);
}

.bss :
{
  *(.bss)
  . = ALIGN(4096);
}

.stab :
{
  *(.stab)
  . = ALIGN(4096);
}

.stabstr :
{
  *(.stabstr)
  . = ALIGN(4096);
}
PROVIDE(__kernel_end_addr = . );
/DISCARD/ : { *(.comment) *(.eh_frame) }
}
