#include "panic.h"

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

const char* panic_gettext(int panic_code) {
    return panic_messages[panic_code];
}
