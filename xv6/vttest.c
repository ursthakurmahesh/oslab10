// Assignment 10, Q2(c): user program that allocates memory with sbrk()
// and translates several virtual addresses to physical addresses with
// the vtop() system call, including an unmapped address.
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int npages = 4;
  char *p;
  int i;
  int addrs[3];
  int bad, pa;

  p = sbrk(npages * 4096);
  if(p == (char*)-1){
    printf(1, "vttest: sbrk failed\n");
    exit();
  }
  // touch the pages so they get real physical frames
  for(i = 0; i < npages; i++)
    p[i * 4096] = 1;

  printf(1, "vttest: translating virtual addresses of the sbrk() block\n");
  addrs[0] = (int)p;                 // start of block
  addrs[1] = (int)p + 4096;          // second page of block
  addrs[2] = (int)p + 2500;          // offset inside first page
  for(i = 0; i < 3; i++){
    pa = vtop(addrs[i]);
    if(pa < 0)
      printf(1, "vttest: va 0x%x -> not mapped\n", addrs[i]);
    else
      printf(1, "vttest: va 0x%x -> pa 0x%x\n", addrs[i], pa);
  }

  // invalid / unmapped virtual address (far above this process's memory)
  bad = 0x40000000;
  pa = vtop(bad);
  if(pa < 0)
    printf(1, "vttest: va 0x%x -> not mapped (vtop returned %d)\n", bad, pa);
  else
    printf(1, "vttest: va 0x%x -> pa 0x%x\n", bad, pa);

  exit();
}
