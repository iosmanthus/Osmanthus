#ifndef _KDEBUG_H_
#define _KDEBUG_H_

#ifdef NDEBUG
#define kassert( cond ) (void)0
#else
void __kassert( const char *file, int line, const char *func,
                const char *expr );

#define kassert( cond )                                                        \
  do {                                                                         \
    if ( !( cond ) ) {                                                         \
      __kassert( __FILE__, __LINE__, __func__, #cond );                        \
    }                                                                          \
  } while ( 0 )
#endif

// Print call stack trace

void kstack_trace();

// Fatal error
void kpanic( const char *msg );

#endif /* ifndef _KDEBUG_H_ */
