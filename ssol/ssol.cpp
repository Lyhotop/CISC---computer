#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NUMMEMORY 16777216 // Maximum number of words in memory.
#define NUMREGS 64 // Number of machine registers.
#define MAXLINELENGTH 1000
// Initialization of commands.
#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define DEC 7
#define DIV 8
#define XIMUL 9
#define XOR 10
#define SHL 11
#define MOV 12
#define JMAE 13
#define JMNGE 14
#define BT 15
#define CMP 16
#define RCL 17
typedef struct stateStruct {
	int pc;
	long long int* mem = new long long int[NUMMEMORY];
	long long int reg[NUMREGS];
	int numMemory;
	int CF;
} stateType;

void printState(stateType*);
void run(stateType);
long long int convertNum(int);

int main(int argc, char* argv[])
{
	int i;
	char line[MAXLINELENGTH];
	stateType state;
	FILE* filePtr;
	if (argc != 2) {
		printf("Error: usage: %s <machine-code file>!\n", argv[0]);
		exit(1);
	}
	// Initialize memories and registers.
	for (i = 0; i < NUMMEMORY; i++) {
		state.mem[i] = 0;
	}
	for (i = 0; i < NUMREGS; i++) {
		state.reg[i] = 0;
	}
	state.pc = 0;
	state.CF = 0;
	// Read machine-code file.
	filePtr = fopen(argv[1], "r");
	if (filePtr == NULL) {
		printf("Error: can't open file %s!\n", argv[1]);
		perror("fopen");
		exit(1);
	}
	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
		state.numMemory++) {
		if (state.numMemory >= NUMMEMORY) {
			printf("Exceeded memory size!\n");
			exit(1);
		}
		if (sscanf(line, "%lld", state.mem + state.numMemory) != 1) {
			printf("Error in reading address %d!\n", state.numMemory);
			exit(1);
		}
		printf("Memory[%d]=%lld\n", state.numMemory, state.mem[state.numMemory]);
	}
	printf("\n");
	run(state);
	return(0);
}
void run(stateType state)
{
	int arg0, arg1, arg2;
	int instructions = 0;
	int opcode, opcodeType = 0;
	long long int addressField;
	int maxMem = -1;
	for (; 1; instructions++) {
		printState(&state);
		if (state.pc < 0 || state.pc >= NUMMEMORY) {
			printf("PC went out of the memory range!\n");
			exit(1);
		}
		maxMem = (state.pc > maxMem) ? state.pc : maxMem;
		// This is to make the following code easier to read.
		opcodeType = state.mem[state.pc] >> 41 & 0x01;
		opcode = state.mem[state.pc] >> 36 & 0x1F;
		arg0 = (state.mem[state.pc] >> 30) & 0x3F;
		arg1 = (state.mem[state.pc] >> 24) & 0x3F;
		arg2 = state.mem[state.pc] & 0x3F;
		addressField = convertNum(state.mem[state.pc] & 0xFFFFFF);
		state.pc++;
		if (opcode == ADD) {
			state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
		}
		else if (opcode == NAND) {
			state.reg[arg2] = ~(state.reg[arg0] & state.reg[arg1]);
		}
		else if (opcode == LW) {
			if (state.reg[arg0] + addressField < 0 ||
				state.reg[arg0] + addressField >= NUMMEMORY) {
				printf("Address out of bounds!\n");
				exit(1);
			}
			state.reg[arg1] = state.mem[state.reg[arg0] + addressField];
			if (state.reg[arg0] + addressField > maxMem) {
				maxMem = state.reg[arg0] + addressField;
			}
		}
		else if (opcode == SW) {
			if (state.reg[arg0] + addressField < 0 ||
				state.reg[arg0] + addressField >= NUMMEMORY) {
				printf("Address out of bounds!\n");
				exit(1);
			}
			state.mem[state.reg[arg0] + addressField] = state.reg[arg1];
			if (state.reg[arg0] + addressField > maxMem) {
				maxMem = state.reg[arg0] + addressField;
			}
		}
		else if (opcode == BEQ) {
			if (state.reg[arg0] == state.reg[arg1]) {
				state.pc += addressField;
			}
		}
		else if (opcode == JALR) {
			state.reg[arg1] = state.pc;
			if (arg0 != 0)
				state.pc = state.reg[arg0];
			else
				state.pc = 0;
		}
		else if (opcode == HALT) {
			printf("Machine halted.\n");
			printf("Total of %d instructions executed.\n", instructions + 1);
			printf("Final state of machine:\n");
			printState(&state);
			printf("\n< Created by @Lyhotop >\n");
			exit(0);
		}
		else if (opcode == DEC) {
			state.reg[arg0]--;
		}
		else if (opcode == DIV) {
			if (opcodeType == 0)
			{
				state.reg[arg2] = abs(state.reg[arg0] / state.reg[arg1]);
			}
			else {
				state.reg[state.reg[arg2]] = abs(state.reg[state.reg[arg0]] / state.reg[state.reg[arg1]]);
			}
		}
		else if (opcode == XIMUL) {
			if (opcodeType == 0)
			{
				state.reg[arg2] = state.reg[arg0] * state.reg[arg1];
				long long int temp = state.reg[arg0];
				state.reg[arg0] = state.reg[arg1];
				state.reg[arg1] = temp;
			}
			else {
				state.reg[state.reg[arg2]] = state.reg[state.reg[arg0]] * state.reg[state.reg[arg1]];
				long long temp = state.reg[state.reg[arg0]];
				state.reg[state.reg[arg0]] = state.reg[state.reg[arg1]];
				state.reg[state.reg[arg1]] = temp;
			}
		}
		else if (opcode == XOR) {
			if (opcodeType == 0)
			{
				state.reg[arg2] = state.reg[arg0] ^ state.reg[arg1];
			}
			else {
				state.reg[state.reg[arg2]] = state.reg[state.reg[arg0]] ^ state.reg[state.reg[arg1]];
			}
		}
		else if (opcode == SHL) {
			state.reg[arg2] = state.reg[arg0] << state.reg[arg1];
		}
		else if (opcode == MOV) {
			if (opcodeType == 0) {
				state.reg[arg2] = state.reg[arg0];
			}
			else {
				state.reg[state.reg[arg2]] = state.reg[state.reg[arg0]];

			}
		}
		else if (opcode == JMAE) {
			if (abs(state.reg[arg0]) >= abs(state.reg[arg1])) {
				state.pc += addressField;
			}
		}
		else if (opcode == JMNGE) {
			if (!(abs(state.reg[arg0]) >= abs(state.reg[arg1]))) {
				state.pc += addressField;
			}
		}
		else if (opcode == BT) {
			long long int temp = state.reg[arg0];
			int i;
			for (i = 0; i < state.reg[arg1]; i++)
				temp = temp >> 1;
			temp = temp & 0x000000001;
			state.CF = temp;
		}
		else if (opcode == CMP) {
			if (abs(state.reg[arg0]) < abs(state.reg[arg1])) {
				state.CF = 1;
			}
			else {
				state.CF = 0;
			}
		}
		else if (opcode == RCL) {
			state.reg[arg2] = state.reg[arg0];
			for (int i = 0; i < (state.reg[arg1]); i++) 
			{
				state.CF = (state.reg[arg2] & 0x20) >> 5; 
				state.reg[arg2] = state.reg[arg2] << 1; 

				if (state.CF == 1)
				{
					state.reg[arg2] = state.reg[arg2] | 1; 
				}
			}
		}
		else {
			printf("error: illegal opcode 0x%lli\n", opcode);
			exit(1);
		}

		state.reg[0] = 0;
	}
}
void
printState(stateType* statePtr)
{
	int i;
	printf("\n@@@\nSTATE:\n");
	printf("\tPC: %d\n", statePtr->pc);
	printf("\tMEMORY:\n");
	for (i = 0; i < statePtr->numMemory; i++) {
		printf("\t\tmem[%d] %lld\n", i, statePtr->mem[i]);
	}
	printf("\tRegisters:\n");
	for (i = 0; i < NUMREGS; i++) {
		printf("\t\treg[%lld] %lld\n", i, statePtr->reg[i]);
	}
	printf("CF = %d\n", statePtr->CF);
	printf("END STATE\n");
}
long long int convertNum(int num)
{
	/* convert a 24-bit number into a 64-bit integer */
	if (num & (1 << 23)) {
		num -= (1 << 24);
	}
	return(num);
}