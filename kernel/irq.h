#ifndef xv7_IRQ_H
#define xv7_IRQ_H

#include "types.h"
#include "proc.h"

typedef enum {
    IRQSRC_EIRQ,  // IRQ Source: External Interrupt Requests
    IRQSRC_IIRQ,  // IRQ Source: Internal Interrupt Requests
    IRQSRC_KII,   // IRQ Source: Kernel-issued Interrupts
    IRQSRC_MISC   // IRQ Source: Miscallenous / Unknown
} interrupt_source_t;

typedef enum {
    IRQL_NONE = 0x0,
    IRQL_GENERAL = 0x1,   // general kernel execution (ex. system calls)
    IRQL_DEVICE,    // timer interrupts, UART
    IRQL_FULLMASK,  // spinlocks
    IRQL_SPECIAL
} interrupt_level_t;

typedef struct {
    interrupt_source_t source;
    interrupt_level_t level;
} icl_t;

#endif
