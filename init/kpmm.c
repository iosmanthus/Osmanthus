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

#include <kdebug.h>
#include <kio.h>
#include <kmultiboot.h>
#include <kpmm.h>

static u32 frame_stack[KPMM_PAGE_CNT];
static u32 frame_stack_top = KPMM_PAGE_CNT + 1;

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

void kinit_pmm()
{
  KMMapEntry *entry = (KMMapEntry *)KERNEL_BOOT_INFO->mmap_addr,
             *end =
               &entry[KERNEL_BOOT_INFO->mmap_length / sizeof( KMMapEntry )];
  for ( KMMapEntry *iter = entry; iter < end; ++iter ) {
    if ( iter->type == KPMM_SEG_AVAILABLE &&
         iter->base_addr_low == (u32)KERNEL_BEGIN ) {

      u32 seg_begin = iter->base_addr_low;
      u32 seg_end = PAGE_ALIGNED( seg_begin + KERNEL_SIZE );
      u32 map_end = seg_begin + iter->length_low;

      for ( u32 page_addr = seg_end;
            page_addr < KPMM_MAX_SIZE && page_addr < map_end;
            page_addr += KPMM_PAGE_SIZE ) {
        kphy_page_free( page_addr );
      }
    }
  }
}

u32 kget_kernel_mem_used( KMemUnit unit )
{
  // Align to specified unit size
  u32 used = ( KERNEL_SIZE + unit - 1 ) / unit;
  return used;
}

u32 kphy_page_alloc()
{
  kassert( frame_stack_top < KPMM_PAGE_CNT + 1 );
  return frame_stack[frame_stack_top++];
}

void kphy_page_free( u32 page )
{
  kassert( frame_stack_top > 0 );
  frame_stack[--frame_stack_top] = page;
}
