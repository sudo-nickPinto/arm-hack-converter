# Section 9 — ASR with stack-top working cells

## Requirement recap
Rewrite ASR to compute using two cells on top of the stack instead of fixed R0/R1.
No SP movement is needed.

## Stack convention used
- `SP` points one cell below top item.
- First top cell: `SP + 1` (work value)
- Second top cell: `SP + 2` (quotient counter)

## Implemented algorithm for `ASR rd, rs, #1`

### Wrapper setup
1. Compute `SP+1` and save its address in RAM[30002]
2. Compute `SP+2` and save its address in RAM[30003]
3. Copy `rs` into `[SP+1]`
4. Initialize `[SP+2] = 0`

### Mini-program core
Loop while `[SP+1] - 2 >= 0`:
- `[SP+1] = [SP+1] - 2`
- `[SP+2] = [SP+2] + 1`

When `[SP+1] - 2 < 0`, exit loop.

### Wrapper follow-up
- Move `[SP+2]` into `rd`

Result: `rd = floor(rs / 2)`.

## Why temp cells 30002/30003
Hack assembly repeatedly needs the two stack addresses.
Storing these addresses once avoids recomputing `SP+1`/`SP+2` each loop iteration.

## Tests wired in `main.cpp`
- `programASR1.arm`
- `programASR2.arm`
- `programASR3.arm`

## Expected behavior from spec
- `programASR2`: manual values in cells 5 and 8 should produce results in cells 3 and 9.
- `programASR3`: should produce `6,4,5` in cells `3,9,10`.
