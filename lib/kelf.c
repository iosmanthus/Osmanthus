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

// Return the name of the function whose address range includes 'addr'
const char *ksearch_function( u32 addr, const KELF *elf )
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
