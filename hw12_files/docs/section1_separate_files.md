# Section 1 — Separate Files (Execution Record + Re-evaluation)

## Assignment requirement (from `hw12_spec.html`)
> Organize the code into separate files:
> - `ArmToHack.h`: class with method signatures
> - `ArmToHack.cpp`: method implementations

This section is only about project structure and reordering of behavior previous to this implementation

---

## Exactly what was executed

### 1) Restored the split-file architecture in `hw12_files/src`
- `ArmToHack.h` keeps declarations only.
- `ArmToHack.cpp` now contains all `ArmToHack::...` method definitions.

### 2) Kept the `main.cpp` path correction
- Input paths: `test/input/programX.arm`
- Output paths: `test/output/programX.asm`


### 3) Cleaned stale generated artifacts
- Removed old generated files from `hw12_files/test/` root (`program*.asm`, `program*.arm.tmp`).
- Kept canonical data under `hw12_files/test/input` and `hw12_files/test/output`.

### 4) Rebuilt and re-ran translator
- Build command succeeded with C++20.
- Translator ran successfully for programs 1..10.

### 5) Regression check against HW11 baseline
- `diff -rq hw11_files/test/output hw12_files/test/output`
- No differences reported.

---


## File map after Section 1
- `src/ArmToHack.h` → class API declarations
- `src/ArmToHack.cpp` → translator logic implementation
- `src/main.cpp` → translation driver using `test/input` and `test/output`


