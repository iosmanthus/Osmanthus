#include <katomic.h>

u32 kfectch_and_increase(u32 *addr)
{
  u32 retval = 0;
  __asm__ volatile(
    "mov $1,%%eax\n\t"
    "lock xadd %%eax,(%0)\n\t" ::"c"(addr));
  __asm__ volatile("mov %%eax,%0" : "=r"(retval) :);
  return retval;
}

void kincrease(u32 *addr)
{
  __asm__ volatile("lock incl (%0)" ::"r"(addr));
}
