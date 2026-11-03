# Q2 - Virtual-to-physical address translation

- `vtop()` in `vm.c` finds the page-table entry with `walkpgdir()` and returns frame address | page offset, or -1 if the page is not mapped.
- New syscall `vtop()` (`sys_vtop` in `sysproc.c`).
- Test program: `vttest.c` translates addresses inside an `sbrk()` block and one unmapped address.

Files here: `kernel_changes.c.txt`, `vttest.c`, `output.txt` (real serial output), `q2_vttest.png` (real QEMU screenshot).

Run: `cd oslab10 && make qemu-nox`, then `vttest`.
