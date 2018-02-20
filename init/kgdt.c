/*
 * Copyright (c) 2018 Liming,Deng <myosmanthustree@gmail.com>
 * Author: Liming Deng
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <kgdt.h>
#include <kio.h>


static KSegmentDescritor gdt[GDT_ENTRY_CNT];

static void set_segment_descriptor( KSegmentDescritor *sd, u32 base, u32 limit,
                                    u8 access, u8 granularity );

void set_segment_descriptor( KSegmentDescritor *sd, u32 base, u32 limit,
                             u8 access, u8 granularity )
{
  sd->limit_low = limit & 0xffff;
  sd->base_low = base & 0xffff;
  sd->base_mid = ( base >> 16 ) & 0xff;
  sd->access = access;
  sd->granularity = ( granularity & 0xf0 ) | ( ( limit >> 16 ) & 0xf );
  sd->base_high = base >> 24;
}

KGDTPtr kinit_gdt()
{
  KGDTPtr gdt_ptr;
  gdt_ptr.addr = (u32)gdt;
  gdt_ptr.limit = sizeof( gdt ) - 1;

  // Null descritor
  set_segment_descriptor( &gdt[NULL_SEG_INDEX], 0x00000000, 0x00000000, 0x00,
                          0x00 );

  // Code segment descritor
  set_segment_descriptor( &gdt[CODE_SEG_INDEX], 0x00000000, 0xffffffff, 0x9a,
                          0xcf );

  // Data segment descritor
  set_segment_descriptor( &gdt[DATA_SEG_INDEX], 0x00000000, 0xffffffff, 0x92,
                          0xcf );

  // User code segment descritor
  set_segment_descriptor( &gdt[USER_CODE_SEG_INDEX], 0x00000000, 0xffffffff,
                          0xfa, 0xcf );

  // User data segment descritor
  set_segment_descriptor( &gdt[USER_DATA_SEG_INDEX], 0x00000000, 0xffffffff,
                          0xf2, 0xcf );

  return gdt_ptr;
}
