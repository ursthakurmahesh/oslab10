# Q1 - Tracking physical memory allocation

- `kalloc()` / `kfree()` in `kalloc.c` count every page handed out and returned (`kpage_allocs`, `kpage_frees`); `kfree_pages()` walks the free list.
- New syscall `memstats()` (`sys_memstats` in `sysproc.c`) prints total allocated, total freed, currently allocated and available physical pages.
- Test program: `memtest.c` (prints stats, allocates 10 pages with `sbrk()`, prints stats, frees them, prints stats).

Files here: `kernel_changes.c.txt` (the Q1 kernel code, for reading), `memtest.c`, `output.txt` (real serial output), `q1_memtest.png` (real QEMU screenshot).

Run (from repo root): `cd oslab10 && make qemu-nox`, then `memtest` at the xv6 prompt.
