# HW12 Roadmap (Incremental)

This folder tracks Assignment 12 work in small, testable steps.

## Scope reminder
- `hw11_files/` is read-only historical baseline.
- All new work happens under `hw12_files/`.

## Sections from spec
1. Separate files (`ArmToHack.h` + `ArmToHack.cpp`)
2. Stack initialization (`SP` default value)
3. Basic `STMDA` / `LDMIB` (single register)
4. Full `STMDA` / `LDMIB` (register list)
5. `STR` / `LDR` memory forms
6. User-defined variables/arrays (`DCD`)
7. `LDR Rd, =array`
8. `ASR` mini-program (R0/R1 version)
9. `ASR` mini-program (stack version)
10. Final integration (`program23`)

## Process used for each section
- Explain goal in plain language
- Implement smallest complete change
- Rebuild and run quick checks
- Commit only section-related files
