## Vole-machine
Machine Simulator in C++
# Overview
This C++ project implements a machine simulator designed to mimic a simple computer architecture. It features a Central Processing Unit (CPU), Arithmetic Logic Unit (ALU), memory with 256 cells, and 16 registers. The simulator executes hex-based instructions loaded from a file, supporting integer and floating-point arithmetic, bitwise operations (OR, AND, XOR), rotation, and memory operations. It includes a control unit (CU) for instruction handling and outputs results to a virtual screen in both ASCII and hexadecimal formats. This project was developed as part of an educational assignment at Cairo University, building on object-oriented programming principles.

# Features

- ALU Operations: Performs hex-based addition (integer and float), bitwise OR, AND, XOR, and left/right rotation of bits.
- Memory and Registers: Manages 256 memory cells and 16 registers, all initialized with hex values, with robust bounds checking to prevent out-of-range access.
- Instruction Set: Supports a variety of commands including load from memory, store to memory, move between registers, jump, halt, and arithmetic/logical operations.
- Screen Output: Displays program output at memory address 0 in ASCII and hex, ideal for visualizing results of executed instructions.
- Error Handling: Includes exception handling for invalid addresses, file operations, and unknown instructions.

# Usage
- Interact with the machine simulator through an interactive menu-driven interface:

- Compile and Run the Program:

  - Use a C++ compiler (e.g., g++ main.cpp class_T4.cpp -o simulator) to compile the project.
  - Run the executable (e.g., ./simulator on Linux/Mac or simulator.exe on Windows).


- Select an Option:

  - Upon starting, a menu appears with five options. Enter a single letter (a-e) to proceed:
  - a: Load a program file and specify a starting memory address.
  - b: Execute the entire loaded program until completion or halt.
  - c: Execute the program one instruction at a time (step-by-step mode).
  - d: Display the current state of registers, memory, and screen output.
  - e: Exit the program and terminate the simulator.




- Follow Prompts:

  - For option a: Enter the filename (e.g., program.txt) and a two-digit hex address (e.g., 10) where the program should start.
  - For options b and c: The simulator processes instructions automatically; use d to check progress.
  - For option d: View the detailed state without modifying it.
  - Repeat the menu selection after each action until choosing e.



- Example Session:
``` bash
Choose one of the following choices:
a) Read file and Load program
b) Execute the program
c) Run step
d) Display the content
e) Exit
a
Enter the filename
program.txt
Enter the memory cell address to store the program at
10
Filed loaded successfully
d
=== Registers ===
00  00  00  00
00  00  00  00
00  00  00  00
00  00  00  00

=== Memory ===
00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00
...
10  12  34  56  78  00  00  00  00  00  00  00  00  00  00  00
...

=== Screen ===
ASCII: Hello
HEX: 48 65 6C 6C 6F
```

Program File Format: Create a text file (e.g., program.txt) with hex instructions, each as a 4-character string (e.g., 1234 for a load instruction). Separate instructions with spaces or newlines.

# Limitations

- Supports only hex-based instructions; decimal input is not directly handled.
- Restricted to 256 memory cells and 16 registers, limiting program size.
- Console-based interface with no graphical visualization.
- Requires manually formatted program files; no syntax checking beyond hex validity.
- Floating-point operations use a simplified 8-bit representation, which may lose precision.

# License
This project is licensed under the MIT License. See the LICENSE file for details.

# Authors

- Mariam Hesham - [mariam0905]
- Computer Science Student at Cairo University

# Acknowledgments

- Developed as an educational project for Cairo University, enhancing skills from prior assignments like InvestEase and gaming frameworks.
- Grateful to instructors and peers for guidance and feedback.

