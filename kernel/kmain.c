#include <kconsole.h>
#include <ktypes.h>

struct multiboot {
  ;
};
u32 write( u8, u8, u8, u32 );

int kmain( struct multiboot *mboot_ptr )
{
  kputs( (const u8 *)"hello world\n" );
  return 0;
}
