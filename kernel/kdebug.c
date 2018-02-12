#include <kdebug.h>
#include <kelf.h>
#include <kio.h>

static KELF kernel_elf;
static const KELF *pkelf = NULL;

// Define in boot/boot.asm
extern KMultiBoot *__kernel_multiboot_info;

static void env_init();

void env_init()
{
  if ( pkelf == NULL ) {
    kernel_elf = kget_kernel_elf_info( __kernel_multiboot_info );
    pkelf = &kernel_elf;
  }
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
     * |    &x    | < ---- call function `x`
     * | -------- |
     * | ebp_prev | < ---- push ebp; mov ebp, esp | <= esp , ebp
     * | -------- |
     */
    eip = ebp + 1; // (void *)eip == (void *)ebp + 4
    kprintf( "%p\t:%s", *eip, VGA_BLACK, VGA_RED,
             kelf_search_symbol( *eip, pkelf ) );
    ebp = (u32 *)*ebp;
  }
}
