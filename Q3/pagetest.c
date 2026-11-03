// Assignment 10, Q3(c): user program that allocates a page, shows its
// page-table information with pageinfo(), removes write permission with
// setreadonly(), and then writes to the protected page to demonstrate
// that a page fault occurs.
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *p;

  p = sbrk(4096);
  if(p == (char*)-1){
    printf(1, "pagetest: sbrk failed\n");
    exit();
  }
  p[0] = 42;  // touch the page so it gets a physical frame

  printf(1, "pagetest: page allocated at va 0x%x\n", (uint)p);
  pageinfo((int)p);

  printf(1, "pagetest: removing write permission with setreadonly()\n");
  if(setreadonly((int)p) < 0){
    printf(1, "pagetest: setreadonly failed\n");
    exit();
  }
  pageinfo((int)p);

  printf(1, "pagetest: attempting to write to the read-only page...\n");
  p[0] = 99;  // must trigger a page fault (trap 14) and kill this process
  printf(1, "pagetest: ERROR - write to read-only page succeeded\n");
  exit();
}
