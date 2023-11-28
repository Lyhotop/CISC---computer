# CISC---computer
### What is CISC-computer?
A **complex instruction set computer (CISC)** is a computer architecture in which single instructions can execute several low-level operations (such as a load from memory, an arithmetic operation, and a memory store) or are capable of multi-step operations or addressing modes within single instructions. The term was retroactively coined in contrast to reduced instruction set computer (RISC) and has therefore become something of an umbrella term for everything that is not RISC, where the typical differentiating characteristic is that most RISC designs use uniform instruction length for almost all instructions, and employ strictly separate load and store instructions.
### What does the project consist of?
The project consists of two parts:
* *asol* - the part of the computer that converts assembly code into machine instructions.
* *ssol* - the part of a computer that executes a set of machine instructions step by step.

The *repository* contains documentation on the development of my CISC-computer.
### How to install the project?

```git clone https://github.com/Lyhotop/CISC---computer```

```cd asol/ssol```

```git status```

```md build```

```cd build```

```cmake ..```


### How to run the project?
  Open **cmd.exe**, then use the *"cd"* command to open the ```"test/asol"``` folder.

  Run **asol.exe**. Write in *CMD*: ```asol.exe fileName.as fileName.mc```.
  
  Run **ssol.exe**. Write in *CMD*: ```ssol.exe fileName.mc>fileName.txt```.

The *"fileName.txt"* file contains the result of the execution of the assembly program "fileName.as".

![image](https://github.com/Lyhotop/CISC---computer/assets/128158238/aaf4f23d-11ec-49e7-8bae-40b197f7531b)


