#include <kport.h>


void kport_out( u16 port, u32 data, u8 size )
{
  if ( size == BYTE )
    __asm__ volatile( "outb %1,%0" ::"d"( port ), "a"( data ) );
  else if ( size == WORD )
    __asm__ volatile( "outw %1,%0" ::"d"( port ), "a"( data ) );
  else if ( size == DWORD )
    __asm__ volatile( "outl %1,%0" ::"d"( port ), "a"( data ) );
}

u32 kport_in( u16 port, u8 size )
{
  u8 db = 0;
  u16 dw = 0;
  u32 dl = 0;
  if ( size == BYTE ) {
    __asm__ volatile( "inb %1, %0" : "=a"( db ) : "d"( port ) );
    return db;
  } else if ( size == WORD ) {
    __asm__ volatile( "inw %1, %0" : "=a"( dw ) : "d"( port ) );
    return dw;
  } else if ( size == DWORD ) {
    __asm__ volatile( "inl %1, %0" : "=a"( dl ) : "d"( port ) );
    return dl;
  }
  return 0;
}
