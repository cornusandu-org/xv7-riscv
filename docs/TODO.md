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
* [ ] Make a build flag to replace the `wfi`-loop with a plain spinloop
