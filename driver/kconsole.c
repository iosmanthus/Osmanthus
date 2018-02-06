#include <kconsole.h>
#include <kports.h>

#define CURSOR_CTRL_PORT 0x3d4 // Control port
#define CURSOR_DATA_PORT 0x3d5 // Data port

#define HEIGHT 25
#define WIDTH 80

#define VGA ( (u8 *)0xb8000 )


static void
  set_cursor( u32 offset ); // Set cursor position with offset from 0xb8000

static u32 get_cusor();                // Get cursor offset
static u32 get_pos_x( u32 offset );    // Get cursor's X position
static u32 get_pos_y( u32 offset );    // Get cursor's Y position
static u32 get_offset( u32 x, u32 y ); // Transform position to offset

// static u32 write( u8 ch, VgaTextAtrr bg, VgaTextAtrr fg,
//                  u32 offset ); // Write VGA memory


void set_cursor( u32 offset )
{
  offset >>= 1;
  kports_out( CURSOR_CTRL_PORT, 0xe, BYTE );         // Open higher byte
  kports_out( CURSOR_DATA_PORT, offset >> 8, BYTE ); // Write

  kports_out( CURSOR_CTRL_PORT, 0xf, BYTE );               // Open lower byte
  kports_out( CURSOR_DATA_PORT, ( offset & 0xff ), BYTE ); // Write
}

u32 get_cusor()
{
  u32 offset = 0;
  kports_out( CURSOR_CTRL_PORT, 0xe, BYTE );
  offset = kports_in( CURSOR_DATA_PORT ) << 8;

  kports_out( CURSOR_CTRL_PORT, 0xf, BYTE );
  offset += kports_in( CURSOR_DATA_PORT );

  return offset << 1;
}

u32 get_pos_x( u32 offset )
{
  offset >>= 1;
  return offset / WIDTH;
}

u32 get_pos_y( u32 offset )
{
  offset >>= 1;
  return offset % WIDTH;
}

u32 get_offset( u32 x, u32 y )
{
  return ( x * WIDTH + y ) << 1;
}

u32 write( u8 ch, VgaTextAtrr bg, VgaTextAtrr fg, u32 offset )
{
  u32 x = get_pos_x( offset ), y = get_pos_y( offset );

  if ( x >= HEIGHT || y >= WIDTH ) {
    kconsole_scroll();
    return get_offset( HEIGHT - 1, 0 );
  }

  if ( ch == '\n' ) {
    if ( x == 24 ) {
      kconsole_scroll();
      return get_offset( HEIGHT - 1, 0 );
    } else
      return get_offset( x + 1, 0 );
  }

  VGA[offset] = ch;
  VGA[offset + 1] = ( (u8)bg << 4 ) + (u8)fg;

  return offset + 2;
}

i32 kconsole_puts_color( const u8 *str, VgaTextAtrr bg, VgaTextAtrr fg )
{
  u32 offset = get_cusor();
  const u8 *s = str;
  i32 cnt = 0;
  while ( *s ) {
    cnt++;
    offset = write( *s++, bg, fg, offset );
    set_cursor( offset );
  }
  return cnt;
}

i32 kconsole_puts( const u8 *str )
{
  return kconsole_puts_color( str, VGA_BLACK, VGA_WHITE );
}

void kconsole_clear()
{
  set_cursor( 0 );
  for ( u16 *p = (u16 *)VGA, *end = &p[HEIGHT * WIDTH]; p < end; ++p )
    *p = 0;
}

void kconsole_scroll()
{
  typedef u16( *VideoMem )[WIDTH];
  VideoMem v = (VideoMem)VGA;

  for ( i32 i = 0; i < HEIGHT - 1; ++i )
    for ( i32 j = 0; j < WIDTH; ++j )
      v[i][j] = v[i + 1][j];

  for ( i32 j = 0; j < WIDTH; ++j )
    v[HEIGHT - 1][j] = 0;
}
