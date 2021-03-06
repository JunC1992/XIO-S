// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

#define NULL 0

typedef u32int pid_t;

// vga
void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

// mem
void memcpy(void *dest, const void *src, u32int len);
void memset(void *dest, u8int val, u32int len);
void bzero(void *dest, u32int len);

// strings
s32int strlen(const char* src);

#endif // COMMON_H
