///*
//
///*
// * Instruction-level simulator for the LC
// */
//
//#define _CRT_SECURE_NO_WARNINGS
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <vector>
//
//#define NUMMEMORY 16777216 /* maximum number of words in memory */
//#define NUMREGS 64 /* number of machine registers */
//#define MAXLINELENGTH 1000
//

//
//typedef struct stateStruct {
//	long long int pc;
//	long long int* mem = new long long int[NUMMEMORY];
//	long long int reg[NUMREGS];
//	int numMemory;
//	int CF;
//	int SF;
//	int ZF;
//} stateType;
//
//void printState(stateType*);
//void run(stateType);
//int convertNum(int);
//
//int
//temp(int argc, char* argv[])
//{
//	int i;
//	char line[MAXLINELENGTH];
//	stateType state;
//	FILE* filePtr;
//
//
//	if (argc != 2) {
//		printf("Error: usage: %s <machine-code file>!\n", argv[0]);
//		exit(1);
//	}
//
//	/* initialize memories and registers */
//	for (i = 0; i < NUMMEMORY; i++) {
//		state.mem[i] = 0;
//	}
//	for (i = 0; i < NUMREGS; i++) {
//		state.reg[i] = 0;
//	}
//	state.pc = 0;
//	state.CF = 0;
//	state.SF = 0;
//	state.ZF = 0;
//
//	/* read machine-code file into instruction/data memory (starting at
//	address 0) */
//
//	filePtr = fopen(argv[1], "r");
//	if (filePtr == NULL) {
//		printf("Error: can't open file %s!\n", argv[1]);
//		perror("fopen");
//		exit(1);
//	}
//
//	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
//		state.numMemory++) {
//		if (state.numMemory >= NUMMEMORY) {
//			printf("Exceeded memory size\n");
//			exit(1);
//		}
//		if (sscanf(line, "%I64d", state.mem + state.numMemory) != 1) {
//			printf("Error in reading address %d\n", state.numMemory);
//			exit(1);
//		}
//		printf("memory[%d]=%I64d\n", state.numMemory, state.mem[state.numMemory]);
//	}
//
//	printf("\n");
//
//	/* run never returns */
//	run(state);
//
//	return(0);
//}
//
//void run(stateType state)
//{
//	long long int arg0, arg1, arg2, addressField;
//	int instructions = 0;
//	int opcode, opcodeType = 0;
//	long long int maxMem = -1;	/* highest memory address touched during run */
//
//	for (; 1; instructions++) { /* infinite loop, exits when it executes halt */
//		printState(&state);
//
//		if (state.pc < 0 || state.pc >= NUMMEMORY) {
//			printf("pc went out of the memory range\n");
//			exit(1);
//		}
//
//		maxMem = (state.pc > maxMem) ? state.pc : maxMem;
//
//		/* this is to make the following code easier to read */
//		opcode = state.mem[state.pc] >> 36;
//		arg0 = (state.mem[state.pc] >> 30) & 0x3F;
//		arg1 = (state.mem[state.pc] >> 24) & 0x3F;
//		arg2 = state.mem[state.pc] & 0x3F; /* only for add, nand */
//
//		addressField = convertNum(state.mem[state.pc] & 0xFFFFF);
//
//
//		state.pc++;
//		/*for (int i = 0; i < NUMREGS; i++) {
//			if (state.reg[i] < -9223372036854775808 || state.reg[i] > 9223372036854775807)
//			{
//				printf("error: grid overflow at %d register\n", i);
//				exit(1);
//			}
//		}*/

//
//void
//printState(stateType* statePtr)
//{
//	int i;
//	printf("\n@@@\nstate:\n");
//	printf("\tpc %I64d\n", statePtr->pc);
//	printf("\tmemory:\n");
//	for (i = 0; i < statePtr->numMemory; i++) {
//		printf("\t\tmem[ %d ] %I64d\n", i, statePtr->mem[i]);
//	}
//	printf("\tregisters:\n");
//	for (i = 0; i < NUMREGS; i++) {
//		printf("\t\treg[ %d ] %I64d\n", i, statePtr->reg[i]);
//	}
//	printf("\tflag:\n");
//	printf("\t\tCF %d\n", statePtr->CF);
//	printf("end state\n");
//}
//
//int
//convertNum(int num)
//{
//	/* convert a 16-bit number into a 32-bit Sun integer */
//	if (num & (1 << 19)) {
//		num -= (1 << 20);
//	}
//	return(num);
//}
//
//
//
//
//*/