#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include "stddef.h"
#include "log.h"
#include "fs.h"
#include "panic.h"
#include "yieldlock.h"

struct yieldlock assertlocks;

#define ALIGNOF(type) offsetof(struct { char c; type t; }, t)

#define SATP_MODE(satp) ((satp) >> 60)

extern volatile int panicked;
extern volatile int panicking;

static void
ASSERT(unsigned char value, const char* const msg)
{
  //printf("kasserts >> run_asserts >> ASSERT()    >> Running condition: %s", msg);
  if (value != 1) {
    printf("panic: kasserts >> run_asserts >> ASSERT() >> Condition failed: %s", msg);
    panic(KASSERT_FAILED_ASSERTION, "kasserts >> ASSERT(false)");
  }
}

static void
OP_ASSERT(unsigned char value, const char* const msg)
{
  //printf("kasserts >> run_asserts >> OP_ASSERT() >> Running condition: %s\n", msg);
  if (value != 1) {
    printf("kasserts >> run_asserts >> OP_ASSERT() >> Condition failed: %s; resuming\n", msg);
  }
}

static void
L_ASSERT(unsigned char value, const char* const msg, char enabled)
{
  //printf("kasserts >> late_asserts >> L_ASSERT(cpu=%d) >> Running condition: %s\n", cpuid(), msg);
  if (value != 1 && enabled == 1) {
    printf("panic: kasserts >> late_asserts >> L_ASSERT(cpu=%d) >> Condition failed: %s\n", cpuid(), msg);
    panic(KASSERT_FAILED_LATE_ASSERTION, "kasserts >> ASSERT(false)");
  } else if (value != 1) {
    printf("kasserts >> late_asserts >> L_ASSERT(cpu=%d) >> Condition failed: %s; resuming\n", cpuid(), msg);
  }
}

void
run_asserts(void)
{
  inityield(&assertlocks, "assertlock", TRUE);

  ASSERT(cpuid()==0, "Checks running on primary hart\n");
  ASSERT(myproc()==0, "Checks not running from within syscall\n");
  ASSERT(sizeof(void*) == 8, "Machine is 64-bit\n");
  ASSERT(ALIGNOF(enum procstate) <= ALIGNOF(int), "Alignment of (enum procstate) is lower or equal to alignment of int\n");
  ASSERT(sizeof(struct logheader) < BSIZE, "Size of logheader is within bounds\n");
  ASSERT(sizeof(int) == 4, "int = int32\n");
  ASSERT(ALIGNOF(int) == ALIGNOF(uint32), "(alignment) int = int32\n");
  ASSERT(PGSIZE >= 4096, "Page size is bigger or equal to 4KB\n");
  uint64 satp = r_satp();
  ASSERT(satp == 0, "Paging not yet initialized\n");
  ASSERT(NPROC >= 2, "Number of processes is at least 2, to ensure xv6 is usable\n");
  ASSERT(NFILE > 1, "Atleast 2 files can be opened\n");
  ASSERT(NOFILE >= 1, "Atleast 1 file can be opened per process\n");

  // Optional assertions that do not panic
  OP_ASSERT(sizeof(void) == 1, "Void type takes one byte");
}

void
late_asserts(void)
{
  acquireyield(&assertlocks);

  uint64 satp = r_satp();
  L_ASSERT(satp != 0, "Paging initialized", 1);
  L_ASSERT(myproc()==0, "Checks not running from within syscall", 1);
  L_ASSERT(SATP_MODE(r_satp()) == 8, "Paging mode is Sv39", 1);

  releaseyield(&assertlocks);
}
