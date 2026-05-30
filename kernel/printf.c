//
// formatted console output -- printf, panic.
//

#include <stdarg.h>
#include <stdalign.h>

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "yieldlock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "panic.h"
#include "proc.h"

volatile int panicking = 0; // printing a panic message
volatile int panicked = 0; // spinning forever at end of a panic

// lock to avoid interleaving concurrent printf's.
static struct {
  struct yieldlock lock;
} pr;

static char digits[] = "0123456789abcdef";

static void
printint(long long xx, int base, int sign)
{
  char buf[20];
  int i;
  unsigned long long x;

  if(sign && (sign = (xx < 0)))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    consputc(buf[i]);
}

static void
printptr(uint64 x)
{
  int i;
  consputc('0');
  consputc('x');
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

volatile int panic_cpu = 0;

__attribute__((aligned(64)))
void
vprintf(const char *fmt, va_list ap) {
    int i, cx, c0, c1, c2;
    char *s;

    if(panicking == 0)
        acquireyield(&pr.lock);

    if (panicked==1) {
      intr_off();
      for (;;)
        ;
    }

    if (panicking == 1 && panic_cpu != cpuid()) {
      intr_off();
      for (;;)
        ;
    }

    for(i = 0; (cx = fmt[i] & 0xff) != 0; i++){
        if (panicked==1) {
          for (;;)
            ;
        }
        if(cx != '%'){
          consputc(cx);
          continue;
        }
        i++;
        c0 = fmt[i+0] & 0xff;
        c1 = c2 = 0;
        if(c0) c1 = fmt[i+1] & 0xff;
        if(c1) c2 = fmt[i+2] & 0xff;
        if(c0 == 'd'){
          printint(va_arg(ap, int), 10, 1);
        } else if(c0 == 'l' && c1 == 'd'){
          printint(va_arg(ap, uint64), 10, 1);
          i += 1;
        } else if(c0 == 'l' && c1 == 'l' && c2 == 'd'){
          printint(va_arg(ap, uint64), 10, 1);
          i += 2;
        } else if(c0 == 'u'){
          printint(va_arg(ap, uint32), 10, 0);
        } else if(c0 == 'l' && c1 == 'u'){
          printint(va_arg(ap, uint64), 10, 0);
          i += 1;
        } else if(c0 == 'l' && c1 == 'l' && c2 == 'u'){
          printint(va_arg(ap, uint64), 10, 0);
          i += 2;
        } else if(c0 == 'x'){
          printint(va_arg(ap, uint32), 16, 0);
        } else if(c0 == 'l' && c1 == 'x'){
          printint(va_arg(ap, uint64), 16, 0);
          i += 1;
        } else if(c0 == 'l' && c1 == 'l' && c2 == 'x'){
          printint(va_arg(ap, uint64), 16, 0);
          i += 2;
        } else if(c0 == 'p'){
          printptr(va_arg(ap, uint64));
        } else if(c0 == 'c'){
          consputc(va_arg(ap, uint));
        } else if(c0 == 's'){
          if((s = va_arg(ap, char*)) == 0)
            s = "(null)";
          for(; *s; s++)
            consputc(*s);
        } else if(c0 == '%'){
          consputc('%');
        } else if(c0 == 0){
          break;
        } else {
          // Print unknown % sequence to draw attention.
          consputc('%');
          consputc(c0);
        }
    }

    if(panicking == 0)
        releaseyield(&pr.lock);
}

// Print to the console.
__attribute__((aligned(64)))
int
printf(char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);

  return 0;
}

void
panic(int code, char *s, ...)
{
  intr_off();
  if(__sync_lock_test_and_set(&panicking, 1) != 0)
    _panic_spin();

  panic_cpu = cpuid();
  panicking = 1;                        __sync_synchronize();
  char buf[100];                        __sync_synchronize();
  memset(buf, '\n', sizeof(buf) - 1);   __sync_synchronize();
  buf[99] = 0;                          __sync_synchronize();
  printf("%s\t\t\t\t\t=== Kernel Panic ===\n\n\n\t\t\t\t%s (0x%x)", buf, paniccode_tostr(code), code);      __sync_synchronize();
  printf("\n\n\n\n%s", panic_gettext(code));                                                                __sync_synchronize();
  printf("\n\n\n\n\n\n\n\n\nCPU: %d\nPanic message:\n```\n", cpuid());                                     __sync_synchronize();
  
  va_list ap;
  va_start(ap, s);

  vprintf(s, ap);

  va_end(ap);

  printf("\n```\n");

  panicked = 1; /* freeze uart output from other CPUs */                                                    __sync_synchronize();                                
  _panic_spin();
}

void StateCheck(char name[], char *s, ...)
{
  int code = get_paniccode_from_custom(name);
  intr_off();
  if(__sync_lock_test_and_set(&panicking, 1) != 0)
    _panic_spin();

  panic_cpu = cpuid();
  panicking = 1;                        __sync_synchronize();
  char buf[100];                        __sync_synchronize();
  memset(buf, '\n', sizeof(buf) - 1);   __sync_synchronize();
  buf[99] = 0;                          __sync_synchronize();
  printf("%s\t\t\t\t\t=== Kernel Panic ===\n\n\n\t\t\t\t%s (0x%x)", buf, name, code);      __sync_synchronize();
  printf("\n\n\n\n%s", panic_gettext(code));                                                                __sync_synchronize();
  printf("\n\n\n\n\n\n\n\n\nCPU: %d\nPanic message:\n```\n", cpuid());                                      __sync_synchronize();
  
  va_list ap;
  va_start(ap, s);

  vprintf(s, ap);

  va_end(ap);

  printf("\n```\n");

  panicked = 1; /* freeze uart output from other CPUs */                                                    __sync_synchronize();                                
  _panic_spin();
}

void
printfinit(void)
{
  inityield(&pr.lock, "pr", TRUE);
}
