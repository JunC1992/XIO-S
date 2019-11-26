// console.h -- Defines the interface for console.h
//              From JamesM's kernel development tutorials.

#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h"

// Write a single character out to the screen.
void console_put(char c);

// Clear the screen to all black.
void console_clear();

// Output a null-terminated ASCII string to the console.
void console_write(char *c);

void console_write_hex(u32int n);

void console_write_dec(u32int n);

#endif // CONSOLE_H
