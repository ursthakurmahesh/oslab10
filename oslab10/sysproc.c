#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// Assignment 10, Q1(a): allocation/free counters maintained in kalloc.c
extern int kpage_allocs;
extern int kpage_frees;
// first address after the kernel image; defined by kernel.ld
extern char end[];

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Assignment 10, Q1(b): display physical-memory statistics gathered by
// the kalloc()/kfree() counters in kalloc.c.
int
sys_memstats(void)
{
  int total_alloc = kpage_allocs;              // total allocated pages (cumulative)
  int total_free  = kpage_frees;               // total freed pages (cumulative)
  int current     = total_alloc - total_free;  // pages in use right now
  // frames managed by the page allocator: from end of kernel to PHYSTOP
  int total_frames = (PHYSTOP - V2P((uint)end)) / PGSIZE;
  int available    = kfree_pages();            // exact free-list length (kalloc.c)

  cprintf("memstats: total allocated pages = %d\n", total_alloc);
  cprintf("memstats: total freed pages = %d\n", total_free);
  cprintf("memstats: currently allocated pages = %d\n", current);
  cprintf("memstats: available physical memory pages = %d (of %d total frames)\n",
          available, total_frames);
  return 0;
}

// Assignment 10, Q2(a): syscall wrapper - arg is the virtual address.
int
sys_vtop(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return vtop((uint)va);
}

// Assignment 10, Q3(a): syscall wrapper - arg is the virtual address.
int
sys_pageinfo(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return pageinfo((uint)va);
}

// Assignment 10, Q3(b): syscall wrapper - arg is the virtual address.
int
sys_setreadonly(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return setreadonly((uint)va);
}
