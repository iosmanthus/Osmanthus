#ifndef _KMUTEX_H_
#define _KMUTEX_H_

#include <kqueue.h>
#include <ktypes.h>

#define KTHREAD_MUTEX_INITIALIZER                                              \
  (KThreadMutex){                                                              \
    .entries = 0, .exits = 0, .block = {.front = NULL, .back = NULL}};

typedef struct _KThreadMutex {
  u32 entries;
  u32 exits;
  KQueue block;
} KThreadMutex;

void kthread_mutex_lock(KThreadMutex *mutex);
void kthread_mutex_unlock(KThreadMutex *mutex);

#endif /* _KMUTEX_H_ */
