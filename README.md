# hasm
Assembler for hack machine language built for virtual hardware platform from [From Nand to Tetris](https://www.coursera.org/learn/build-a-computer) course.

### How to use
```bash
➜  ~ make
➜  ~ ./hasm [inputfile.asm]
```
Assembling process has two passes.The first pass scans source code,inserts labels and user defined variables into symbol table and creates intermediate file that doesn't include labels,spaces and comments.The second pass scans intermediate file,converts **operands** to their respective memory addresses via symbol table and **mnemonics** to equivelant machine instructions,then creates [inputfile].o file.

More information on hack assembly language [*Chapter 4-Machine Language*](http://www.nand2tetris.org/chapters/chapter%2004.pdf)
