#include <kconsole.h>
#include <ktypes.h>

struct multiboot {
  ;
};
u32 write( u8, u8, u8, u32 );

int kmain( struct multiboot *mboot_ptr )
{
  kconsole_clear();
  kconsole_puts_color(
    (const u8 *)"Welcome to Osmanthus!\n"
                "I am a fucking stupid operating system!\n"
                "The only thing i can do is:\n"
                "printing some useless words on the god damn small console\n",
    VGA_BLACK, VGA_GREEN );
  kconsole_clear();
  kconsole_puts_color(
    (const u8 *)"Welcome to Osmanthus!\n"
                "I am a fucking stupid operating system!\n"
                "The only thing i can do is:\n"
                "printing some useless words on the god damn small console\n",
    VGA_BLACK, VGA_GREEN );
  return 0;
}
