#include "irq.h"
#include "defs.h"
#include "proc.h"

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

void KeIrqlGTE(interrupt_level_t min) {

}

void            KeIrqLTE(interrupt_level_t max) {

}

void            KeSetIrqContext(interrupt_source_t src) {

}

// Taking a break here, because I am super pissed off by autocomplete not working properly
