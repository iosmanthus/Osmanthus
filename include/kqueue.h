#ifndef _KQUEUE_H_
#define _KQUEUE_H_

#include <ktypes.h>

typedef struct _KQueueNode {
  void *data;
  struct _KQueueNode *prev;
  struct _KQueueNode *next;
} KQueueNode;

typedef struct _KQueue {
  KQueueNode *front;
  KQueueNode *back;
} KQueue;

KQueueNode *kqueue_front(KQueue *queue);
KQueueNode *kqueue_back(KQueue *queue);
KQueue *kqueue_init();

KQueueNode *kqueue_pop_front(KQueue *queue);
void kqueue_push_back_with_data(KQueue *queue, void *data);
void kqueue_push_back(KQueue *queue, KQueueNode *node);
void kqueue_delete(KQueue *queue, KQueueNode *node);

#endif /* _KQUEUE_H_ */
