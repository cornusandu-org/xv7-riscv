# To-Do
## Signals
* [ ] Implement `sigreg` system call
* [ ] Implement `sigcall` and `sigret` system calls, with stack-based trapframe saving
* [ ] Implement kernel-provided standard signal handler

## yieldlocks
* [ ] Test yieldlocks within user contexts

## More Types
* [x] Implement `__user` macro

## Panic Codes
* [x] Runtime-defined panic codes
    * [x] Fix returning wrong struct from ADD_PANIC_CODE
* [x] Code cleanups and documentation

## Panic
* [X] Make a build flag to replace the `wfi`-loop with a plain spinloop
* [X] Use `_panic_spin()` in panick-checks in UART driver and scheduler
* [ ] Implement IPI (Inter-processor Interrups) on-panic to immediately stop all cores

## Throwable Exceptions
* [ ] Implement throwable exceptions via _cmpext_KeTry() and _cmpext_KeCatch() macros

## Interrupt Handling
* [ ] Implement IRQL system, storing `context` (IIRQ = Internal IRQ, EIRQ = External IRQ, KII = Kernel-Issued Interrupt) and `level` (`IRQL_NONE`, `IRQL_GENERAL`, `IRQL_DEVICE`, `IRQL_FULLMASK`, `IRQL_SPECIAL`)
