#include <ksched.h>
#include <kthread.h>
#include <kio.h>
#include <kmalloc.h>
#include <kmutex.h>
#include <kisr.h>
#include <kpit.h>
#include <katomic.h>
#include <kqueue.h>


static KQueueNode *running = NULL;

static KQueue ready = (KQueue){.front = NULL, .back = NULL};
static KThreadMutex ready_mutex = KTHREAD_MUTEX_INITIALIZER;

static KQueue terminated = (KQueue){.front = NULL, .back = NULL};
static KThreadMutex terminated_mutex = KTHREAD_MUTEX_INITIALIZER;

static KThreadMutex scheduler_mutex = KTHREAD_MUTEX_INITIALIZER;


static inline void timer_interrput_handler(KPTRegs *pt_regs);
static inline void thread_exit();


extern void kthread_context_switch(KThreadContext *prev, KThreadContext *next);

void kinit_sched()
{
  if (!ready.front) {

    KThread *thread = (KThread *)kmalloc_unsafe(sizeof(KThread));
    thread->tid = 0;
    thread->state = KTHREAD_NEW;
    thread->stack = kget_kernel_stack_base();

    // Add to ready queue
    KQueueNode *node = (KQueueNode *)kmalloc_unsafe(sizeof(KQueueNode));
    node->data = thread;
    thread->box = node;
    kqueue_push_back(&ready, node);

    running = node;

    init_timer(200, timer_interrput_handler);
    ksti();
  }
}

void kthread_add(KThread *thread)
{
  kthread_mutex_lock(&ready_mutex);
  kqueue_push_back_with_data(&ready, thread);
  thread->box = ready.back;
  kthread_mutex_unlock(&ready_mutex);
}

void kthread_join(KTID tid, void **retval)
{
  KThread *wait = (KThread *)((!tid) ? ready.front->data : (void *)tid);
  while (wait->state != KTHREAD_TERMINATED)
    kscheduler();

  // Retrieve thread return value;
  *retval = wait->retval;

  kthread_mutex_lock(&terminated_mutex);

  kqueue_delete(&terminated, (KQueueNode *)wait->box);

  if (wait->tid)
    kfree(wait->stack);
  kfree(wait->box);
  kfree(wait);

  kthread_mutex_unlock(&terminated_mutex);
}

void kthread_exit(void *retval)
{
  KThread *me = kthread_self_ref();
  me->state = KTHREAD_TERMINATED;
  me->retval = retval;
  kthread_mutex_lock(&terminated_mutex);
  kthread_mutex_lock(&ready_mutex);

  kqueue_delete(&ready, running);
  kqueue_push_back(&terminated, running);

  kthread_mutex_unlock(&ready_mutex);
  kthread_mutex_unlock(&terminated_mutex);

  scheduler_lock();
  KThread *prev = (KThread *)running->data;
  KThread *next = (KThread *)ready.front->data;
  running = ready.front;
  kthread_context_switch(&prev->context, &next->context);
}

void thread_exit()
{
  register void *retval asm("%eax");
  kthread_exit(retval);
}

KTID kthread_create(void *(*func)(void *arg), void *arg)
{
  KThread *thread = (KThread *)kmalloc(sizeof(KThread));

  thread->tid = (u32)thread;
  thread->state = KTHREAD_RUNNABLE;
  thread->stack = kmalloc(KERNEL_STACK_SIZE);

  u32 *thread_stack_top = (u32 *)((u32)thread->stack + KERNEL_STACK_SIZE);

  // GOOD CODE ?
  *(--thread_stack_top) = (u32)arg;
  *(--thread_stack_top) = (u32)thread_exit;
  *(--thread_stack_top) = (u32)func;

  thread->context.eflags = 0x200;
  thread->context.esp = (u32)thread_stack_top;

  kthread_add(thread);
  return thread->tid;
}


u32 scheduler_lock()
{
  u32 tickets = kfectch_and_increase(&scheduler_mutex.entries);
  if (tickets != scheduler_mutex.exits)
    return 1;
  return 0;
}

void scheduler_unlock()
{
  ++scheduler_mutex.exits;
}

void task_switch()
{

  KThread *prev = (KThread *)running->data;

  if (ready.front != ready.back) {
    running = running->next;
    if (!running)
      running = ready.front;
  }

  KThread *next = (KThread *)running->data;

  if (prev != next)
    kthread_context_switch(&prev->context, &next->context);
  else
    scheduler_unlock();
}

void kscheduler()
{
  if (scheduler_lock()) {
    scheduler_unlock();
    return;
  }
  task_switch();
}

KTID kthread_self()
{
  return ((KThread *)running->data)->tid;
}

KThread *kthread_self_ref()
{
  return (KThread *)running->data;
}

void timer_interrput_handler(KPTRegs *pt_regs)
{
  kscheduler();
}
