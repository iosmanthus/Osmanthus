#ifndef _KIO_H_
#define _KIO_H_

#include <karg.h>
#include <kvga.h>

// Kernel level I/O functions

i32 kvsprintf( char *s, const char *fmt, kva_list args );
i32 kprintf( const char *fmt, ... );
i32 kcprintf( VgaTextAtrr bg, VgaTextAtrr fg, const char *fmt,
              ... ); // With color
i32 kputchar( char ch );
i32 kcputchar( char ch, VgaTextAtrr bg, VgaTextAtrr fg );
i32 kputs( const char *str );
i32 kcputs( VgaTextAtrr bg, VgaTextAtrr fg, const char *str );

#endif /* ifndef _KIO_H_ */
