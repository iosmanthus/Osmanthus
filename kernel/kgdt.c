#include <kgdt.h>
#include <kio.h>

#define GDT_ENTRY_CNT 5
#define GDT_ENTRY_SIZE 8

#define NULL_SEG_INDEX 0
#define CODE_SEG_INDEX 1
#define DATA_SEG_INDEX 2
#define USER_CODE_SEG_INDEX 3
#define USER_DATA_SEG_INDEX 4

#define GDT_CODE_SELECTOR ( GDT_ENTRY_SIZE * CODE_SEG_INDEX )
#define GDT_DATA_SELECTOR ( GDT_ENTRY_SIZE * DATA_SEG_INDEX )
#define GDT_USER_CODE_SELECTOR ( GDT_ENTRY_SIZE * USER_CODE_SEG_INDEX )
#define GDT_USER_DATA_SELECTOR ( GDT_ENTRY_SIZE * USER_DATA_SEG_INDEX )

typedef KSegmentDescritor KGdt[GDT_ENTRY_CNT];

static KGdt gdt;

static void set_segment_descriptor( KSegmentDescritor *sd, u32 base, u32 limit,
                                    u8 access, u8 granularity );

void set_segment_descriptor( KSegmentDescritor *sd, u32 base, u32 limit,
                             u8 access, u8 granularity )
{
  sd->limit_low = limit & 0xffff;
  sd->base_low = base & 0xffff;
  sd->base_mid = ( base >> 16 ) & 0xff;
  sd->access = access;
  sd->granularity = ( granularity & 0xf0 ) | ( ( limit >> 16 ) & 0xf );
  sd->base_high = base >> 24;
}

KGDTPtr kinit_gdt()
{
  KGDTPtr gdt_ptr;
  gdt_ptr.addr = (u32)&gdt;
  gdt_ptr.limit = sizeof( gdt );

  kprintf( "%d %d\n", sizeof( gdt ), gdt_ptr.limit );

  // Null descritor
  set_segment_descriptor( &gdt[NULL_SEG_INDEX], 0x00000000, 0x00000000, 0x00,
                          0x00 );

  // Code segment descritor
  set_segment_descriptor( &gdt[CODE_SEG_INDEX], 0x00000000, 0xffffffff, 0x9a,
                          0xcf );

  // Data segment descritor
  set_segment_descriptor( &gdt[DATA_SEG_INDEX], 0x00000000, 0xffffffff, 0x92,
                          0xcf );

  // User code segment descritor
  set_segment_descriptor( &gdt[USER_CODE_SEG_INDEX], 0x00000000, 0xffffffff,
                          0xfa, 0xcf );

  // User data segment descritor
  set_segment_descriptor( &gdt[USER_DATA_SEG_INDEX], 0x00000000, 0xffffffff,
                          0xf2, 0xcf );

  return gdt_ptr;
}
