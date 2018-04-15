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

#ifndef _KTHREAD_H_
#define _KTHREAD_H_

#include <ktypes.h>
#include <kvmm.h>
#include <kpmm.h>

typedef u32 KTID;

typedef enum _KTHREAD_STATE {
  KTHREAD_NEW,

  KTHREAD_RUNNABLE,

  KTHREAD_BLOCKED,

  KTHREAD_WAITING,

  KTHREAD_TIMED_WAITING,

  KTHREAD_TERMINATED

} KTHREAD_STATE;

/*typedef struct _KThreadMemInfo {
  KPGD *pgd;
  void *stack;
  } KThreadMemInfo;*/

typedef struct _KThreadContext {
  u32 esp;
  u32 ebp;
  u32 ebx;
  u32 esi;
  u32 edi;
  u32 eflags;
} KThreadContext;

typedef struct _KThread {
  KTID tid;
  KThreadContext context;
  volatile KTHREAD_STATE state;
  void *stack;
  void *retval;
  void *box;
} KThread;

KTID kthread_create(void *(*func)(void *arg), void *arg);
void kthread_exit(void *retval);
KTID kthread_self();
KThread *kthread_self_ref();
void kthread_join(KTID tid, void **retval);

#endif /* _KTHREAD_H_ */
