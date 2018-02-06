#include <kconsole.h>
#include <ktypes.h>

struct multiboot {
  ;
};
u32 write( u8, u8, u8, u32 );

int kmain( struct multiboot *mboot_ptr )
{
  kconsole_clear();
  for ( int i = 0; i < 25; ++i )
    for ( int j = 0; j < 80; ++j )
      kconsole_puts( (const u8 *)"c" );

  for ( int j = 0; j < 81; ++j )
    kconsole_puts( (const u8 *)"O" );
  return 0;
}
