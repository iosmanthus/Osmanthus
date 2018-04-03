/*
 * Copyright (c) 2018 Liming,Deng <myosmanthustree@gmail.com>
 * Author: Liming Deng
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _KGDT_H_
#define _KGDT_H_
#include <ktypes.h>

#define GDT_ENTRY_CNT 5
#define GDT_ENTRY_SIZE 8

#define NULL_SEG_INDEX 0
#define CODE_SEG_INDEX 1
#define DATA_SEG_INDEX 2
#define USER_CODE_SEG_INDEX 3
#define USER_DATA_SEG_INDEX 4

#define GDT_CODE_SELECTOR (GDT_ENTRY_SIZE * CODE_SEG_INDEX)
#define GDT_DATA_SELECTOR (GDT_ENTRY_SIZE * DATA_SEG_INDEX)
#define GDT_USER_CODE_SELECTOR (GDT_ENTRY_SIZE * USER_CODE_SEG_INDEX)
#define GDT_USER_DATA_SELECTOR (GDT_ENTRY_SIZE * USER_DATA_SEG_INDEX)

typedef struct _KSegmentDescritor {
  u16 limit_low;
  u16 base_low;
  u8 base_mid;
  u8 access;
  u8 granularity;
  u8 base_high;
} __attribute__((packed)) KSegmentDescritor;

typedef struct _KGDTPtr {
  u16 limit; // GDT actual size - 1
  u32 addr;  // GDT begin addr
} __attribute__((packed)) KGDTPtr;

KGDTPtr kinit_gdt();
void kload_gdt(const KGDTPtr* gdt_ptr);

#endif /* ifndef _KGDT_H_ */
