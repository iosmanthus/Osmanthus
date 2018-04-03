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

#include <karg.h>
#include <kdebug.h>
#include <kelf.h>
#include <kgdt.h>
#include <kidt.h>
#include <kio.h>
#include <kisr.h>
#include <kmalloc.h>
#include <kmultiboot.h>
#include <kpit.h>
#include <kpmm.h>
#include <kport.h>
#include <kstring.h>
#include <ktypes.h>
#include <kvga.h>
#include <kvmm.h>

// QEMU shudown
void kshutdown() { kout(0xf4, 0x00, KBYTE); }

void hlt() {
  while (1)
    __asm__ volatile("hlt");
}

typedef struct _KList {
  void *data;
  struct _KList *next;
  struct _KList *prev;
} KList;

KList *klist_append(KList *list, void *data) {
  KList *node = (KList *)kmalloc(sizeof(KList));
  node->data = data;
  node->next = NULL;

  if (!list)
    return node;

  KList *iter = list;
  while (iter->next)
    iter = iter->next;

  iter->next = node;
  node->prev = iter;

  return list;
}

KList *klist_delete(KList *list, KList *node) {
  KList *iter = list;
  while (iter && iter != node)
    iter = iter->next;

  if (!iter)
    return list;

  if (iter->prev)
    iter->prev->next = iter->next;
  if (iter->next)
    iter->next->prev = iter->prev;

  KList *new_head = (node == list) ? list->next : list;
  kfree(node);
  return new_head;
}

void kmain() {
  KGDTPtr gdt_ptr = kinit_gdt();

  kload_gdt(&gdt_ptr);
  KIDTPtr idt_ptr = kget_idt();

  kload_idt(&idt_ptr);

  kinit_pmm();
  kinit_vmm();

  kprintf("%u\n", kget_phy_pages_avail());
  KList *list = NULL;
  for (i32 i = 0; i < 1024; ++i) {
    list = klist_append(list, (void *)i);
    kprintf("%u\n", kget_phy_pages_avail());
  }

  KList *iter = list;
  while (iter) {
    kprintf("%d\n", iter->data);
    iter = iter->next;
  }

  for (i32 i = 0; i < 1024; ++i) {
    list = klist_delete(list, list);
    kprintf("%u\n", kget_phy_pages_avail());
  }

  hlt();
}
