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

#ifndef _KVMM_H_
#define _KVMM_H_

#include <kisr.h>
#include <kpmm.h>
#include <ktypes.h>

#define KERNEL_STACK_SIZE 0x10000

#define KERNEL_VM_OFFSET 0xc0000000
#define PGD_ENTRY_CNT 0x400
#define PGT_ENTRY_CNT 0x400

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

#define PGD_INDEX(x) (((x) >> 22) & 0x3ff)
#define PGT_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_OFFSET(x) ((x)&0xfff)

#define PDE_NEED (KPMM_MAX_SIZE / (KPMM_PAGE_SIZE * PGT_ENTRY_CNT))

typedef u32 KPGD;
typedef u32 KPGT;

void kenable_paging();
void kpage_fault(KPTRegs *pt_regs);
void kswitch_pgd(u32 pgd);
void kinit_vmm();
void kmap(KPGD *pgd_now, u32 va, u32 pa, u32 flags);
void kunmap(KPGD *pdg_now, u32 va);
u32 kget_mapping(KPGD *pdg_now, u32 va);
KPGD *kget_kernel_pgd();
u8 *kget_kernel_stack_base();

#endif /* ifndef _KVMM_H_ */
