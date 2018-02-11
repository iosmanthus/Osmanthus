#include <kelf.h>
#include <kstring.h>

#define ELF32_ST_TYPE( i ) ( (i)&0xf )

// Get kernel elf info
KELF kget_kernel_elf_info( const KMultiBoot *mb )
{
  KELFSectionHeader *begin = (KELFSectionHeader *)mb->addr;
  KELF elf;

  u32 shstrtab = begin[mb->shndx].addr; // Section name array
  // Loop through all sections

  const KELFSectionHeader *const end = &begin[mb->num];
  for ( const KELFSectionHeader *sh = begin; sh < end; ++sh ) {
    const char *section_name = (const char *)( shstrtab + sh->name );

    if ( kstrcmp( section_name, ".strtab" ) == 0 ) {
      elf.strtab = (const char *)sh->addr;
      elf.strtabsz = sh->size;
      continue;
    } else if ( kstrcmp( section_name, ".symtab" ) == 0 ) {
      elf.symtab = (KELFSymbol *)sh->addr;
      elf.symtabsz = sh->size;
    }
  }
  return elf;
}

// Return symbol whose address range includes 'addr'
const char *kelf_search_symbol( u32 addr, const KELF *elf )
{
  if ( elf ) {
    const KELFSymbol *const end =
      &elf->symtab[elf->symtabsz / sizeof( KELFSymbol )];

    for ( const KELFSymbol *symbol = elf->symtab; symbol < end; ++symbol )
      if ( ELF32_ST_TYPE( symbol->info ) != 0x2 )
        continue;
      else if ( addr >= symbol->value && addr < symbol->value + symbol->size )
        return (const char *)( elf->strtab + symbol->name );
  }
  return NULL;
}
