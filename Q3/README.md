# Q3 - Page-table management and page protection

- `pageinfo()` in `vm.c` prints the mapping, frame number and P/W/U flags for a virtual address.
- `setreadonly()` in `vm.c` clears `PTE_W` and reloads CR3 to flush the TLB.
- New syscalls `pageinfo` and `setreadonly` (wrappers in `sysproc.c`).
- Test program: `pagetest.c` shows flags, makes the page read-only, shows flags again, then writes to it. The write causes a page fault (`trap 14 err 7`), which is the expected demonstration.

Files here: `kernel_changes.c.txt`, `pagetest.c`, `output.txt` (real serial output), `q3_pagetest.png` (real QEMU screenshot).

Run: `cd oslab10 && make qemu-nox`, then `pagetest`.
