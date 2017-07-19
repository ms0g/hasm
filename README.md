# hasm
Assembler for hack machine language built for virtual hardware platform from [From Nand to Tetris](https://www.coursera.org/learn/build-a-computer) course.
### Platform
The platform is a 16-bit von Neumann machine, consisting of a CPU,two separate memory modules serving as instruction memory and data memory.The CPU consists of the ALU and three registers called data register(D), address register (A), and program counter(PC). D and A are general-purpose 16-bit registers that can be manipulated by arithmetic and logical instructions like A=D-1, D=D|A, and so on.While the D-register is used solely to store data values, the contents of the A-register can be interpreted in three different ways, depending on the instruction’s context: as a data value, as a RAM address, or as a ROM address.

The machine language is based on two 16-bit command types. The address instruction has the format 0xxxxxxxxxxxxxxx.
This instruction causes the computer to load the 15-bit constant xxx...x into the A-register. The compute instruction has the format 111accccccdddjjj. The a and c-bits instruct the ALU which function to compute, the d-bits instruct where to store the ALU output, and the j-bits specify an optional jump condition.

### How to use
```bash
➜  ~ make
➜  ~ ./hasm [inputfile.asm]
```

Assembling process has two passes.The first pass scans source code,inserts labels and user defined variables into symbol table and creates intermediate file that doesn't include labels,spaces and comments.The second pass scans intermediate file,converts **operands** to their respective memory addresses via symbol table and **mnemonics** to equivelant machine instructions,then creates hack executable [inputfile].hex file.

To run hack program
```bash
➜  ~ make hvm
➜  ~ ./hvm [inputfile.hex]
```

More information on hack assembly language [*Chapter 4-Machine Language*](http://www.nand2tetris.org/chapters/chapter%2004.pdf)
