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
#include <kidt.h>
#include <kisr.h>
#include <kport.h>


#define ISR_INIT( id )                                                         \
  do {                                                                         \
    set_interrupt_gate( &idt[( id )], (u32)isr##id, GDT_CODE_SELECTOR, 0x8e ); \
  } while ( 0 );

#define IRQ_INIT( id )                                                         \
  do {                                                                         \
    set_interrupt_gate( &idt[( id + 32 )], (u32)irq##id, GDT_CODE_SELECTOR,    \
                        0x8e );                                                \
  } while ( 0 );


static KInterrputGate idt[IDT_ENTRY_CNT];
static int idt_init;

static void set_interrupt_gate( KInterrputGate *intgt, u32 base,
                                u16 segment_selector, u8 flags );

void set_interrupt_gate( KInterrputGate *intgt, u32 base, u16 segment_selector,
                         u8 flags )
{
  intgt->base_low = base & 0xffff;
  intgt->segment_selector = segment_selector;
  intgt->reserve = 0x0;
  intgt->flags = flags; // 10001110b
  intgt->base_high = ( base >> 16 ) & 0xffff;
}

KIDTPtr kget_idt()
{
  static KIDTPtr idt_ptr;
  if ( !idt_init ) {
    idt_ptr.base = (u32)&idt;
    idt_ptr.limit = sizeof( idt ) - 1;
    idt_init = 1;

    // Remap the irq table.
    kout( 0x20, 0x11, KBYTE );
    kout( 0xA0, 0x11, KBYTE );
    kout( 0x21, 0x20, KBYTE );
    kout( 0xA1, 0x28, KBYTE );
    kout( 0x21, 0x04, KBYTE );
    kout( 0xA1, 0x02, KBYTE );
    kout( 0x21, 0x01, KBYTE );
    kout( 0xA1, 0x01, KBYTE );
    kout( 0x21, 0x0, KBYTE );
    kout( 0xA1, 0x0, KBYTE );

    ISR_INIT( 0 );
    ISR_INIT( 1 );
    ISR_INIT( 2 );
    ISR_INIT( 3 );
    ISR_INIT( 4 );
    ISR_INIT( 5 );
    ISR_INIT( 6 );
    ISR_INIT( 7 );
    ISR_INIT( 8 );
    ISR_INIT( 9 );
    ISR_INIT( 10 );
    ISR_INIT( 11 );
    ISR_INIT( 12 );
    ISR_INIT( 13 );
    ISR_INIT( 14 );
    ISR_INIT( 15 );
    ISR_INIT( 16 );
    ISR_INIT( 17 );
    ISR_INIT( 18 );
    ISR_INIT( 19 );
    ISR_INIT( 20 );
    ISR_INIT( 21 );
    ISR_INIT( 22 );
    ISR_INIT( 23 );
    ISR_INIT( 24 );
    ISR_INIT( 25 );
    ISR_INIT( 26 );
    ISR_INIT( 27 );
    ISR_INIT( 28 );
    ISR_INIT( 29 );
    ISR_INIT( 30 );
    ISR_INIT( 31 );

    IRQ_INIT( 0 );
    IRQ_INIT( 1 );
    IRQ_INIT( 2 );
    IRQ_INIT( 3 );
    IRQ_INIT( 4 );
    IRQ_INIT( 5 );
    IRQ_INIT( 6 );
    IRQ_INIT( 7 );
    IRQ_INIT( 8 );
    IRQ_INIT( 9 );
    IRQ_INIT( 10 );
    IRQ_INIT( 11 );
    IRQ_INIT( 12 );
    IRQ_INIT( 13 );
    IRQ_INIT( 14 );
    IRQ_INIT( 15 );


    // System call
    ISR_INIT( 255 );
  }
  return idt_ptr;
}
