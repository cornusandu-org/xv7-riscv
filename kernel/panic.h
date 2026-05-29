#include "param.h"

enum PanicCodes : int {
    UNKNOWN_FAILURE,
    SPINLOCK_REACQ,
    SPINLOCK_NOTYOURS_RELEASE,
    MMFREE_UNALIGNEDPAGE,
    MMFREE_FREEKERNEL,
    MMFREE_NONPHYSIC,
    SCHED_FAILSCHEDINIT,
    PROC_CRITICAL_PROCESS_DIED__INIT,
    KASSERT_FAILED_ASSERTION,
    KASSERT_FAILED_LATE_ASSERTION,
    CPU_POPOFF_UNDERFLOW_INTRON,
    CPU_POPOFF_UNDERFLOW,
    ARGRAW_OUTOFBOUNDS,
    LOADSEG_NOVALIDMAPPING,
    VIRTO_CONFIGFEAT_FAIL,
    VIRTIO_READYTOOEARLY,
    KTRAP_INTRENABLED,
    KTRAP_NOTSMODE,
    KTRAP_UNKNOWNSOURCE,
    UTRAP_NOTUMODE,
    BUFFEROVERFLOW,

    ___PANIC_ENUM_END
};
struct custom_panic_code_t {
    int code;
    char *name;  // should be const, but eh
    const char *msg;
};

#define CUSTOM_CODES_NO 4096
extern struct custom_panic_code_t custom_codes[CUSTOM_CODES_NO];

extern const char* panic_gettext(int);

const char* paniccode_tostr(int code);

struct custom_panic_code_t ADD_PANIC_CODE(char code[], const char* msg);

int get_paniccode_from_custom(char name[]);


__attribute__((noreturn)) inline static void _panic_spin() {
    __asm__ volatile("fence iorw, iorw" :::"memory");
    for(;;)
        #if DEBUG_PANIC_NOWFI == 0
        __asm__ volatile("wfi" :::"memory");
        #else
        __asm__ volatile("fence iorw, iorw" :::"memory");
        #endif
}
