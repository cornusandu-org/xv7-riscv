#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "irq.h"
#include "spinlock.h"
#include "proc.h"
#include "panic.h"
#include "defs.h"

static int GetIRQL(void);

static void apply_irql(interrupt_level_t irql) {
    if (irql >= IRQL_DEVICE)
        intr_off();
    else
        intr_on();
}

interrupt_level_t KeRaiseIrql(interrupt_level_t irql) {
    struct cpu* c = mycpu();
    int irql_old = GetIRQL();
    KeIrqLTE(irql);
    apply_irql(irql);
    c->irq.level = irql;
    return irql_old;
}

void KeLowerIrql(interrupt_level_t irql) {
    struct cpu* c = mycpu();
    KeIrqlGTE(irql);
    apply_irql(irql);
    c->irq.level = irql;
}

static icl_t GetIRQ() {
    struct cpu* c = mycpu();
    return c->irq;
}

static int GetIRQL() {
    icl_t icl = GetIRQ();
    int irql = (int)icl.level;
    return irql;
}

void KeIrqlGTE(interrupt_level_t min) {
    register int irql = GetIRQL();
    register int target = (int)min;
    
    if (irql < target)
        panic(IRQL_NOT_GREATER_OR_EQUAL, "IRQL (found) %d not greater or equal than IRQL (expected) %d", irql, target);
}

void KeIrqLTE(interrupt_level_t max) {
    register int irql = GetIRQL();
    register int target = (int)max;
    
    if (irql > target)
        panic(IRQL_NOT_GREATER_OR_EQUAL, "IRQL (found) %d not lower or equal than IRQL (expected) %d", irql, target);
}

void KeSetIrqContext(interrupt_source_t src) {
    struct cpu* c = mycpu();
    c->irq.source = src;
}
