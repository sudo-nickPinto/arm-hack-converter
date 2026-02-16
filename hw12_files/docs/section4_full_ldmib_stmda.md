# Section 4 — Full `LDMIB` and `STMDA` (multi-entry register list)

## Goal
Extend Section 3 from single-register lists to full register lists, while preserving list order.

Examples covered:
- `STMDA Rn!, {R7, R2, R11}`
- `LDMIB Rn!, {R7, R2, R11}`

## Why order matters
The spec states the user will list registers in the same order for related save/restore instructions.
So translation must process the list left-to-right exactly as written.

## Implemented behavior

### `STMDA Rn!, {rs1, rs2, ...}`
For each `rs` in order:
1. `D = rs`
2. `RAM[Rn] = D`
3. `Rn = Rn - 1`

### `LDMIB Rn!, {rd1, rd2, ...}`
For each `rd` in order:
1. `Rn = Rn + 1`
2. `D = RAM[Rn]`
3. `rd = D`

## Parsing strategy
Inside `translateXXX(...)`:
- Parse `Rn!` first, strip `!`
- Repeatedly extract tokens from the remainder of the line
- Strip braces with `strip(token, "{}")`
- Skip empty tokens and process each valid register (`rs` for `STMDA`, `rd` for `LDMIB`)

This handles `{R2, LR, R1}` and similar lists directly.

## Program expectations
Using your test files:
- `program13.arm`: initial `SP=20`, three `STMDA` stores → final `SP=17`
- `program14.arm`: initial `SP=5`, three `LDMIB` loads → final `SP=8`

## Validation run
- Added translation calls for program13/program14 in `src/main.cpp`
- Built and executed translator successfully
- Generated:
  - `test/output/program13.asm`
  - `test/output/program14.asm`

## Notes
- Variable naming follows operand role (`rs` for source in `STMDA`, `rd` for destination in `LDMIB`).
- This section does not yet implement `LDR/STR`; that is Section 5.
