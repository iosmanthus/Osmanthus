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

#include <kdebug.h>
#include <kidt.h>
#include <kio.h>
#include <kisr.h>
#include <kport.h>

static KInterruptHandler int_handler_pool[IDT_ENTRY_CNT] = {NULL};

// Register an interrupt handler function
void kreg_int_handler(u32 int_id, KInterruptHandler handler)
{
  int_handler_pool[int_id] = handler;
}

void kisr_handler(KPTRegs *pt_regs)
{
  // If registered
  KInterruptHandler handler = int_handler_pool[pt_regs->int_id];
  if (handler)
    handler(pt_regs);
  else
    kcprintf_unsafe(VGA_BLACK, VGA_RED, "Unhandled interrupt: %d\n",
		    pt_regs->int_id);
}


void kirq_handler(KPTRegs *pt_regs)
{
  // Send an EOI (end of interrupt) signal to the PICs.
  // If this interrupt involved the slave.
  if (pt_regs->int_id >= 40)
    kout(0xA0, 0x20, KBYTE);

  // Reset master
  kout(0x20, 0x20, KBYTE);

  kisr_handler(pt_regs);
}

void kcli()
{
  __asm__ volatile("cli");
}

void ksti()
{
  __asm__ volatile("sti");
}
