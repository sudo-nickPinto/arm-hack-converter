# Section 8 — ASR (R0,R1 only)

## Scope
Implement `ASR DestReg, RegToShift, #1` using the required mini-program idea,
with working cells `R0` and `R1`.

## Mini-program logic emitted
1. `R1 = 0`
2. Loop while `R0 - 2 >= 0`:
   - `R0 = R0 - 2`
   - `R1 = R1 + 1`
3. Stop when `R0 - 2 < 0`

Result: `R1 = floor(original R0 / 2)`.

## Notes about this section
- This stage intentionally ties computation to `R0` input and `R1` output,
  matching assignment wording.
- The parser still reads `DestReg`, `RegToShift`, and shift token for clarity,
  but the emitted mini-program uses `R0/R1` only.

## Files changed
- `src/ArmToHack.cpp` (added `ASR` opcode branch)
- `src/main.cpp` (added `programASR1` translation call)

## Test
- Input: `test/input/programASR1.arm`
- Output: `test/output/programASR1.asm`
- In CPU Emulator:
  - set RAM[0] manually (even/odd)
  - run program
  - verify RAM[1] is RAM[0] divided by 2 (integer floor)
