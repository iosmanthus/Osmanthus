# Osmanthus

Osmanthus is a developing ~~almost~~ unix-like toy kernel.

Here are the features that has been implemented (or not) :

- [x] Boot with GRUB
- [x] Basic VGA driver
- [x] Format printing functions
- [x] Kernel debug functions
- [x] Interrupt handling library (partially) 
- [x] Enable paging
- [x] Kernel heap allocator
- [x] Kernel level multithreading
  - [x] Create thread via `kthread_create`
  - [x] Join thread via `kthread_join`
  - [x] Thread scheduling (simplest algorithm :joy: )
  - [x] Mutex ! (Bakery algorithm)
- [ ] Multiprocessing (Maybe we need a file system)



## Build

Requirements:

1. `GCC` >= 4.7.1
2. `GDB`
3. `yasm` 
4. `qemu-system-i386` to run the system
5. `xorriso` && `grub` && `mtools` to build ISO file

After you install the requirements, just type `make debug`, then you can 

1. `make run` to test the system directly
2. `make gdb` to debug the system with `gdb`
3. `make gdbgui` to debug the system with `gdbgui`

**IF YOU WANT TO KNOW MORE ABOUT THE PROJECT, CLICK THE FOLLOWING LINK**

[Osmanthus Wiki](https://github.com/iosmanthus/Osmanthus/wiki)