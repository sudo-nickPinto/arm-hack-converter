# Section 7 — Defined Variables and `LDR rd, =array`

## Goal
Extend `LDR` so this form works:
- `LDR rd, =arrayName`

Meaning:
- load the base RAM address of `arrayName` into `rd`.

## New lookup table
Added in `ArmToHack.h`:
- `var_map`

Purpose:
- map `arrayName -> baseAddress`
- example: `numbers1 -> 16`

## How it works end-to-end

### 1) During `DCD` processing
Inside `translateDCD(...)`:
- read `var_name`
- before storing literals, save base:
  - `var_map[var_name] = var_address`

Then store each literal at current `var_address`, incrementing for each cell.

### 2) During `LDR`
Inside `LDR` handler:
- parse `rd` and next token
- if next token starts with `=`:
  - strip `=` to get symbol name
  - lookup base in `var_map`
  - write base immediate into `rd`
  - return from `LDR` handler

Else continue with existing memory forms (`[rb, ri, ...]` and `[rb, #... ]`).

## Why this design
- Keeps variable declaration and variable use cleanly separated.
- Uses one symbol table (`var_map`) created from `DCD` lines.
- Preserves existing `LDR` behavior for normal memory addressing.

## Validation targets
- `program21`: memory cells `16..19` should become `4,2,-3,1`
- `program22`: registers `R4..R10` should become `1,4,5,5,4,5,7`
