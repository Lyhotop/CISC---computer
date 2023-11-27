/* Assembler for LC */
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLINELENGTH 1000
#define MAXNUMLABELS 10000
#define MAXLABELLENGTH 15 
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

long long int num;
// Instructions that form machine code.
long long int writeInstructionTypeR(int opcodeNum, int opcodeType, char* arg0, char* arg1, char* arg2);
long long int writeInstructionTypeJ(int opcodeNum, char* arg0, char* arg1);
long long int writeInstructionTypeO(int opcodeNum);
long long int writeInstructionTypeM(int opcodeNum, char* arg0);
long long int writeInstructionTypeY(int opcodeNum, int opcodeType, char* arg0, char* arg2);
long long int writeInstructionTypeI(int opcodeNum, char* arg0, char* arg1, int addressField);

int readAndParse(FILE*, char*, char*, char*, char*, char*);
int translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH], int labelAddress[MAXNUMLABELS], int, char*);
int isNumber(char*);
void testRegArg(char*);
void testAddrArg(char*);

int main(int argc, char* argv[])
{
	char* inFileString, * outFileString;
	FILE* inFilePtr, * outFilePtr;
	int address;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH], argTmp[MAXLINELENGTH];
	int i;
	int numLabels = 0;
	int addressField;
	char labelArray[MAXNUMLABELS][MAXLABELLENGTH];
	int labelAddress[MAXNUMLABELS];

	if (argc != 3) {
		printf("Error: usage: %s <assembly-code-file> <machine-code-file>!\n",
			argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("Error in opening: %s!\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("Error in opening: %s!\n", outFileString);
		exit(1);
	}

	// Map symbols to addresses.
	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); address++) {
		// Check for illegal opcode.
		if (strcmp(opcode, "add") && strcmp(opcode, "nand") &&
			strcmp(opcode, "lw") && strcmp(opcode, "sw") &&
			strcmp(opcode, "beq") && strcmp(opcode, "jalr") &&
			strcmp(opcode, "halt") && strcmp(opcode, "dec") &&
			strcmp(opcode, "div") && strcmp(opcode, "ximul") &&
			strcmp(opcode, "xor") && strcmp(opcode, "shl") &&
			strcmp(opcode, "mov") && strcmp(opcode, "jmae") &&
			strcmp(opcode, "jmnge") && strcmp(opcode, "bt") &&
			strcmp(opcode, "cmp") && strcmp(opcode, "rcl") &&
			strcmp(opcode, "#ximul") && strcmp(opcode, "#xor") &&
			strcmp(opcode, "#mov") && strcmp(opcode, ".fill")) {
			printf("Error: unrecognized opcode %s at address %d!\n", opcode, address);
			exit(1);
		}
		// Check register fields.
		if (!strcmp(opcode, "add") || !strcmp(opcode, "nand") ||
			!strcmp(opcode, "lw") || !strcmp(opcode, "sw") ||
			!strcmp(opcode, "beq") || !strcmp(opcode, "jalr") ||
			!strcmp(opcode, "div") ||
			!strcmp(opcode, "ximul") ||
			!strcmp(opcode, "xor") ||
			!strcmp(opcode, "shl") ||
			!strcmp(opcode, "jmae") ||
			!strcmp(opcode, "jmnge") ||
			!strcmp(opcode, "bt") ||
			!strcmp(opcode, "cmp") ||
			!strcmp(opcode, "rcl")) {
			testRegArg(arg0);
			testRegArg(arg1);
		}
		if (!strcmp(opcode, "add") || !strcmp(opcode, "nand") ||
			!strcmp(opcode, "div") || !strcmp(opcode, "ximul") ||
			!strcmp(opcode, "xor") || !strcmp(opcode, "mov") ||
			!strcmp(opcode, "shl") || !strcmp(opcode, "rcl")) {
			testRegArg(arg2);
		}
		if (!strcmp(opcode, "dec") || !strcmp(opcode, "mov")) {
			testRegArg(arg0);
		}

		// Check addressField.
		if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") ||
			!strcmp(opcode, "beq") || !strcmp(opcode, "jmb") ||
			!strcmp(opcode, "jmae") || !strcmp(opcode, "jmnge")) {
			testAddrArg(arg2);
		}
		if (!strcmp(opcode, ".fill")) {
			testAddrArg(arg0);
		}
		// Check for enough arguments.
		if ((strcmp(opcode, "halt")
			&& strcmp(opcode, ".fill") && strcmp(opcode, "jalr")
			&& strcmp(opcode, "dec") && strcmp(opcode, "bt")
			&& strcmp(opcode, "cmp") && arg2[0] == '\0') ||
			(!strcmp(opcode, "jalr") && arg1[0] == '\0') ||
			(!strcmp(opcode, "cmp") && arg1[0] == '\0') ||
			(!strcmp(opcode, "bt") && arg1[0] == '\0') ||
			(!strcmp(opcode, ".fill") && arg0[0] == '\0') ||
			(!strcmp(opcode, "dec") && arg0[0] == '\0')) {
			printf("Error at address %d: not enough arguments!\n", address);
			exit(2);
		}

		if (label[0] != '\0') {
			// Check for labels that are too long.
			if (strlen(label) >= MAXLABELLENGTH) {
				printf("Label %d too long!\n", address);
				exit(2);
			}

			// Make sure label starts with letter.
			if (!sscanf(label, "%[a-zA-Z]", argTmp)) {
				printf("Label doesn't start with letter!\n");
				exit(2);
			}

			// Make sure label consists of only letters and numbers.
			sscanf(label, "%[a-zA-Z0-9]", argTmp);
			if (strcmp(argTmp, label)) {
				printf("Label has character other than letters and numbers!\n");
				exit(2);
			}

			// Look for duplicate label.
			for (i = 0; i < numLabels; i++) {
				if (!strcmp(label, labelArray[i])) {
					printf("Error: duplicate label %s at address %d!\n",
						label, address);
					exit(1);
				}
			}
			// See if there are too many labels.
			if (numLabels >= MAXNUMLABELS) {
				printf("Error: too many labels (label=%s)!\n", label);
				exit(2);
			}
			strcpy(labelArray[numLabels], label);
			labelAddress[numLabels++] = address;
		}
	}


	// Print machine code, with symbols filled in as addresses.
	rewind(inFilePtr);
	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
		address++) {

		if (!strcmp(opcode, "add")) {
			num = writeInstructionTypeR(ADD, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "nand")) {
			num = writeInstructionTypeR(NAND, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "div")) {
			num = writeInstructionTypeR(DIV, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "ximul")) {
			num = writeInstructionTypeR(XIMUL, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "#ximul")) {
			num = writeInstructionTypeR(XIMUL, 1, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "xor")) {
			num = writeInstructionTypeR(XOR, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "#xor")) {
			num = writeInstructionTypeR(XOR, 1, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "shl")) {
			num = writeInstructionTypeR(SHL, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "rcl")) {
			num = writeInstructionTypeR(RCL, 0, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "jalr")) {
			num = writeInstructionTypeJ(JALR, arg0, arg1);
		}
		else if (!strcmp(opcode, "bt")) {
			num = writeInstructionTypeJ(BT, arg0, arg1);
		}
		else if (!strcmp(opcode, "cmp")) {
			num = writeInstructionTypeJ(CMP, arg0, arg1);
		}
		else if (!strcmp(opcode, "halt")) {
			num = writeInstructionTypeO(HALT);
		}
		else if (!strcmp(opcode, "dec")) {
			num = writeInstructionTypeM(DEC, arg0);
		}
		else if (!strcmp(opcode, "mov")) {
			num = writeInstructionTypeY(MOV, 0, arg0, arg2);
		}
		else if (!strcmp(opcode, "#mov")) {
			num = writeInstructionTypeY(MOV, 1, arg0, arg2);
		}
		else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") ||
			!strcmp(opcode, "jmae") || !strcmp(opcode, "jmnge")
			|| !strcmp(opcode, "beq")) {
			/* if arg2 is symbolic, then translate into an address */
			if (!isNumber(arg2)) {
				addressField = translateSymbol(labelArray, labelAddress, numLabels, arg2);
				if (!strcmp(opcode, "beq") || !strcmp(opcode, "jmae")
					|| !strcmp(opcode, "jmnge")) {
					addressField = addressField - address - 1;
				}
			}
			else {
				addressField = atoi(arg2);
			}
			if (addressField < -8388608 || addressField > 8388607) {
				printf("Error: offset %d out of range!\n", addressField);
				exit(1);
			}
			// Truncate the offset field, in case it's negative.
			addressField = addressField & 0x7FFFFF;
			if (!strcmp(opcode, "jmae")) {

				num = writeInstructionTypeI(JMAE, arg0, arg1, addressField);
			}
			else if (!strcmp(opcode, "jmnge")) {
				num = writeInstructionTypeI(JMNGE, arg0, arg1, addressField);
			}
			else if (!strcmp(opcode, "beq")) {
				num = writeInstructionTypeI(BEQ, arg0, arg1, addressField);
			}
			else if (!strcmp(opcode, "lw")) {
				num = writeInstructionTypeI(LW, arg0, arg1, addressField);
			}
			else if (!strcmp(opcode, "sw")) {
				num = writeInstructionTypeI(SW, arg0, arg1, addressField);
			}
		}
		else if (!strcmp(opcode, ".fill")) {
			if (!isNumber(arg0)) {
				num = translateSymbol(labelArray, labelAddress, numLabels, arg0);
			}
			else {
				num = atoi(arg0);
			}
		}
		fprintf(outFilePtr, "%lld\n", num);
	}
	exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0, char* arg1, char* arg2)
{
	char line[MAXLINELENGTH];
	char* ptr = line;
	/* Delete prior values. */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
	/* Read the line from the assembly-language file. */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* Reached end of file. */
		return(0);
	}
	/* Check for line too long. */
	if (strlen(line) == MAXLINELENGTH - 1) {
		printf("Error: line too long!\n");
		exit(1);
	}

	/* Is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n ]", label)) {
		/* Successfully read label; advance pointer over the label. */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		opcode, arg0, arg1, arg2);
	return(1);
}

int translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH], int labelAddress[MAXNUMLABELS], int numLabels, char* symbol)
{
	int i;
	/* Search through address label table. */
	for (i = 0; i < numLabels && strcmp(symbol, labelArray[i]); i++) {}
	if (i >= numLabels) {
		printf("Error: missing label %s!\n", symbol);
		exit(1);
	}
	return(labelAddress[i]);
}

int isNumber(char* string)
{
	/* Return 1 if string is a number. */
	int i;
	return((sscanf(string, "%d", &i)) == 1);
}

/* Test register argument; make sure it's in range and has no bad characters. */
void testRegArg(char* arg)
{
	int num;
	char c;
	if (atoi(arg) < 0 || atoi(arg) > 63) {
		printf("Error: register out of range!\n");
		exit(2);
	}
	if (sscanf(arg, "%d%c", &num, &c) != 1) {
		printf("Bad character in register argument!\n");
		exit(2);
	}
}

/* Test addressField argument. */
void testAddrArg(char* arg)
{
	int num;
	char c;
	/* Test numeric addressField */
	if (isNumber(arg)) {
		if (sscanf(arg, "%d%c", &num, &c) != 1) {
			printf("Bad character in addressField!\n");
			exit(2);
		}
	}
}

// Functions that form machine codes.
long long int writeInstructionTypeR(int opcodeNum, int opcodeType, char* arg0, char* arg1, char* arg2) {
	return num = ((long long int)opcodeType << 41) | ((long long int)opcodeNum << 36) |
		((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24) | atoi(arg2);
}

long long int writeInstructionTypeI(int opcodeNum, char* arg0, char* arg1, int addressField) {
	return num = ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24) | addressField;
}

long long int writeInstructionTypeJ(int opcodeNum, char* arg0, char* arg1) {
	return num = ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24);
}

long long int writeInstructionTypeO(int opcodeNum) {
	return num = ((long long int)HALT << 36);
}

long long int writeInstructionTypeM(int opcodeNum, char* arg0) {
	return num = ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30);
}
long long int writeInstructionTypeY(int opcodeNum, int opcodeType, char* arg0, char* arg2) {
	return num = ((long long int)opcodeType << 41) | ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30) | atoi(arg2);
}