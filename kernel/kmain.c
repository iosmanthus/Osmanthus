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
 * The abmain.c
    ~/Projects/Osmanthus/kernel/kove copyright notice and this permission notice
 shall be included in
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
#include <kthread.h>
#include <kmutex.h>
#include <kqueue.h>
#include <ksched.h>

// QEMU shudown
void kshutdown()
{
  kout(0xf4, 0x00, KBYTE);
}

void hlt()
{
  while (1)
    __asm__ volatile("hlt");
}

int g = 0;

KThreadMutex mutex = KTHREAD_MUTEX_INITIALIZER;

void *print_a(void *arg)
{
  kprintf("%p\n", arg);
  return arg;
}

void *print_b(void *arg)
{
  for (int i = 0; i < 100; ++i)
    kcprintf(VGA_BLACK, VGA_GREEN, "love world\n");
  // kthread_exit((void *)0xdeadbeaf);
  return (void *)0xdeadbeaf;
}

void *print_c(void *arg)
{
  for (int i = 0; i < 100; ++i)
    kcprintf(VGA_BLACK, VGA_LIGHT_RED, "fuck\n");
  return (void *)0xdeaddead;
}

void kmain()
{
  KGDTPtr gdt_ptr = kinit_gdt();
  kload_gdt(&gdt_ptr);

  KIDTPtr idt_ptr = kget_idt();
  kload_idt(&idt_ptr);

  kinit_pmm();
  kinit_vmm();


  // KQueue *queue = kqueue_init();
  // for (int i = 0; i < 10; ++i)
  //  kqueue_push_back(queue, (void *)i);
  // for (int i = 0; i < 10; ++i) {
  //  kprintf("%p\n", kqueue_front(queue)->data);
  //  kqueue_pop_front(queue);
  //}

  kinit_sched();
  KTID tid_a = kthread_create(print_a, (void *)0x8366251);
  KTID tid_b = kthread_create(print_b, (void *)0xdeadbeef);
  KTID tid_c = kthread_create(print_c, (void *)2);
  kcprintf(VGA_BLACK, VGA_CYAN, "hello world\n");

  void **ret = (void **)kmalloc(sizeof(void *));

  kthread_join(tid_a, ret);
  kprintf("%p\n", *ret);

  kthread_join(tid_b, ret);
  kprintf("%p\n", *ret);

  kthread_join(tid_c, ret);
  kprintf("%p\n", *ret);


  hlt();
}
