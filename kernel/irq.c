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
    int register irql = GetIRQL();
    int register target = (int)min;
    
    if (irql < target) {
        printf("[CRIT] IRQ: IRQL not greater or equal (%d < %d)", irql, target);
        panic(IRQL_NOT_GREATER_OR_EQUAL, "");
    }
}

void KeIrqLTE(interrupt_level_t max) {

}

void KeSetIrqContext(interrupt_source_t src) {

}

// Taking a break here, because I am super pissed off by autocomplete not working properly
