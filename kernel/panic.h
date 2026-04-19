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
    UTRAP_NOTUMODE
};

extern const char* panic_gettext(int);

static inline const char* paniccode_tostr(int code) {
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
