#ifndef _KGDT_H_
#define _KGDT_H_
#include <ktypes.h>

typedef struct _KSegmentDescritor {
  u16 limit_low;
  u16 base_low;
  u8 base_mid;
  u8 access;
  u8 granularity;
  u8 base_high;
} __attribute__( ( packed ) ) KSegmentDescritor;

typedef struct _KGDTPtr {
  u16 limit; // GDT actual size - 1
  u32 addr;  // GDT begin addr
} __attribute__( ( packed ) ) KGDTPtr;

KGDTPtr kinit_gdt();
void kload_gdt( const KGDTPtr *gdt_ptr );

#endif /* ifndef _KGDT_H_ */
