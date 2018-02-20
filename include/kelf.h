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
KELF kget_kernel_elf_info( const KMultiBoot *mb );

// Return symbol whose address range includes 'addr'
const char *ksearch_function( u32 addr, const KELF *elf );

#endif /* ifndef _KELF_H_ */
