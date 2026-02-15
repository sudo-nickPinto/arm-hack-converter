# Section 1 — Separate Files

## What we changed
We split the monolithic translator into:
- `src/ArmToHack.h` → class declaration only
- `src/ArmToHack.cpp` → method implementations

## Why this is the right design
- **Separation of interface and implementation**: readers can learn the class API quickly from the header.
- **Scalability**: HW12 adds many new instructions and helper methods; keeping all code in a header becomes hard to maintain.
- **Build hygiene**: source files compile once, reducing duplicate compilation work.

## What stayed the same
- Existing HW11 behavior for programs 1–10
- Existing maps (`reg_map`, `jump_map`, `label_map`, `fix_map`)
- Existing two-pass jump-fix architecture

## Validation
- Built with `g++ -std=c++20`
- Ran translator on programs 1–10
- Diff check shows `hw12_files/test/output` matches `hw11_files/test/output`

## Notes
`main.cpp` paths were aligned to current folder layout:
- inputs: `test/input/*.arm`
- outputs: `test/output/*.asm`
