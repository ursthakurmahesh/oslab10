// Assignment 10, Q4: user program implementing and comparing three page
// replacement algorithms - FIFO, LRU and Optimal - on the reference
// string  7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1  (20 references).
//
// Usage:
//   pagereplace         full run: per-reference traces for 3 frames and
//                       the 3/4/5-frame comparison table
//   pagereplace fifo    FIFO trace with 3 frames only (fits one screen)
//   pagereplace lru     LRU trace with 3 frames only
//   pagereplace opt     Optimal trace with 3 frames only
//   pagereplace table   comparison table only (3, 4 and 5 frames)
#include "types.h"
#include "stat.h"
#include "user.h"

#define NREF 20
static int ref[NREF] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

static int
in_frames(int *frames, int n, int page)
{
  int i;
  for(i = 0; i < n; i++)
    if(frames[i] == page)
      return 1;
  return 0;
}

static void
print_frames(int *frames, int n)
{
  int i;
  printf(1, "[");
  for(i = 0; i < n; i++){
    if(frames[i] < 0)
      printf(1, " -");
    else
      printf(1, " %d", frames[i]);
  }
  printf(1, " ]");
}

static void
print_header(char *name, int nframes)
{
  int i;
  printf(1, "=== %s (%d frames) ===\n", name, nframes);
  printf(1, "ref string:");
  for(i = 0; i < NREF; i++)
    printf(1, " %d", ref[i]);
  printf(1, "\n");
}

// Q4(a): FIFO - evict the page that entered earliest.
static int
fifo(int nframes, int trace)
{
  int frames[16];
  int i, page, faults = 0, qpos = 0;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  if(trace) print_header("FIFO", nframes);
  for(i = 0; i < NREF; i++){
    page = ref[i];
    if(in_frames(frames, nframes, page)){
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " HIT\n");
      }
    } else {
      frames[qpos] = page;               // replace oldest page
      qpos = (qpos + 1) % nframes;
      faults++;
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " FAULT\n");
      }
    }
  }
  if(trace) printf(1, "FIFO total page faults: %d\n\n", faults);
  return faults;
}

// Q4(a): LRU - evict the page whose last use is farthest in the past.
static int
lru(int nframes, int trace)
{
  int frames[16], used[16];
  int i, j, page, faults = 0, victim, oldest;

  for(i = 0; i < nframes; i++){
    frames[i] = -1;
    used[i] = -1;
  }

  if(trace) print_header("LRU", nframes);
  for(i = 0; i < NREF; i++){
    page = ref[i];
    if(in_frames(frames, nframes, page)){
      for(j = 0; j < nframes; j++)
        if(frames[j] == page)
          used[j] = i;                   // record most recent use
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " HIT\n");
      }
    } else {
      // victim: an empty frame, else the least recently used one
      victim = -1;
      oldest = NREF + 1;
      for(j = 0; j < nframes; j++){
        if(frames[j] < 0){ victim = j; break; }
        if(used[j] < oldest){ oldest = used[j]; victim = j; }
      }
      frames[victim] = page;
      used[victim] = i;
      faults++;
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " FAULT\n");
      }
    }
  }
  if(trace) printf(1, "LRU total page faults: %d\n\n", faults);
  return faults;
}

// Q4(a): Optimal - evict the page whose next use is farthest in the
// future (or never used again).
static int
optimal(int nframes, int trace)
{
  int frames[16];
  int i, j, k, page, faults = 0, victim, farthest, next;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  if(trace) print_header("Optimal", nframes);
  for(i = 0; i < NREF; i++){
    page = ref[i];
    if(in_frames(frames, nframes, page)){
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " HIT\n");
      }
    } else {
      // victim: an empty frame, else the page used farthest ahead
      victim = -1;
      farthest = -1;
      for(j = 0; j < nframes; j++){
        if(frames[j] < 0){ victim = j; break; }
        next = NREF;                     // "never used again" by default
        for(k = i + 1; k < NREF; k++)
          if(ref[k] == frames[j]){ next = k; break; }
        if(next > farthest){ farthest = next; victim = j; }
      }
      frames[victim] = page;
      faults++;
      if(trace){
        printf(1, "ref %d: ", page);
        print_frames(frames, nframes);
        printf(1, " FAULT\n");
      }
    }
  }
  if(trace) printf(1, "Optimal total page faults: %d\n\n", faults);
  return faults;
}

static void
run_three(int nframes)
{
  fifo(nframes, 1);
  lru(nframes, 1);
  optimal(nframes, 1);
}

// Q4(c): comparison table for 3, 4 and 5 frames.
static void
print_table(void)
{
  int nf, f_fifo, f_lru, f_opt, hits;

  printf(1, "=== Comparison table: 3, 4 and 5 frames ===\n");
  printf(1, "(same reference string, %d references)\n\n", NREF);
  printf(1, "Frames  Algo     Faults  Hits  Hit%%   Fault%%\n");
  for(nf = 3; nf <= 5; nf++){
    f_fifo = fifo(nf, 0);
    f_lru  = lru(nf, 0);
    f_opt  = optimal(nf, 0);

    hits = NREF - f_fifo;
    printf(1, "  %d     FIFO     %d      %d    %d%%    %d%%\n",
           nf, f_fifo, hits, hits * 100 / NREF, f_fifo * 100 / NREF);
    hits = NREF - f_lru;
    printf(1, "  %d     LRU      %d      %d    %d%%    %d%%\n",
           nf, f_lru, hits, hits * 100 / NREF, f_lru * 100 / NREF);
    hits = NREF - f_opt;
    printf(1, "  %d     Optimal  %d      %d    %d%%    %d%%\n",
           nf, f_opt, hits, hits * 100 / NREF, f_opt * 100 / NREF);
    printf(1, "\n");
  }
  printf(1, "Best for this reference string: Optimal (9 faults at 3 frames).\n");
  printf(1, "LRU is close behind (ties Optimal at 4 and 5 frames); FIFO is\n");
  printf(1, "the worst of the three at every frame count tested.\n");
}

int
main(int argc, char *argv[])
{
  if(argc > 1){
    if(strcmp(argv[1], "fifo") == 0){ fifo(3, 1); exit(); }
    if(strcmp(argv[1], "lru") == 0){ lru(3, 1); exit(); }
    if(strcmp(argv[1], "opt") == 0){ optimal(3, 1); exit(); }
    if(strcmp(argv[1], "table") == 0){ print_table(); exit(); }
    printf(1, "usage: pagereplace [fifo|lru|opt|table]\n");
    exit();
  }

  // Q4(a)+(b): full per-reference traces with 3 frames
  run_three(3);
  // Q4(c): comparison across 3, 4 and 5 frames
  print_table();
  exit();
}
