#ifndef _KCONSOLE_H_
#define _KCONSOLE_H_

#include <ktypes.h>

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

/*
 * Print a character on the console and move the cursor
 * with specified attribute
 *
 * 'ch': character to be printed
 * 'bg': background color
 * 'fg': foreground color
 *
 * return the character's ASCII value if successed
 * or return EOF defined in <ktypes.h>
 * */
i32 kputc_color( u8 ch, VgaTextAtrr bg, VgaTextAtrr fg );


/*
 * Print a character on the console and move the cursor
 *
 * 'ch': character to be printed
 *
 * return the character's ASCII value if successed
 * or return EOF defined in <ktypes.h>
 * */
i32 kputc( u8 ch );

/*
 * Print a string on the console and move the cursor
 * with specified attribute
 *
 * 'str': the string pointer to be printed
 * 'bg': background color
 * 'fg': foreground color
 *
 * return the count of character that has been printed if successed
 * or return EOF defined in <ktypes.h>
 * */
i32 kputs_color( const u8 *str, VgaTextAtrr bg, VgaTextAtrr fg );


/*
 * Print a string on the console and move the cursor
 *
 * 'str': the string pointer to be printed
 *
 * return the count of character that has been printed if successed
 * or return EOF defined in <ktypes.h>
 * */
i32 kputs( const u8 *str );

/*
 * Scroll the screen if there is no more space
 * in the console to be written.
 * */
void kscroll();

/*
 * Clear the console
 * */
void kclear();

#endif /* ifndef _KCONSOLE_H_ */
