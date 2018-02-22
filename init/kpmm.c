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

#include <kio.h>
#include <kmultiboot.h>
#include <kpmm.h>

void kshow_mem_map()
{
  kprintf( "Memory map:\n" );
  KMMapEntry *entry = (KMMapEntry *)KERNEL_BOOT_INFO->mmap_addr,
             *end =
               &entry[KERNEL_BOOT_INFO->mmap_length / sizeof( KMMapEntry )];
  kprintf( "Index\tBase\t\t\t\tSize\t\t\t\tType\n" );
  for ( KMMapEntry *iter = entry; iter != end; ++iter ) {
    kprintf( "%u\t\t"
             "%#010x%08x\t"
             "%#010x%08x\t"
             "%x\n",
             iter - entry, iter->base_addr_high, iter->base_addr_low,
             iter->length_high, iter->length_low, iter->type );
  }
}

u32 kget_kernel_mem_used( KMemUnit unit )
{
  // Align to specified unit size
  u32 used = ( KERNEL_END - KERNEL_BEGIN + unit - 1 ) / unit;

  return used;
}
