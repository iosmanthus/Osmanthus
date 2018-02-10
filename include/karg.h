#ifndef _KARG_H_
#define _KARG_H_

// Various arguments macro for i386

#include <ktypes.h>

typedef char *kva_list;

#define kva_start( ap, last )                                                  \
  ( ap = ( kva_list )( ( (kva_list)&last ) + ( ( sizeof( last ) + 3 ) & ~3 ) ) )

#define kva_arg( ap, type )                                                    \
  ( *(type *)( ap += ( ( sizeof( type ) + 3 ) & ~3 ),                          \
               ap - ( ( sizeof( type ) + 3 ) & ~3 ) ) )

#define kva_copy( dst, src ) ( dst ) = ( src )

#define kva_end( ap ) ( ap = (kva_list)NULL )

#endif /* ifndef _KARG_H_ */
