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

#include <kport.h>


void kout( u16 port, u32 data, KDataSize size )
{
  if ( size == KBYTE )
    __asm__ volatile( "outb %%al,%%dx" ::"d"( port ), "a"( data ) );
  else if ( size == KWORD )
    __asm__ volatile( "outw %%ax,%%dx" ::"d"( port ), "a"( data ) );
  else if ( size == KDWORD )
    __asm__ volatile( "outl %%eax,%%dx" ::"d"( port ), "a"( data ) );
}

u32 kin( u16 port, KDataSize size )
{
  u8 db = 0;
  u16 dw = 0;
  u32 dl = 0;
  if ( size == KBYTE ) {
    __asm__ volatile( "inb %%dx, %%al" : "=a"( db ) : "d"( port ) );
    return db;
  } else if ( size == KWORD ) {
    __asm__ volatile( "inw %%dx, %%ax" : "=a"( dw ) : "d"( port ) );
    return dw;
  } else if ( size == KDWORD ) {
    __asm__ volatile( "inl %%dx, %%eax" : "=a"( dl ) : "d"( port ) );
    return dl;
  }
  return 0;
}
