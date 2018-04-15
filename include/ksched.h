#ifndef _KSCHED_H_
#define _KSCHED_H_

#include <kmutex.h>
#include <kqueue.h>
#include <kthread.h>

void kinit_sched();
void kthread_add(KThread *thread);
void kscheduler();
u32 scheduler_lock();
void scheduler_unlock();

#endif /* _KSCHED_H_ */
