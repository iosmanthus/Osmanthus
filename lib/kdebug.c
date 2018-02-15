#include <kdebug.h>
#include <kelf.h>
#include <kio.h>
#include <kmultiboot.h>


static KELF kernel_elf;
static const KELF *pkelf = NULL;
extern KMultiBoot *__kernel_multiboot_info;

// Define in boot/boot.asm

static void env_init();

void env_init()
{
  if ( pkelf == NULL ) {
    kernel_elf = kget_kernel_elf_info( __kernel_multiboot_info );
    pkelf = &kernel_elf;
  }
}

void kpanic( const char *msg )
{
  kprintf( "****Fatal error eccured****\n"
           "****Kernel in panic****\n" );
  if ( msg )
    kprintf( "%s\n", msg );
  kstack_trace();
  while ( 1 )
    ;
}

void __kassert( const char *file, int line, const char *func, const char *expr )
{
  kcprintf( VGA_BLACK, VGA_LIGHT_BROWN,
            "In file %s: %d function %s asserts '%s' failed\n", file, line,
            func, expr );
  kpanic( NULL );
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
     * |    &$    | < ---- call function `x` ($ is the addr of next instruction)
     * | -------- |
     * | ebp_prev | < ---- push ebp; mov ebp, esp | <= esp , ebp
     * | -------- |
     */
    eip = ebp + 1; // (void *)eip == (void *)ebp + 4
    kprintf( "\t[%p]:\t%s\n", *eip, ksearch_function( *eip, pkelf ) );
    ebp = (u32 *)*ebp;
  }
}
