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
#include <kmain.h>
#include <kmultiboot.h>
#include <kstring.h>
#include <kvmm.h>

#define SECTION(x) __attribute__((section(#x)))
#define ALIGNED(x) __attribute__((aligned((x))))

ALIGNED(KPMM_PAGE_SIZE)
static u8 kstack_base[KERNEL_STACK_SIZE];

ALIGNED(KPMM_PAGE_SIZE)
static KPGD kernel_pgd[PGD_ENTRY_CNT];
ALIGNED(KPMM_PAGE_SIZE)
static KPGT kernel_pgt[PDE_NEED][PGT_ENTRY_CNT];

SECTION(.init.data)
static KPGD *ktmp_pgd = (KPGD *)0x1000;

SECTION(.init.data)
static KPGT *kpt_low = (KPGT *)0x2000;

SECTION(.init.data)
static KPGT *kpt_high = (KPGT *)0x3000;

SECTION(.init.text)
void kenable_paging(KMultiBoot *mb) {

  KMultiBoot **mb_load_addr =
      (KMultiBoot **)((u32)&KERNEL_BOOT_INFO - KERNEL_VM_OFFSET);
  *mb_load_addr = mb;
  *mb_load_addr = (KMultiBoot *)((u32)(*mb_load_addr) + KERNEL_VM_OFFSET);

  // Map VM 0 - 4MB to PM 0 - 4MB
  ktmp_pgd[0] = (u32)kpt_low | PAGE_WRITE | PAGE_PRESENT;
  for (u32 i = 0; i < PGD_ENTRY_CNT; ++i)
    kpt_low[i] = (i << 12) | PAGE_WRITE | PAGE_PRESENT;

  // map VM 0xc0000000 - 0xc0400000 to PM 0 - 4MB
  ktmp_pgd[PGD_INDEX(KERNEL_VM_OFFSET)] =
      (u32)kpt_high | PAGE_WRITE | PAGE_PRESENT;
  for (u32 i = 0; i < PGT_ENTRY_CNT; ++i)
    kpt_high[i] = (i << 12) | PAGE_WRITE | PAGE_PRESENT;

  __asm__ volatile("mov %0, %%cr3" : : "r"(ktmp_pgd));

  // Enable paging
  u32 cr0;
  __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));

  u32 kstack_p = (u32)(kstack_base + KERNEL_STACK_SIZE) & 0xfffffff0;
  __asm__ volatile("mov %0, %%esp\n\t"
                   "xor %%ebp, %%ebp"
                   :
                   : "r"(kstack_p));

  kmain();
}

void kinit_vmm() {
  u32 kernel_pgd_idx = PGD_INDEX(KERNEL_VM_OFFSET);
  for (u32 i = kernel_pgd_idx, j = 0; i < kernel_pgd_idx + PDE_NEED; ++i, ++j) {
    u32 pgt_phy_addr = (u32)&kernel_pgt[j] - KERNEL_VM_OFFSET;
    kernel_pgd[i] = pgt_phy_addr | PAGE_WRITE | PAGE_PRESENT;
  }

  KPGT *pte = (KPGT *)kernel_pgt;
  for (int i = 1; i < PDE_NEED * PGT_ENTRY_CNT; ++i)
    pte[i] = (i << 12) | PAGE_WRITE | PAGE_PRESENT;

  u32 kernel_pgd_phy_addr = (u32)kernel_pgd - KERNEL_VM_OFFSET;

  // Register an page fault handler
  kreg_int_handler(0xe, kpage_fault);
  kswitch_pgd(kernel_pgd_phy_addr);
}

void kswitch_pgd(u32 pgd) { __asm__ volatile("mov %0, %%cr3" : : "r"(pgd)); }

void kmap(KPGD *pgd_now, u32 va, u32 pa, u32 flags) {
  u32 pgd_idx = PGD_INDEX(va);
  u32 pt_idx = PGT_INDEX(va);

  KPGT *pte = (KPGT *)(pgd_now[pgd_idx] & KPAGE_MASK);
  if (!pte) {
    pte = (KPGT *)kphy_page_alloc();
    pgd_now[pgd_idx] = (u32)pte | PAGE_WRITE | PAGE_PRESENT;
    pte = (KPGT *)((u32)pte + KERNEL_VM_OFFSET);
    kbzero(pte, KPMM_PAGE_SIZE * sizeof(KPGT));
  } else
    pte = (KPGT *)((u32)pte + KERNEL_VM_OFFSET);

  pte[pt_idx] = (pa & 0xfffff000) | flags;
  __asm__ volatile("invlpg (%0)" : : "a"(va));
}

void kunmap(KPGD *pdg_now, u32 va) {
  u32 pgd_idx = PGD_INDEX(va);
  u32 pt_idx = PGT_INDEX(va);

  KPGT *pte = (KPGT *)(pdg_now[pgd_idx] & KPAGE_MASK);
  if (!pte)
    return;

  pte = (KPGT *)((u32)pte + KERNEL_VM_OFFSET);
  pte[pt_idx] = 0;
}

// Return the physical page addr that page 'va' maps to
u32 kget_mapping(KPGD *pdg_now, u32 va) {
  u32 pgd_idx = PGD_INDEX(va);
  u32 pt_idx = PGT_INDEX(va);

  KPGT *pte = (KPGT *)(pdg_now[pgd_idx] & KPAGE_MASK);
  if (!pte)
    return 0;

  pte = (KPGT *)((u32)pte + KERNEL_VM_OFFSET);
  return pte[pt_idx] & KPAGE_MASK;
}

void kpage_fault(KPTRegs *pt_regs) {
  u32 cr2 = 0;
  // Get reg cr2 value;
  __asm__ volatile("mov %%cr2,%0" : "=r"(cr2));

  kprintf("Page fault at %p, because access virtual page: %p\n", pt_regs->eip,
          cr2);
  u32 error_code = pt_regs->error_code;
  // Present?
  if (error_code & PAGE_PRESENT)
    kprintf("the fault was caused by a protection violation\n");
  else
    kprintf("it was caused by a non-present page\n");

  // Read/Write fault
  if (error_code & PAGE_WRITE)
    kprintf("Write fault\n");
  else
    kprintf("Read fault\n");

  // User/Supervisor mode?
  if (error_code & PAGE_USER)
    kprintf("In user mode\n");
  else
    kprintf("In kernel mode\n");

  if (error_code & 0x8)
    kprintf("Reserved bit overwritten\n");

  if (error_code & 0x10)
    kprintf("the fault was caused by an instruction fetch\n");

  while (1)
    ;
}

KPGD *kget_kernel_pgd() { return kernel_pgd; }
