# Section 2 — ArmToHack Translator (Stack Initialization)

## Requirement from spec
Each translated program must start by initializing the stack pointer to the expected default address in memory.

## Clarification about your "+4 bytes" note
In ARM machine layout, stack movement is often discussed in bytes (e.g., +4 per word).
In this project, we translate to Hack RAM **cells** (one address per cell), and the spec says to ignore ARM byte-alignment details in this environment.
So we set a default cell index for `SP`, not a byte offset.

## Implementation performed

### 1) Added one configurable data member
In [src/ArmToHack.h](../src/ArmToHack.h):
- `int default_sp_address = 256;`

Rationale:
- keeps the default in one place

### 2) Inserted stack init at the beginning of first pass
In [src/ArmToHack.cpp](../src/ArmToHack.cpp), inside `translateFirstPass(...)`, immediately after `reset()`:
1. `@256` (via `default_sp_address`)
2. `D=A`
3. `@13`  (`R13` is `SP`)
4. `M=D`

Rationale:
- this runs exactly once per translated input file
- uses `write_line(...)`, so `line_number` stays correct for labels/jumps
- preserves spec rule that user code may still change `SP` later

## Validation performed
- Build succeeded with C++20 using:
  - `g++ -std=c++20 src/main.cpp src/token_io.cpp src/ArmToHack.cpp -o translator_hw12`
- Translator ran for program1..program10.
- Output inspection confirmed stack init appears at top of generated output, for example program1 begins with:
  - `@256`
  - `D=A`
  - `@13`
  - `M=D`
