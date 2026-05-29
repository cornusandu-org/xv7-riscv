# List of Changes
## Boot flow
* Kernel Assertions
* Improved boot flow

## Diagnostics
* Compile-time-defined panic codes
* Runtime-defined panic codes
* More panic checks, so other CPUs also spin on panic

## Runtime primitives
* `yieldlock` lock type, usable both within early boot / interrupt context, and within system calls
* Kernel `vprintf`
* Better panic output
* More types (ex. `userptr_t`)

## New syscalls
* Basic signals (to be reworked)

## Miscallenous
* Small optimizations around spinlocks
* Switched to `-O2` optimization
