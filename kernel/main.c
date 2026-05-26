#include "types.h"
#include "param.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"

volatile static int started = 0;
int hart_started[NPROC];

int step_no = 0;
const int total_steps = 15;
void init_step(void) {
  printf("[%d/%d]\r", step_no+1, total_steps);
  step_no++;
}
void
init_early(void)
{
  consoleinit();  init_step();
  printfinit();  init_step();
  memset((void*)hart_started, 0, NPROC * sizeof(int));  init_step();
  printf("\n");
  printf("init_early() called\nEntered early state.\n");
  run_asserts();  init_step();
}

void
init_hardware(void)
{
  printf("init_hardware: Initialise RAM\n");

  kinit();         init_step();  // physical page allocator       
  kvminit();       init_step();  // create kernel page table    
  kvminithart();   init_step();  // turn on paging

  printf("init_hardware: Initialise processes\n");

  procinit();      init_step();  // process table

  printf("init_hardware: Initialise interrupts\n");

  trapinit();      init_step();  // trap vectors
  trapinithart();  init_step();  // install kernel trap vector
  plicinit();      init_step();  // set up interrupt controller
  plicinithart();  init_step();  // ask PLIC for device interrupts

  printf("init_hardware: Initialise file system\n");

  binit();         init_step();  // buffer cache
  iinit();         init_step();  // inode table
  fileinit();      init_step();  // file table
  printf("\n");
}

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    init_early();
    printf("xv6 kernel is booting\n");
    init_hardware();
    printf("initialized hardware state\n");
    virtio_disk_init(); // emulated hard disk
    printf("initialised emulated hard disk\n");
    hart_started[0] = 1;

    late_asserts();

    printf("entering userinit()\n");
    userinit();      // first user process
    printf("exiting userinit()\n");
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
    hart_started[cpuid()] = 1;
    late_asserts();
  }

  scheduler();        
}