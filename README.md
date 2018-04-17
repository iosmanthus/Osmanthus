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

** IF YOU WANT TO KNOW MORE ABOUT THE PROJECT, CLICK THE FOLLOWING LINK **

[Osmanthus Wiki](https://github.com/iosmanthus/Osmanthus/wiki)