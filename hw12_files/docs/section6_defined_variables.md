# Section 6 — Defined Variables (`DCD`)

## What this section adds
Supports ARM-style data directives such as:
- `numbers1 DCD 1, 2, 3, 4`
- `value DCD 4`

In this assignment, each value occupies one Hack RAM cell.
Allocation starts at RAM address `16` and grows upward.

## Implementation summary

### 1) New allocation pointer in class state
In `ArmToHack.h`:
- `next_var_address = 16`

This pointer tracks where the next `DCD` value should be placed.

### 2) Reset pointer for each translation
In `reset()`:
- `next_var_address = 16`

This ensures each program starts variable allocation from cell 16.

### 3) Detect `DCD` lines during first pass
In `translateFirstPass(...)`:
- parse first and second tokens
- if second token is `DCD`, call `translateDCD(line)` and continue

This prevents `DCD` lines from being treated as normal opcodes.

### 4) New `translateDCD(...)` helper
Steps performed:
1. read variable name
2. read `DCD` keyword
3. loop over each remaining literal value
4. emit Hack code:
   - `@value`
   - `D=A`
   - `@next_var_address`
   - `M=D`
5. increment `next_var_address`

### 5) Literal normalization
`+N` values are normalized by stripping leading `+` so generated `@N` is valid.
Negative values remain valid for Hack A-instructions as used in this project setup.

## Why this design
- Keeps data directives separate from executable instruction translation.
- Keeps allocation deterministic and aligned with spec requirement: start at cell 16.
- Sets up cleanly for Section 7, where variable names need base-address lookup.

## Validation targets
- `program19.arm` should produce values `1..4` in RAM `16..19`.
- `program20.arm` should produce values `1..8` in RAM `16..23`.
