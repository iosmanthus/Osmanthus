#include <karg.h>
#include <kelf.h>
#include <kio.h>
#include <kmultiboot.h>
#include <kport.h>
#include <kstring.h>
#include <ktypes.h>
#include <kvga.h>

struct multiboot {
  ;
};

// QEMU shudown
void kshutdown()
{
  kout( 0xf4, 0x00, KBYTE );
}

int sum_up( int n, ... )
{
  kva_list arg_list;
  kva_start( arg_list, n );
  int sum = 0;
  while ( n-- )
    sum += kva_arg( arg_list, int );
  kva_end( arg_list );
  return sum;
}

i32 kmain( KMultiBoot *mboot_ptr )
{
  KELF elf = kget_kernel_elf_info( mboot_ptr );
  kprintf( "%s:\t%p\n", kelf_search_symbol( (u32)sum_up, &elf ), sum_up );
  kprintf( "%s:\t%p\n", kelf_search_symbol( (u32)kmain, &elf ), kmain );
  return 0;
}
