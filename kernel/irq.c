#include "irq.h"
#include "defs.h"
#include "proc.h"
#include "panic.h"

void KeRaiseIrql(interrupt_level_t irql) {
    struct cpu* c = mycpu();
    icl_t entry;
    if (irql < c->irqstack[c->irqstack_index].level)
        irql = c->irqstack[c->irqstack_index].level;
    entry.level = irql;
    entry.source = c->irqstack[c->irqstack_index].source;
    c->irqstack[c->irqstack_index++] = entry;
}

void KeLowerIrql(interrupt_level_t irql) {
    struct cpu* c = mycpu();
    icl_t entry;
    if (irql > c->irqstack[c->irqstack_index].level)
        irql = c->irqstack[c->irqstack_index].level;
    entry.level = irql;
    entry.source = c->irqstack[c->irqstack_index].source;
    c->irqstack[c->irqstack_index++] = entry;
}

static icl_t GetIRQ() {
    struct cpu* c = mycpu();
    return c->irqstack[c->irqstack_index];
}

static int GetIRQL() {
    icl_t icl = GetIRQ();
    auto int irql = (int)icl.level;
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
    icl_t* icl = &c->irqstack[c->irqstack_index];
    icl->source = src;
}
