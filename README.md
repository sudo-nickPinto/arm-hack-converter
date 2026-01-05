# ArmToHack Translator

## Project Overview

This project implements a translator that converts ARM Assembly code into Hack Assembly code. The goal is to bridge the gap between the ARM architecture and the Hack computer architecture (from the Nand2Tetris course), allowing ARM assembly programs to be executed on the Hack platform.

This project is part of the CS 221 Computer Organization and Assembly Language Programming course (Fall 2025).

## Features

-   **Translation Core**: Converts ARM instructions (MOV, ADD, SUB, RSB, CMP, END) to Hack Assembly.
-   **Register Mapping**: Maps ARM registers (R0-R15) to Hack RAM addresses.
-   **Numeric Constants**: Supports immediate values (e.g., `#N`).
-   **Branching**: Handles conditional and unconditional jumps (`BXX` instructions) and label resolution.

## Project Structure

The project is organized as follows:

-   `ArmToHack.h`: Header file containing the `ArmToHack` class definition and translator logic.
-   `main.cpp`: Entry point for the application, handles file I/O and executes the translation.
-   `token_io.h` / `token_io.cpp`: Helper utilities for tokenizing assembly input.
-   `test/`: Directory containing ARM assembly test files (`.arm`) and their translated outputs (`.asm`).

## Development Practices

This project adheres to standard software engineering practices:
-   **Version Control**: Git is used for source code management.
-   **Modular Design**: Separation of concerns between the translator logic and the main execution flow.
-   **Testing**: Comprehensive testing using provided ARM example programs.

## Deployment and Usage

<!-- TEMPLATE: Deployment and Usage Instructions -->
<!-- 
    Instructions on how to build and run the project will be added here.
    
    Example:
    1. Compile the project:
       g++ main.cpp token_io.cpp -o ArmToHack
       
    2. Run the translator:
       ./ArmToHack
       
    3. Verify output in the test/ directory.
-->

*Detailed build and usage instructions to be documented.*

---
*Author: Nicholas Pinto*
*Date: January 2026*
