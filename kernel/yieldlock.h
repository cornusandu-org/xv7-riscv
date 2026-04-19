struct yieldlock {
  uint locked;       // Is the lock held?
  uint owner;   // The cpu id (if myproc() == NULL) or the pid of the process holding the lock.
  uint irqsafe;  // Whether to disable interrupts when myproc() == NULL
  struct spinlock lk;

  // For debugging:
  char *name;        // Name of lock.
} __attribute__((aligned(16)));

