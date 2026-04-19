#include "param.h"
#include "types.h"
#include "spinlock.h"
#include "yieldlock.h"
#include "riscv.h"
#include "proc.h"
#include "memlayout.h"
#include "defs.h"
#include "panic.h"

#define SPIN_TIME 10  // ~1 microsecond if 10MHz

static inline uint me() {
    if (myproc() != NULL)
        return NCPU + myproc()->pid;
    
    return cpuid();
}

int holdingyield(struct yieldlock* lock) {
    return (lock->locked == TRUE) && (lock->owner == me());
}

void inityield(struct yieldlock* lock, char* name, uint irqsafe) {
    lock->name = name;
    lock->owner = 0;
    lock->locked = FALSE;
    lock->irqsafe = irqsafe;
    initlock(&lock->lk, name);
}

void acquireyield(struct yieldlock* lock) {
    if (holdingyield(lock))
        panic(UNKNOWN_FAILURE, "acquireyield: already holding");

    if (lock->irqsafe && myproc() == NULL)
        push_off();

    if (mycpu()->inintr != 0 && lock->owner > NCPU)
        panic(UNKNOWN_FAILURE, "acquireyield: acquired from interrupt handler while being held by a process");

    if (myproc() == NULL) {
        while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
            ;
    } else {
        // Spin for a bit before falling back to sleeping, for performance benefits
        uint64 start = r_time();
        
        acquire(&lock->lk);

        while (r_time() - start < SPIN_TIME) {
            if (__sync_lock_test_and_set(&lock->locked, 1) == 0)
                goto goto_acquired;
            __sync_synchronize();
        }

        while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
            sleep(lock, &lock->lk);
        release(&lock->lk);
    }

    goto_acquired:

    __sync_synchronize();

    lock->owner = me();
}

void releaseyield(struct yieldlock* lock) {
    if (!holdingyield(lock))
        panic(UNKNOWN_FAILURE, "releaseyield: cant release (not holding)");

    acquire(&lock->lk);

    lock->owner = 0;

    __sync_synchronize();

    __sync_lock_release(&lock->locked);

    wakeup(lock);

    release(&lock->lk);

    if (lock->irqsafe && myproc() == NULL)
        pop_off();
}
