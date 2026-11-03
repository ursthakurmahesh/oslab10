# OS Assignment 10 - xv6 (x86)

Solutions for Operating Systems Assignment 10 on MIT's xv6-public (x86) teaching OS:

| Q | Topic | Kernel work | User program |
|---|-------|-------------|--------------|
| 1 | Tracking physical memory allocation | `kalloc()`/`kfree()` counters in `kalloc.c`, new `memstats()` syscall | `memtest.c` |
| 2 | Virtual-to-physical address translation | `vtop()` in `vm.c` using `walkpgdir()`, new `vtop` syscall | `vttest.c` |
| 3 | Page-table management and page protection | `pageinfo()` and `setreadonly()` in `vm.c`, two new syscalls | `pagetest.c` |
| 4 | Page replacement algorithms | none (user-space simulation) | `pagereplace.c` (FIFO, LRU, Optimal; 3/4/5-frame comparison) |

## Layout

- `xv6/` - full xv6-public source with all modifications. Every change is commented `Assignment 10, Qx`.
  Modified kernel files: `kalloc.c`, `vm.c`, `sysproc.c`, `syscall.c`, `syscall.h`, `defs.h`, `user.h`, `usys.S`, `Makefile`.
- `evidence/modifications.diff` - complete diff against pristine xv6-public.
- `evidence/terminal-output.txt` - full serial log of one QEMU boot running all four test programs.
- `screenshots/` - genuine QEMU VGA screen captures (screendump) of each test run:
  - `q1_memtest.png` - memstats before/after allocating and releasing 10 pages
  - `q2_vttest.png` - va -> pa translations and the unmapped-address case
  - `q3_pagetest.png` - page flags before/after `setreadonly()` and the resulting page fault (`trap 14 err 7`)
  - `q4_fifo.png`, `q4_lru.png`, `q4_opt.png` - per-reference 3-frame traces (15 / 12 / 9 faults)
  - `q4_table.png` - 3/4/5-frame comparison table (faults, hits, hit ratio, fault ratio)
- `report/OS-Assignment-10-Report.pdf` - short report (implementation, modifications, algorithms, observations).

## Build and run

```sh
cd xv6
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
