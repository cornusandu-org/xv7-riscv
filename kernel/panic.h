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
    CPU_POPOFF_UNDERFLOW
};

extern const char* panic_gettext(int);

static inline const char* paniccode_tostr(int code) {
    switch (code) {
        case SPINLOCK_REACQ: return                     "      SPINLOCK_REACQ";
        case MMFREE_UNALIGNEDPAGE: return               "    MMFREE_UNALIGNEDPAGE";
        case MMFREE_FREEKERNEL: return                  "      MMFREE_FREEKERNEL";
        case MMFREE_NONPHYSIC: return                   "      MMFREE_NONPHYSIC";
        case SPINLOCK_NOTYOURS_RELEASE: return          "   SPINLOCK_NOTYOURS_RELEASE";
        case SCHED_FAILSCHEDINIT: return                "     SCHED_FAILSCHEDINIT";
        case PROC_CRITICAL_PROCESS_DIED__INIT: return   "PROC_CRITICAL_PROCESS_DIED__INIT";
        case KASSERT_FAILED_ASSERTION: return           "    KASSERT_FAILED_ASSERTION";
        case KASSERT_FAILED_LATE_ASSERTION: return      "  KASSERT_FAILED_LATE_ASSERTION";
        case CPU_POPOFF_UNDERFLOW_INTRON: return        "   CPU_POPOFF_UNDERFLOW_INTRON";
        case CPU_POPOFF_UNDERFLOW: return               "      CPU_POPOFF_UNDERFLOW";
        default: return                                 "        UNKNOWN_FAILURE";
    };
}
