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

#define PGD_INDEX( x ) ( ( ( x ) >> 22 ) & 0x3ff )
#define PGT_INDEX( x ) ( ( ( x ) >> 12 ) & 0x3ff )
#define PAGE_OFFSET( x ) ( (x)&0xfff )

#define PDE_NEED ( KPMM_MAX_SIZE / ( KPMM_PAGE_SIZE * PGT_ENTRY_CNT ) )

typedef u32 KPGD;
typedef u32 KPGT;

void kenable_paging();
void kpage_fault( KPTRegs *pt_regs );
void kswitch_pgd( u32 pgd );
void kinit_vmm();

#endif /* ifndef _KVMM_H_ */
