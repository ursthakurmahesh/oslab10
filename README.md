# OS Assignment 10 - xv6 (x86)

Solutions for Operating Systems Assignment 10 on MIT's xv6-public (x86) teaching OS:

| Q | Topic | Kernel work | User program |
|---|-------|-------------|--------------|
| 1 | Tracking physical memory allocation | `kalloc()`/`kfree()` counters in `kalloc.c`, new `memstats()` syscall | `memtest.c` |
| 2 | Virtual-to-physical address translation | `vtop()` in `vm.c` using `walkpgdir()`, new `vtop` syscall | `vttest.c` |
| 3 | Page-table management and page protection | `pageinfo()` and `setreadonly()` in `vm.c`, two new syscalls | `pagetest.c` |
| 4 | Page replacement algorithms | none (user-space simulation) | `pagereplace.c` (FIFO, LRU, Optimal; 3/4/5-frame comparison) |

## Layout

Each question has its own folder with a README, its test program, the relevant kernel code, real output and real screenshots:

- `Q1/` - physical memory tracking (`memtest.c`, `q1_memtest.png`)
- `Q2/` - virtual-to-physical translation (`vttest.c`, `q2_vttest.png`)
- `Q3/` - page tables and protection (`pagetest.c`, `q3_pagetest.png`)
- `Q4/` - page replacement (`pagereplace.c`, `q4_fifo.png`, `q4_lru.png`, `q4_opt.png`, `q4_table.png`)
- `oslab10/` - the one full, buildable xv6-public tree with all modifications (every change is commented `Assignment 10, Qx`). The `Qn/` copies are for reading; build from here. Shared plumbing (`syscall.c/h`, `user.h`, `usys.S`, `defs.h`, `Makefile`) lives only here.
- `evidence/` - `modifications.diff` (complete diff against pristine xv6-public) and `terminal-output.txt` (full serial log of one boot).
- `report/OS-Assignment-10-Report.pdf` - one-page report.

## Build and run

```sh
cd oslab10
make            # builds xv6.img and fs.img (needs a 32-bit-capable gcc/binutils)
make qemu-nox   # boots xv6 in QEMU
```

Inside the xv6 shell:

```
$ memtest
$ vttest
$ pagetest                  # ends in a page fault (trap 14) - that is the demonstration
$ pagereplace               # full traces (3 frames) + 3/4/5-frame comparison table
$ pagereplace fifo          # single-algorithm 3-frame trace (also: lru, opt)
$ pagereplace table         # comparison table only
```

## Results summary (Q4)

| Frames | FIFO | LRU | Optimal |
|--------|------|-----|---------|
| 3      | 15   | 12  | 9       |
| 4      | 10   | 8   | 8       |
| 5      | 9    | 7   | 7       |

(page faults on the reference string `7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`).

All outputs were captured from real runs of the modified kernel under QEMU 6.2 (TCG);
screenshots are QEMU `screendump` images of the emulated VGA console, and the serial log
in `evidence/` is the raw output of the same runs.
