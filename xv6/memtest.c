// Assignment 10, Q1(c): user program that exercises kalloc()/kfree()
// through sbrk() and shows the memstats() counters before, after
// allocation and after release.
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int npages = 10;
  char *p;
  int i;

  printf(1, "memtest: initial memory statistics\n");
  memstats();

  p = sbrk(npages * 4096);
  if(p == (char*)-1){
    printf(1, "memtest: sbrk failed\n");
    exit();
  }
  // touch every page so the kernel really allocates physical frames
  for(i = 0; i < npages; i++)
    p[i * 4096] = 1;

  printf(1, "memtest: allocated %d pages with sbrk()\n", npages);
  memstats();

  sbrk(-npages * 4096);
  printf(1, "memtest: released %d pages\n", npages);
  memstats();

  exit();
}
