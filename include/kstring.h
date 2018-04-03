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

#ifndef _KSTRING_H_
#define _KSTRING_H_

#include <ktypes.h>

void* kmemcpy(void* dst, const void* src, u32 n);

void* kmemset(void* s, u8 c, u32 n);
void* kbzero(void* s, u32 n);

i32 kstrcmp(const char* s1, const char* s2);
i32 kstrncmp(const char* s1, const char* s2, u32 n);

char* kstrcpy(char* dst, const char* src);
char* kstrncpy(char* dst, const char* src, u32 n);

char* kstrcat(char* dest, const char* src);
char* kstrncat(char* dest, const char* src, u32 n);

u32 kstrlen(const char* s);

#endif /* ifndef _KSTRING_H_ */
