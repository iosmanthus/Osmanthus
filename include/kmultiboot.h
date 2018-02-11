#ifndef _KMULTIBOOT_H_
#define _KMULTIBOOT_H_

#include <ktypes.h>

typedef struct _KMultiBoot {
  u32 flags;
  u32 mem_lower;
  u32 mem_upper;

  u32 boot_device;
  u32 cmdline;
  u32 mods_count;
  u32 mods_addr;

  u32 num;
  u32 size;
  u32 addr;
  u32 shndx;

  u32 mmap_length;
  u32 mmap_addr;

  u32 drives_length;
  u32 drives_addr;
  u32 config_table;
  u32 boot_loader_name;
  u32 apm_table;
  u32 vbe_control_info;
  u32 vbe_mode_info;
  u32 vbe_mode;
  u32 vbe_interface_seg;
  u32 vbe_interface_off;
  u32 vbe_interface_len;
} __attribute__( ( packed ) ) KMultiBoot;

typedef struct _KMMapEntry {
  u32 size;
  u32 base_addr_low;
  u32 base_addr_high;
  u32 length_low;
  u32 length_high;
  u32 type;
} __attribute__( ( packed ) ) KMMapEntry;

#endif /* ifndef _KMULTIBOOT_H_ */