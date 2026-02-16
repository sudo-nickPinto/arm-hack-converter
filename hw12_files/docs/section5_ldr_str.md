# Section 5 — `LDR` and `STR`

## Scope implemented
Supported forms:
- `STR rs, [rb, ri, LSL #2]`
- `LDR rd, [rb, ri, LSL #2]`
- `STR rs, [rb, #+/-N]`
- `LDR rd, [rb, #+/-N]`

`LSL #2` is ignored as required by the spec.

## Effective address rule
For both instructions:
- `addr = rb + offset`
- `offset` is either register value (`ri`) or immediate (`#N`, `#+N`, `#-N`)

## Translation design

### `LDR rd, [...]`
1. Compute `D = rb + offset`
2. Load `D = RAM[D]`
3. Store in `rd`

### `STR rs, [...]`
1. Save `rs` value in temp RAM cell `30001`
2. Compute `D = rb + offset`
3. Save effective address in temp RAM cell `30000`
4. Write `RAM[addr] = rs`

## Why temp cells were used for `STR`
Hack needs both source value and computed address at the same time.
Using two dedicated high RAM scratch cells avoids clobbering ARM registers `R0..R15`.

## Files changed
- `src/ArmToHack.cpp` (new opcode handlers)
- `src/main.cpp` (added program15..program18 translation calls)

## Validation target checks
Expected outcomes from spec:
- `program15`: `R7=42`
- `program16`: `R3=42, R4=24, R5=66`
- `program17`: `RAM[19]=42`
- `program18`: `RAM[18]=42, RAM[19]=24, RAM[20]=66`

## Line-by-line walkthrough of implementation

Implementation area in source:
- `LDR` block: [hw12_files/src/ArmToHack.cpp](../src/ArmToHack.cpp#L456-L508)
- `STR` block: [hw12_files/src/ArmToHack.cpp](../src/ArmToHack.cpp#L510-L568)

### `LDR rd, [rb, ...]` walkthrough

1. Opcode guard enters `LDR` handler.
	- [if (opcode == "LDR")](../src/ArmToHack.cpp#L456)

2. Parse 3 meaningful tokens from the ARM line.
	- `rd` destination register token
	- `rb` base token (arrives like `[R3`)
	- offset token (arrives like `R4` or `#2]`)
	- [token extraction lines](../src/ArmToHack.cpp#L461-L463)

3. Remove bracket characters so tokens become plain register/immediate text.
	- [strip brackets](../src/ArmToHack.cpp#L465-L466)

4. Convert `rd` and `rb` into mapped Hack RAM register indices.
	- [map lookup](../src/ArmToHack.cpp#L468-L469)

5. Start effective-address math: load base value into `D`.
	- [D = rb](../src/ArmToHack.cpp#L472-L473)

6. Add offset to `D`.
	- If offset is immediate (`#...`), strip `#` and handle sign:
	  - negative: `D=D-A`
	  - non-negative: `D=D+A`
	- If offset is register, add register contents: `D=D+M`
	- [offset branch](../src/ArmToHack.cpp#L476-L495)

7. Dereference computed address.
	- `A=D` then `D=M`, so now `D = RAM[rb+offset]`
	- [dereference](../src/ArmToHack.cpp#L498-L499)

8. Write loaded value into destination register `rd`.
	- [store to rd](../src/ArmToHack.cpp#L502-L503)

9. If destination is `PC`, perform control-flow jump.
	- [pc write handling](../src/ArmToHack.cpp#L505)

### `STR rs, [rb, ...]` walkthrough

1. Opcode guard enters `STR` handler.
	- [if (opcode == "STR")](../src/ArmToHack.cpp#L510)

2. Parse `rs`, `rb`, and offset tokens.
	- [token extraction lines](../src/ArmToHack.cpp#L514-L516)

3. Remove bracket characters from memory-form tokens.
	- [strip brackets](../src/ArmToHack.cpp#L518-L519)

4. Resolve register map addresses for `rs` and `rb`.
	- [map lookup](../src/ArmToHack.cpp#L521-L522)

5. Preserve source value before address computations.
	- load `rs` into `D`
	- save `D` into scratch RAM cell `30001`
	- [save source value](../src/ArmToHack.cpp#L525-L528)

6. Load base value into `D`.
	- [D = rb](../src/ArmToHack.cpp#L531-L532)

7. Compute `D = rb + offset` with same immediate/register handling as `LDR`.
	- [offset branch](../src/ArmToHack.cpp#L535-L554)

8. Save computed effective address to scratch RAM cell `30000`.
	- [save effective address](../src/ArmToHack.cpp#L557-L558)

9. Complete store using two scratch cells.
	- reload source value from `30001`
	- set `A` to address stored in `30000`
	- write `M=D`
	- [final write sequence](../src/ArmToHack.cpp#L561-L565)

## Intuition in assignment context

- `LDR` means: **read from memory into a register**.
  - Think: "go to address `rb+offset`, fetch value, put in `rd`."

- `STR` means: **write from a register into memory**.
  - Think: "take value in `rs`, place it at address `rb+offset`."

In this assignment, `LSL #2` is intentionally ignored, so we do not multiply offsets by 4.
