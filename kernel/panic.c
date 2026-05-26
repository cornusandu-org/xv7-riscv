#include "types.h"
#include "riscv.h"
#include "panic.h"
#include "spinlock.h"
#include "defs.h"

static const char* const panic_messages[] = {
    [SPINLOCK_REACQ] = "The kernel attempted to reacquire a resource it already held. To avoid deadlock, the system has to halt.",
    [SPINLOCK_NOTYOURS_RELEASE] = "The kernel attempted to release a lock it doesn't hold. This can hint at a corrupted kernel or bad data. To avoid further data corruption, the system has to halt.",
    [MMFREE_UNALIGNEDPAGE] = "The kernel attempted to free a block of memory that isn't where it should be and crosses the boundary between two real blocks of memory.",
    [MMFREE_FREEKERNEL] = "The kernel attempted to free memory that holds kernel code or critical kernel data. To avoid data corruption, the system has to halt.",
    [MMFREE_NONPHYSIC] = "The kernel attempted to free memory that doesn't exist.",
    [SCHED_FAILSCHEDINIT] = "The kernel failed to create the init process. As the system is otherwise unusable, it has to halt.",
    [PROC_CRITICAL_PROCESS_DIED__INIT] = "The init process has died. To avoid data corruption or an otherwise invalid system state, the kernel has to halt.",
    [UNKNOWN_FAILURE] = "An unknown error occured. This error is either unexpected or uncategorised.",
    [KASSERT_FAILED_ASSERTION] = "The current environment/system was deemed unfit for execution. As such, the kernel has to halt.",
    [KASSERT_FAILED_LATE_ASSERTION] = "Something went wrong during system initialisation, and the kernel has to halt.",
    [CPU_POPOFF_UNDERFLOW_INTRON] = "Attempted to restore interrupt state on a CPU core that already has interrupts enabled. To avoid data corruption or undefined behaviour, the kernel has to halt.",
    [CPU_POPOFF_UNDERFLOW] = "Attempted to restore interrupt state on a CPU core with no stored previous interrupt state. To avoid data corruption or undefined behaviour, the kernel has to halt.",
    [ARGRAW_OUTOFBOUNDS] = "The kernel attempted to access a syscall argument that doesn't exist. To avoid undefined behaviour or data corruption, the kernel has to halt.",
    [LOADSEG_NOVALIDMAPPING] = "The kernel attempted to load an ELF binary without preallocating memory.",
    [VIRTO_CONFIGFEAT_FAIL] = "The virtio driver and disk failed to agree on a compatible feature set.",
    [VIRTIO_READYTOOEARLY] = "The VirtIO disk was ready before it should've been.",
    [KTRAP_INTRENABLED] = "kerneltrap() was reached while interrupts were enabled.",
    [KTRAP_NOTSMODE] = "kerneltrap() was reached outside of Supervisor mode.",
    [KTRAP_UNKNOWNSOURCE] = "kerneltrap() was reached from an unknown source. See logs above for more details.",
    [UTRAP_NOTUMODE] = "usertrap() was reached outside of User mode."
};

#define CUSTOM_CODES_NO 4096
struct custom_panic_code_t custom_codes[CUSTOM_CODES_NO] = {};
int custom_codes_index = 0;
struct spinlock custom_codes_lock;

void init() {
    initlock(&custom_codes_lock, "custom_panic_codes_lock");
}

struct custom_panic_code_t ADD_PANIC_CODE(char name[], const char* msg) {
    if (custom_codes_index >= CUSTOM_CODES_NO)
        panic(BUFFEROVERFLOW, "ADD_PANIC_CODE: custom_codes_index >= CUSTOM_CODES_NO");
    acquire(&custom_codes_lock);
    int code = ___PANIC_ENUM_END + custom_codes_index;
    struct custom_panic_code_t c;
    c.code = code;
    c.name = name;
    c.msg = msg;
    custom_codes[custom_codes_index] = c;
    custom_codes_index++;
    release(&custom_codes_lock);
    return custom_codes[custom_codes_index];
}

const char* panic_gettext(int panic_code) {
    for (int i = 0; i < custom_codes_index; i++) {
        if (custom_codes[i].code == panic_code)
            return custom_codes[i].msg;
    }
    return panic_messages[panic_code];
}

const char* paniccode_tostr(int code) {
    if (code >= ___PANIC_ENUM_END) {
        if (code >= ___PANIC_ENUM_END + custom_codes_index) {
            return "(Missing error message for invalid code -- too high)";
        }
        return custom_codes[code - ___PANIC_ENUM_END].name;
    }

    switch (code) {
        case SPINLOCK_REACQ: return                     "      SPINLOCK_REACQ";
        case MMFREE_UNALIGNEDPAGE: return               "    MMFREE_UNALIGNEDPAGE";
        case MMFREE_FREEKERNEL: return                  "      MMFREE_FREEKERNEL";
        case MMFREE_NONPHYSIC: return                   "      MMFREE_NONPHYSIC";
        case SPINLOCK_NOTYOURS_RELEASE: return          "  SPINLOCK_NOTYOURS_RELEASE";
        case SCHED_FAILSCHEDINIT: return                "     SCHED_FAILSCHEDINIT";
        case PROC_CRITICAL_PROCESS_DIED__INIT: return   "PROC_CRITICAL_PROCESS_DIED__INIT";
        case KASSERT_FAILED_ASSERTION: return           "   KASSERT_FAILED_ASSERTION";
        case KASSERT_FAILED_LATE_ASSERTION: return      " KASSERT_FAILED_LATE_ASSERTION";
        case CPU_POPOFF_UNDERFLOW_INTRON: return        "  CPU_POPOFF_UNDERFLOW_INTRON";
        case CPU_POPOFF_UNDERFLOW: return               "    CPU_POPOFF_UNDERFLOW";
        case ARGRAW_OUTOFBOUNDS: return                 "     ARGRAW_OUTOFBOUNDS";
        case LOADSEG_NOVALIDMAPPING: return             "    LOADSEG_NOVALIDMAPPING";
        case VIRTO_CONFIGFEAT_FAIL: return              "     VIRTO_CONFIGFEAT_FAIL";
        case VIRTIO_READYTOOEARLY: return               "     VIRTIO_READYTOOEARLY";
        case KTRAP_INTRENABLED: return                  "      KTRAP_INTRENABLED";
        case KTRAP_NOTSMODE: return                     "       KTRAP_NOTSMODE";
        case KTRAP_UNKNOWNSOURCE: return                "      KTRAP_UNKNOWNSOURCE";
        case UTRAP_NOTUMODE: return                     "       UTRAP_NOTUMODE";

        default: return                                 "       UNKNOWN_FAILURE";
    };
}

int get_paniccode_from_custom(char name[]) {
    acquire(&custom_codes_lock);
    for (int i = 0; i < custom_codes_index; i++) {
        if (strncmp(custom_codes[i].name, name, 200) == 0) {
            release(&custom_codes_lock);
            return custom_codes[i].code;
        }
    }
    release(&custom_codes_lock);
    return UNKNOWN_FAILURE;
}
