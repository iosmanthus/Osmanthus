#include <karg.h>
#include <kio.h>
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

i32 kmain( struct multiboot *mboot_ptr )
{
  i32 s = sum_up( 4, 1, 2, 3, 4 );
  kprintf( "%.4s\n%20d", "hello world", 123456789 );
  return 0;
}
