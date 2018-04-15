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

#include <kqueue.h>
#include <kmalloc.h>


KQueueNode *kqueue_front(KQueue *queue)
{
  if (queue)
    return queue->front;
  return NULL;
}

KQueueNode *kqueue_back(KQueue *queue)
{
  if (queue)
    return queue->back;
  return NULL;
}

KQueue *kqueue_init()
{
  KQueue *queue = (KQueue *)kmalloc(sizeof(KQueue));
  queue->front = queue->back = NULL;
  return queue;
}

KQueueNode *kqueue_pop_front(KQueue *queue)
{
  if (queue && queue->front) {
    KQueueNode *ret = queue->front;

    kqueue_delete(queue, queue->front);

    return ret;
  }
  return NULL;
}
void kqueue_push_back(KQueue *queue, KQueueNode *node)
{
  if (queue) {
    node->next = NULL;
    node->prev = NULL;
    if (queue->back) {
      queue->back->next = node;
      node->prev = queue->back;
    } else
      queue->front = node;
    queue->back = node;
  }
}

void kqueue_delete(KQueue *queue, KQueueNode *node)
{
  if (queue && node) {

    if (node == queue->front)
      queue->front = node->next;
    if (node == queue->back)
      queue->back = node->prev;

    if (node->prev)
      node->prev->next = node->next;
    if (node->next)
      node->next->prev = node->prev;
  }
}
void kqueue_push_back_with_data(KQueue *queue, void *data)
{
  if (queue) {

    KQueueNode *node = (KQueueNode *)kmalloc(sizeof(KQueueNode));
    node->data = data;
    node->next = NULL;

    kqueue_push_back(queue, node);
  }
}
