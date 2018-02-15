#include <karg.h>
#include <kdebug.h>
#include <kelf.h>
#include <kgdt.h>
#include <kio.h>
#include <kmultiboot.h>
#include <kport.h>
#include <kstring.h>
#include <ktypes.h>
#include <kvga.h>

// QEMU shudown
void kshutdown()
{
  kout( 0xf4, 0x00, KBYTE );
}

KMultiBoot *__kernel_multiboot_info = NULL;

int sum_up( int n, ... )
{
  kva_list arg_list;
  kva_start( arg_list, n );
  int sum = 0;
  while ( n-- > 0 )
    sum += kva_arg( arg_list, int );
  kva_end( arg_list );
  return sum;
}

i32 kmain()
{
  KGDTPtr gdt_ptr = kinit_gdt();
  kload_gdt( &gdt_ptr );
  kprintf( "hello world\n" );
  return 0;
}
