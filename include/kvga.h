#ifndef _KVGA_H_
#define _KVGA_H_

#include <ktypes.h>

#define TAB_WIDTH 4

typedef enum _VgaTextAtrr {
  VGA_BLACK = 0,
  VGA_BLUE,
  VGA_GREEN,
  VGA_CYAN,
  VGA_RED,
  VGA_MAGENTA,
  VGA_BROWN,
  VGA_LIGHT_GREY,
  VGA_DARK_GREY,
  VGA_LIGHT_BLUE,
  VGA_LIGHT_GREEN,
  VGA_LIGHT_CYAN,
  VGA_LIGHT_RED,
  VGA_LIGHT_MAGENTA,
  VGA_LIGHT_BROWN, // Yellow
  VGA_WHITE
} VgaTextAtrr;

i32 kvga_cputc( char ch, VgaTextAtrr bg, VgaTextAtrr fg ); // With color

i32 kvga_putc( char ch );

i32 kvga_cputs( const char *str, VgaTextAtrr bg, VgaTextAtrr fg ); // With color

i32 kvga_puts( const char *str );

void kvga_scroll();

void kvga_clear();

#endif /* ifndef _kvga_H_ */
