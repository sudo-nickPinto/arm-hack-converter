# Assembler and Translator: The Bridge Between Architectures

## 1. Introduction: What are we building?

We are building a **Translator**. In the world of compilers and assembly, a translator converts code from one source language to another target language.

*   **Source Language**: ARM Assembly (A strictly register-based, widely used architecture).
*   **Target Language**: Hack Assembly (The simplified, educational architecture from the Nand2Tetris course).

Our goal is to write a C++ program that reads an ARM assembly file (e.g., `ADD R1, R2, R3`) and spits out the equivalent Hack assembly instructions (e.g., `@R2, D=M, @R3...`).

---

## 2. Key Readings & Concepts

### N2T Chapter 6: The Assembler (Nand2Tetris)
This chapter describes the **Hack Assembly Language** and how it maps to binary machine code. While we aren't writing a Hack Assembler (that converts Hack to Binary), we are writing code that *outputs* Hack Assembly. Therefore, we must master the destination language.

**Crucial Hack Concepts:**
1.  **The A-Instruction (`@value`)**:
    *   Sets the **A-register** to a specific value.
    *   Example: `@100` puts the value 100 into register A.
    *   *Side Effect*: It also selects the memory address `M` (RAM[A]). If A=100, then `M` refers to RAM[100].

2.  **The C-Instruction (`dest = comp ; jump`)**:
    *   Performs usage computation.
    *   **comp**: The AL computation (e.g., `D+1`, `D+M`, `0`, `-1`).
    *   **dest**: Where to store the result (e.g., `M`, `D`, `MD`, `A`).
    *   **jump**: Conditional jump based on the result (e.g., `JGT`, `JEQ`, `JMP`).

3.  **The Registers:**
    *   **D**: Data register. Holds values for calculation.
    *   **A**: Address register. Holds addresses or immediate constants.
    *   **M**: The value in RAM at the address currently stored in A.

### ARM Architecture (The Source)
ARM is a RISC (Reduced Instruction Set Computer) architecture. The assignment references standard ARMv7-M or similar thumb instructions.

**Crucial ARM Concepts:**
1.  **Instruction Format**: `OPCODE Dest, Op1, Op2`
    *   Example: `ADD R0, R1, R2` means `R0 = R1 + R2`.
2.  **Registers**:
    *   **R0 - R12**: General purpose registers.
    *   **R13 (SP)**: Stack Pointer.
    *   **R14 (LR)**: Link Register (stores return addresses).
    *   **R15 (PC)**: Program Counter (stores current execution address).

---

## 3. The Core Challenge: Mapping Architectures

The hardest part of this project is that ARM is much more powerful than Hack. We have to "downgrade" complex ARM instructions into sequences of simple Hack instructions.

### The "Three-Address" vs. "Two-Address" Problem
*   **ARM is 3-Address**: `ADD R0, R1, R2` (Dest, Source1, Source2).
*   **Hack is (mostly) 1-Address**: You can only operate on D and *one other thing* (A or M).

**Translation Strategy Example: `ADD R0, R1, R2`**
To do `R1 + R2` and store in `R0` on Hack, we need multiple steps:
1.  Get value of R1 into D register.
2.  Add value of R2 to D.
3.  Store result D into R0.

**Hypothetical Hack Translation:**
```
// Get R1
@R1   // Select address of R1
D=M   // Load value of R1 into D

// Add R2
@R2   // Select address of R2
D=D+M // Add value of R2 to D, store result in D

// Store in R0
@R0   // Select address of R0
M=D   // Store D into RAM[R0]
```

### The Label & Jump Problem
*   **ARM**: `BUEQ LABEL` (Branch to LABEL if equal).
*   **Hack**:
    ```
    @LABEL // Load address of LABEL into A
    D;JEQ  // Jump to address in A if D == 0
    ```
    *Note: In our Translator assignment, we assume the comparison result is already in D.*

---

## 4. Reading Specifics for this Project

### `ArmToHack` Structure
The project requires a class-based approach. The logic is encapsulated in the `ArmToHack` class.
*   **First Pass**: We scan the code found labels (like `LOOP:`) and remember which line number they correspond to. This is essential because you can jump *forward* to code that hasn't been written yet.
*   **Translation**: We process line-by-line using `token_io` to split strings like `ADD R0, R1, #5` into `ADD`, `R0`, `R1`, `#5`.

### `token_io.h` (The Helper)
This file is your tokenizer. You don't need to write string parsing logic.
*   `extract_token(input)`: Grabs the next word.
*   `strip(str, "#")`: Removes characters.
This allows you to focus on the *logic* of assembly, not the *tedium* of string manipulation.
