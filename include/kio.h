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

#ifndef _KIO_H_
#define _KIO_H_

#include <karg.h>
#include <kvga.h>

// Kernel level I/O functions

i32 kvsprintf(char* s, const char* fmt, kva_list args);
i32 kprintf(const char* fmt, ...);
i32 kcprintf(VgaTextAtrr bg, VgaTextAtrr fg, const char* fmt,
    ...); // With color
i32 kputchar(char ch);
i32 kcputchar(char ch, VgaTextAtrr bg, VgaTextAtrr fg);
i32 kputs(const char* str);
i32 kcputs(VgaTextAtrr bg, VgaTextAtrr fg, const char* str);

#endif /* ifndef _KIO_H_ */
