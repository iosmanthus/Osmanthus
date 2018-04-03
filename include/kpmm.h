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

#ifndef _KPMM_H_
#define _KPMM_H_

#include <kmacros.h>
#include <ktypes.h>

#define KERNEL_BEGIN (__kernel_begin_addr)
#define KERNEL_END (__kernel_end_addr)
#define KERNEL_SIZE (KERNEL_END - KERNEL_BEGIN)

#define KPMM_MAX_SIZE 0x10000000 // Allow at most 256 MB physical memory
#define KPMM_PAGE_SIZE 0x1000    // 4KB pages

#define KPMM_PAGE_CNT (KPMM_MAX_SIZE / KPMM_PAGE_SIZE)
#define KPMM_SEG_AVAILABLE 1

#define KPAGE_ALIGNED(x) (KALIGN(x, KPMM_PAGE_SIZE))
#define KPAGE_MASK 0xfffff000

extern const u8 __kernel_begin_addr[];
extern const u8 __kernel_end_addr[];

void kshow_mem_map();
u32 kget_kernel_mem_used(KMemUnit unit);

void kinit_pmm();
u32 kphy_page_alloc();
u32 kget_phy_pages_avail();
void kphy_page_free(u32 page);

#endif /* ifndef _KPMM_H_ */
