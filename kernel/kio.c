#include <kio.h>
#include <kstring.h>

#define LEFT 1  // Print in left-justify
#define PLUS 2  // Forces to show plus sign for positive number
#define SPACE 4 // If no sign exist, inserts space before the number

#define SPECIAL                                                                \
  8 // Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X
    // respectively for values different than zero.

#define ZEROPAD                                                                \
  16 // Left-pads the number with zeroes (0) instead of spaces when padding is
     // specified (disable in left-justify)

#define SIGN 32  // Negative number
#define SMALL 64 //  0x rather than 0X


static char *number( char *str, i32 num, i32 size, i32 base, i32 precision,
                     i32 type );
static i32 is_digit( i32 ch );
static i32 skip_atoi( const char **npptr );

inline i32 is_digit( i32 ch )
{
  return ( ch >= '0' && ch <= '9' );
}

inline i32 skip_atoi( const char **npptr )
{
  i32 i = 0;
  while ( is_digit( **npptr ) )
    i = i * 10 + *( ( *npptr )++ ) - '0';

  return i;
}

char *number( char *str, i32 num, i32 size, i32 base, i32 precision, i32 type )
{

  const char *alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char nbuf[64]; // Number buffer

  if ( base < 2 || base > 36 )
    return NULL;


  if ( type & SMALL )
    alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";

  if ( type & LEFT ) // If set left-justify, disable zeropad
    type &= ~ZEROPAD;

  char sign;
  char pad = ( type & ZEROPAD ) ? '0' : ' ';

  if ( type & SIGN && num < 0 ) {
    sign = '-';
    num = -num;
  } else
    sign = ( type & PLUS ) ? '+' : ( ( type & SPACE ) ? ' ' : '\0' );

  if ( sign )
    --size;

  if ( type & SPECIAL ) {
    if ( base == 8 )
      --size;
    else if ( base == 16 )
      size -= 2;
  }

  int i = 0;
  do {
    nbuf[i++] = alphabet[num % base];
    num /= base;
  } while ( num );

  if ( i > precision )
    precision = i;

  size -= precision;

  if ( !( type & ( LEFT | ZEROPAD ) ) )
    while ( size-- > 0 )
      *str++ = ' ';


  /* Before writing nbuf, pad everything properly */
  if ( sign )
    *str++ = sign;

  if ( type & SPECIAL ) {
    if ( base == 8 )
      *str++ = '0';
    else if ( base == 16 ) {
      *str++ = '0';
      *str++ = alphabet[33]; // 'x' or 'X'
    }
  }

  if ( !( type & LEFT ) ) // If right-justify pad with 0s or spaces
    while ( size-- > 0 )
      *str++ = pad;


  while ( i > precision-- ) // Pad with 0s
    *str++ = '0';

  while ( i-- > 0 ) // Copy nbuf into str
    *str++ = nbuf[i];

  while ( size-- > 0 ) // Left-justify
    *str++ = ' ';

  return str;
}


i32 kvsprintf( char *buf, const char *fmt, kva_list args )
{
  char *str = buf;
  for ( ; *fmt; ++fmt ) {
    if ( *fmt != '%' ) {
      *str++ = *fmt;
      continue;
    }

    ++fmt; // Ignore the first '%'
    int type = 0;
    for ( ;; ++fmt ) {
      switch ( *fmt ) {
        case '-':
          type |= LEFT;
          continue;
        case '+':
          type |= PLUS;
          continue;
        case '#':
          type |= SPECIAL;
          continue;
        case ' ':
          type |= SPACE;
          continue;
        case '0':
          type |= ZEROPAD;
          continue;
      }
      break;
    }

    int width = 0;
    if ( is_digit( *fmt ) )
      width = skip_atoi( &fmt );

    else if ( *fmt == '*' ) {
      width = kva_arg( args, int );
      if ( width < 0 ) {
        width = -width;
        type |= LEFT;
        ++fmt;
      }
    }

    int precision = 0;
    if ( *fmt == '.' ) {
      ++fmt;
      if ( is_digit( *fmt ) )
        precision = skip_atoi( &fmt );
      else if ( *fmt == '*' ) {
        ++fmt;
        precision = kva_arg( args, int );
        if ( precision < 0 )
          precision = 0;
      }
    }

    // Ignore
    if ( *fmt == 'h' || *fmt == 'L' || *fmt == 'l' )
      ++fmt;

    switch ( *fmt ) {
      case 'c':
        if ( !( type & LEFT ) ) // if right-justify
          while ( --width > 0 )
            *str++ = ' ';
        *str++ = kva_arg( args, char );
        while ( --width > 0 ) // left-justify
          *str++ = ' ';
        break;
      case 's': {
        const char *tmp = kva_arg( args, const char * );
        i32 len = kstrlen( tmp );

        if ( len > precision )
          len = precision;

        if ( !( type & LEFT ) ) { // right-justify
          while ( len < width-- )
            *str++ = ' ';
          kstrcpy( str, tmp );
          str += len + 1;
        }

        while ( len < width-- ) // left-justify
          *str++ = ' ';

      } break;
      case 'o':
        str = number( str, kva_arg( args, u32 ), width, 8, precision, type );
        break;
      case 'p':
        if ( width == 0 ) {
          width = 8;
          type |= ZEROPAD;
        }
        str = number( str, (u32)kva_arg( args, void * ), width, 16, precision,
                      type );
        break;
      case 'x':
        type |= SMALL;
      case 'X':
        str = number( str, kva_arg( args, u32 ), width, 16, precision, type );
        break;
      case 'i':
      case 'd':
        type |= SIGN;
        str = number( str, kva_arg( args, i32 ), width, 10, precision, type );
        break;
      case 'u':
        str = number( str, kva_arg( args, u32 ), width, 10, precision, type );
        break;
      case 'b':
        str = number( str, kva_arg( args, u32 ), width, 2, precision, type );
        break;
      case 'n':
        *kva_arg( args, int * ) = str - buf;
        break;
      default:
        if ( *fmt != '%' )
          *str++ = '%'; // Restore previous '%'
        if ( *fmt )
          *str++ = *fmt;
        else
          --fmt;
        break;
    }
  }
  *str = '\0';
  return str - buf;
}

i32 kprintf( const char *fmt, ... )
{
  static char buf[0x40000];
  kva_list args;
  kva_start( args, fmt );

  i32 cnt = kvsprintf( buf, fmt, args );
  if ( cnt > 0 )
    kvga_puts( buf );

  kva_end( args );

  return cnt;
}

i32 kcprintf( VgaTextAtrr bg, VgaTextAtrr fg, const char *fmt, ... )
{
  static char buf[0x40000];
  kva_list args;
  kva_start( args, fmt );

  i32 cnt = kvsprintf( buf, fmt, args );
  if ( cnt > 0 )
    kvga_cputs( buf, bg, fg );

  kva_end( args );

  return cnt;
}
