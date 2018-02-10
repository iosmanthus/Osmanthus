#include <kport.h>
#include <kvga.h>

#define CURSOR_CTRL_PORT 0x3d4 // Control port
#define CURSOR_DATA_PORT 0x3d5 // Data port

#define HEIGHT 25
#define WIDTH 80
#define END ( ( HEIGHT * WIDTH ) << 1 )

#define VGA ( (u8 *)0xb8000 ) // VGA base address


static void
  set_cursor( u32 offset ); // Set cursor position with offset from 0xb8000

static u32 get_cursor();               // Get cursor offset
static u32 get_pos_x( u32 offset );    // Get cursor's X position
static u32 get_pos_y( u32 offset );    // Get cursor's Y position
static u32 get_offset( u32 x, u32 y ); // Transform position to offset

// static u32 write( u8 ch, VgaTextAtrr bg, VgaTextAtrr fg,
//                  u32 offset ); // Write VGA memory


inline void set_cursor( u32 offset )
{
  offset >>= 1;
  kout( CURSOR_CTRL_PORT, 0xe, KBYTE );         // Open higher byte
  kout( CURSOR_DATA_PORT, offset >> 8, KBYTE ); // Write

  kout( CURSOR_CTRL_PORT, 0xf, KBYTE );               // Open lower byte
  kout( CURSOR_DATA_PORT, ( offset & 0xff ), KBYTE ); // Write
}

inline u32 get_cursor()
{
  u32 offset = 0;
  kout( CURSOR_CTRL_PORT, 0xe, KBYTE );
  offset = kin( CURSOR_DATA_PORT, KBYTE ) << 8;

  kout( CURSOR_CTRL_PORT, 0xf, KBYTE );
  offset += kin( CURSOR_DATA_PORT, KBYTE );

  return offset << 1;
}

inline u32 get_pos_x( u32 offset )
{
  offset >>= 1;
  return offset / WIDTH;
}

inline u32 get_pos_y( u32 offset )
{
  offset >>= 1;
  return offset % WIDTH;
}

inline u32 get_offset( u32 x, u32 y )
{
  return ( x * WIDTH + y ) << 1;
}


u32 write( char ch, VgaTextAtrr bg, VgaTextAtrr fg, u32 offset )
{
  u32 x = get_pos_x( offset ), y = get_pos_y( offset );

  if ( x >= HEIGHT || y >= WIDTH ) {
    kvga_scroll();
    offset = get_offset( HEIGHT - 1, 0 );
  }

  if ( ch == '\n' ) {
    if ( x == 24 ) {
      kvga_scroll();
      return get_offset( HEIGHT - 1, 0 );
    } else
      return get_offset( x + 1, 0 );
  }

  if ( ch == '\t' ) {
    for ( int i = 0; i < 4; ++i )
      offset = write( ' ', bg, fg, offset );
    return offset;
  }

  VGA[offset] = ch;
  VGA[offset + 1] = ( (u8)bg << 4 ) + (u8)fg;

  return offset + 2;
}

i32 kvga_cputc( char ch, VgaTextAtrr bg, VgaTextAtrr fg )
{
  u32 offset = get_cursor();
  offset = write( ch, bg, fg, offset );
  set_cursor( offset );
  return ch;
}

inline i32 kvga_putc( char ch )
{
  return kvga_cputc( ch, VGA_BLACK, VGA_WHITE );
}

i32 kvga_cputs( const char *str, VgaTextAtrr bg, VgaTextAtrr fg )
{
  u32 offset = get_cursor();
  i32 cnt = 0;
  while ( *str ) {
    cnt++;
    offset = write( *str++, bg, fg, offset );
    set_cursor( offset );
  }
  return cnt;
}

i32 kvga_puts( const char *str )
{
  return kvga_cputs( str, VGA_BLACK, VGA_WHITE );
}

void kvga_clear()
{
  set_cursor( 0 );
  u8 *s = VGA;
  while ( s < &VGA[END] ) {
    *s++ = 0;
    *s++ = ( u8 )( VGA_BLACK << 4 ) + (u8)VGA_WHITE;
  }
}

void kvga_scroll()
{
  typedef u16( *VideoMem )[WIDTH];
  VideoMem v = (VideoMem)VGA;

  for ( i32 i = 0; i < HEIGHT - 1; ++i )
    for ( i32 j = 0; j < WIDTH; ++j )
      v[i][j] = v[i + 1][j];

  for ( i32 j = 0; j < WIDTH; ++j )
    v[HEIGHT - 1][j] = 0;
}
