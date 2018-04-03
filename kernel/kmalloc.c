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

#include <kmalloc.h>
#include <kpmm.h>
#include <kvmm.h>

static KBlockHeader *heap_start = NULL;
static KBlockHeader *heap_end = (KBlockHeader *)KHEAP_START;

static void split_block(KBlockHeader *header, u32 size);
static void alloc_block(u32 start, u32 size);
static void glue_next_block(KBlockHeader *header);
static KBlockHeader *glue_block(KBlockHeader *header);
static void free_block(KBlockHeader *header);

void *kmalloc(u32 size) {

  // Add block header to a block
  size += sizeof(KBlockHeader);
  size = BLOCK_ALIGNED(size);

  KBlockHeader *cur_blk = heap_start;
  KBlockHeader *prev_blk = NULL;
  while (cur_blk) {
    if (cur_blk->used == 0 && cur_blk->block_size > size) {
      split_block(cur_blk, size);
      cur_blk->used = 1;
      return (void *)((u32)cur_blk + sizeof(KBlockHeader));
    }
    // Try to find an availiable block
    prev_blk = cur_blk;
    cur_blk = cur_blk->next;
  }

  u32 blk_addr = 0;
  if (prev_blk) {
    blk_addr = (u32)prev_blk + prev_blk->block_size;
  } else {
    blk_addr = KHEAP_START;
    heap_start = (KBlockHeader *)blk_addr;
  }

  alloc_block(blk_addr, size);
  cur_blk = (KBlockHeader *)blk_addr;
  cur_blk->prev = prev_blk;
  cur_blk->next = NULL;
  cur_blk->used = 1;
  cur_blk->block_size = size;
  if (prev_blk)
    prev_blk->next = cur_blk;

  // Cut off the header
  return (void *)((u32)cur_blk + sizeof(KBlockHeader));
}

void kfree(void *ptr) {
  if (ptr) {
    // Recover the header
    KBlockHeader *blk = (KBlockHeader *)((u32)ptr - sizeof(KBlockHeader));
    blk->used = 0;
    blk = glue_block(blk);
    free_block(blk);
  }
}

void split_block(KBlockHeader *header, u32 size) {
  // Make sure one header left
  // Because all block's size are aligned to 16 bytes
  // then at least 16 bytes left in a block after spliting operation
  if (header->used == 0 &&
      header->prev->block_size - size > sizeof(KBlockHeader)) {

    KBlockHeader *new_blk = (KBlockHeader *)((u32)header + size);
    new_blk->prev = header;
    new_blk->next = header->next;
    if (header->next)
      header->next->prev = new_blk;
    new_blk->block_size = header->block_size - size;
    new_blk->used = 0;

    header->block_size = size;
    header->next = new_blk;
  }
}

void alloc_block(u32 start, u32 size) {
  u32 end = (u32)heap_end;
  // Allocate more physical pages if heap is not large enough
  while (start + size > end) {
    u32 phy_page_addr = kphy_page_alloc();
    kmap(kget_kernel_pgd(), end, phy_page_addr, PAGE_PRESENT | PAGE_WRITE);
    end += KPMM_PAGE_SIZE;
  }
  heap_end = (KBlockHeader *)end;
}

void glue_next_block(KBlockHeader *header) {
  header->block_size += header->next->block_size;
  header->next = header->next->next;
  if (header->next)
    header->next->prev = header;
}

KBlockHeader *glue_block(KBlockHeader *header) {
  if (header && header->used == 0) {
    if (header->next && header->next->used == 0)
      glue_next_block(header);
    if (header->prev && header->prev->used == 0) {
      header = header->prev;
      glue_next_block(header);
    }
  }
  return header;
}

void free_block(KBlockHeader *header) {
  // if and only if the header represent the last block
  if (header && !header->used && !header->next) {
    if (header->prev)
      header->prev->next = NULL;
    else
      heap_start = NULL;

    u32 size = header->block_size;
    u32 end = (u32)heap_end;
    while (size >= KPMM_PAGE_SIZE) {
      // Unmap the page and free the physical page
      end -= KPMM_PAGE_SIZE;
      size -= KPMM_PAGE_SIZE;
      u32 phy_page_addr = kget_mapping(kget_kernel_pgd(), end);
      kunmap(kget_kernel_pgd(), end);
      kphy_page_free(phy_page_addr);
    }
    header->block_size = size;
    heap_end = (KBlockHeader *)end;
  }
}
