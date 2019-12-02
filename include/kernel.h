#ifndef KERNEL_H
#define KERNEL_H

#include "vargs.h"

int printk(const char* fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);

#endif
