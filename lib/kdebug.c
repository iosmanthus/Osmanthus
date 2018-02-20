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
#include <kelf.h>
#include <kio.h>
#include <kmultiboot.h>


static KELF kernel_elf;
static const KELF *pkelf = NULL;
extern KMultiBoot *__kernel_multiboot_info;

// Define in boot/boot.asm

static void env_init();

void env_init()
{
  if ( pkelf == NULL ) {
    kernel_elf = kget_kernel_elf_info( __kernel_multiboot_info );
    pkelf = &kernel_elf;
  }
}

void kpanic( const char *msg )
{
  kprintf( "****Fatal error eccured****\n"
           "****Kernel in panic****\n" );
  if ( msg )
    kprintf( "%s\n", msg );
  kstack_trace();
  while ( 1 )
    ;
}

void __kassert( const char *file, int line, const char *func, const char *expr )
{
  kcprintf( VGA_BLACK, VGA_LIGHT_BROWN,
            "In file %s: %d function %s asserts '%s' failed\n", file, line,
            func, expr );
  kpanic( NULL );
}

void kstack_trace()
{
  env_init();
  u32 *ebp, *eip;

  // Get %ebp
  __asm__ volatile( "mov %%ebp,%0" : "=r"( ebp ) );

  // %ebp was initiated in boot/boot.asm with 0x0
  while ( ebp ) {
    /*
     * | -------- |
     * |   ...    |  < ---- ebp_prev
     * | -------- |
     * |    .     |
     * |    .     |
     * |    .     |
     * | -------- |
     * |    &$    | < ---- call function `x` ($ is the addr of next instruction)
     * | -------- |
     * | ebp_prev | < ---- push ebp; mov ebp, esp | <= esp , ebp
     * | -------- |
     */
    eip = ebp + 1; // (void *)eip == (void *)ebp + 4
    kprintf( "\t[%p]:\t%s\n", *eip, ksearch_function( *eip, pkelf ) );
    ebp = (u32 *)*ebp;
  }
}
