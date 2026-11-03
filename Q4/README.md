# Q4 - Page replacement algorithms

No kernel change. `pagereplace.c` is a user-space xv6 program that simulates FIFO, LRU and Optimal on the reference string `7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`.

| Frames | FIFO | LRU | Optimal |
|--------|------|-----|---------|
| 3      | 15   | 12  | 9       |
| 4      | 10   | 8   | 8       |
| 5      | 9    | 7   | 7       |

Files here: `pagereplace.c`, `output.txt` (real serial output, all traces + table), `q4_fifo.png`, `q4_lru.png`, `q4_opt.png`, `q4_table.png` (real QEMU screenshots).

Run: `cd oslab10 && make qemu-nox`, then `pagereplace` (or `pagereplace fifo|lru|opt|table`).
