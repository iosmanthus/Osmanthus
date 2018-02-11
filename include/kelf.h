#ifndef _KELF_H_
#define _KELF_H_

#include <kmultiboot.h>
#include <ktypes.h>

// ELF section header
typedef struct _KELFSectionHeader {
  u32 name;
  u32 type;
  u32 flags;
  u32 addr;
  u32 offset;
  u32 size;
  u32 link;
  u32 info;
  u32 addralign;
  u32 entsize;
} __attribute__( ( packed ) ) KELFSectionHeader;

// ELF Symbol
typedef struct _KELFSymbol {
  u32 name;
  u32 value;
  u32 size;
  u8 info;
  u8 other;
  u16 shndx;
} __attribute__( ( packed ) ) KELFSymbol;

// ELF info
typedef struct _KELF {
  KELFSymbol *symtab;
  u32 symtabsz;
  const char *strtab;
  u32 strtabsz;
} KELF;

// Get kernel elf info
KELF kget_kernel_elf_info( KMultiBoot *mb );

// Return symbol whose address range includes 'addr'
const char *kelf_search_symbol( u32 addr, const KELF *elf );

#endif /* ifndef _KELF_H_ */
