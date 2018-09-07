# hasm
Assembler for the machine language built for the virtual hardware platform from [From Nand to Tetris MOOC](https://www.coursera.org/learn/build-a-computer) course.

The machine language is based on two 16-bit command types. The address instruction has the format 0xxxxxxxxxxxxxxx.
This instruction causes the computer to load the 15-bit constant xxx...x into the A-register. The compute instruction has the format 111accccccdddjjj. The a and c-bits instruct the ALU which function to compute, the d-bits instruct where to store the ALU output, and the j-bits specify an optional jump condition.
### Building
```bash
mkdir build && cd build
cmake .. && cmake --build .
```
### Usage
```bash
./hasm [inputfile.asm]
```

Assembling process has two passes.The first pass scans source code,inserts labels and user defined variables into symbol table and creates intermediate file that doesn't include labels,spaces and comments.The second pass scans intermediate file,converts **operands** to their respective memory addresses via symbol table and **mnemonics** to equivelant machine instructions,then creates hack executable [inputfile].hex file.

To run hack program, use [hvm](https://github.com/gurpinars/hvm)

More information on hack assembly language [*Chapter 4-Machine Language*](http://www.nand2tetris.org/chapters/chapter%2004.pdf)
