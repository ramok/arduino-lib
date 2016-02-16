#ifndef CPPFIX_H
#define CPPFIX_H

#include <stdlib.h>

// from http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=410870
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

void * operator new(size_t size);
void operator delete(void * ptr);

#endif

