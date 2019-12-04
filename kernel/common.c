// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "common.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from src to dest.
void memcpy(void *dest, const void *src, u32int len)
{
    u8int* tdest = dest;
    const u8int* tsrc = src;
    if (len <= 0) {
        return;
    }

    while(len--) {
        *(tdest++) = *(tsrc++);
    }
}

// Write len copies of val into dest.
void memset(void *dest, u8int val, u32int len)
{
    u8int* tmp = dest;
    if (len <= 0) {
        return;
    }

    while(len--) {
        *(tmp++) = val;
    }
}

// Write len copies of val into dest.
void bzero(void *dest, u32int len)
{
   memset(dest, 0, len);
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
    // TODO: implement this yourself!
    return 0;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    // TODO: implement this yourself!
    return 0;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    // TODO: implement this yourself!
    return 0;
}

s32int strlen(const char* src)
{
    const char *eos = src; 

    while (*eos++)     
        ;                

    return (eos - src - 1);
}
