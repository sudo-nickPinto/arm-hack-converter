# Section 3 — Basic `STMDA` and `LDMIB` (single register)

## Spec scope implemented
This section implements the **basic** case only:
- `STMDA Rn!, {R7}`
- `LDMIB Rn!, {R7}`

The register list is assumed to have exactly one register in this section.

## Semantics used
- `STMDA Rn!, {Rs}`
  - `MEM[Rn] = Rs`
  - `Rn = Rn - 1`

- `LDMIB Rn!, {Rd}`
  - `Rn = Rn + 1`
  - `Rd = MEM[Rn]`

These are the exact behaviors emitted in Hack instructions.

## Code changes

### 1) Translator logic
In [src/ArmToHack.cpp](../src/ArmToHack.cpp):
- Added `if (opcode == "STMDA") { ... }`
- Added `if (opcode == "LDMIB") { ... }`

Parsing details:
- Base register token is like `SP!` → stripped with `strip(token, "!")`
- Register-list token is like `{R2}` → stripped with `strip(token, "{}")`

Generated Hack pattern for `STMDA`:
1. Load source register value into `D`
2. Set `A` to memory address stored in `Rn` (`A=M`)
3. Store `D` there (`M=D`)
4. Decrement `Rn` (`M=M-1`)

Generated Hack pattern for `LDMIB`:
1. Increment `Rn` (`M=M+1`)
2. Use incremented pointer to read memory (`A=M`, `D=M`)
3. Write value to destination register

### 2) Test driver
In [src/main.cpp](../src/main.cpp):
- Added translation calls for:
  - `test/input/program11.arm -> test/output/program11.asm`
  - `test/input/program12.arm -> test/output/program12.asm`

## Reasoning check for expected `SP`
- `program11.arm` final `SP` should be **17**.
- `program12.arm` final `SP` should be **7**.

## Validation performed
- Build succeeded with C++20.
- Translator ran successfully.
- `program11.asm` and `program12.asm` were generated in `test/output`.

## Notes
- This section intentionally handles only single-register lists.
