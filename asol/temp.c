/* Assembler for LC 
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLINELENGTH 1000
#define MAXNUMLABELS 10000
#define MAXLABELLENGTH 15 /* includes the null character termination 

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
#define STORE 18
#define SETIR 19
#define SETBR 20

long long int writeInstructionTypeR(int opcodeNum, char* arg0, char* arg1, char* arg2) {
	long long int num;

	char* delimiter = strchr(arg2, '#'); // Пошук символу "#"

	if (delimiter != NULL) {
		// Знайдено символ "#"
		strcpy(arg2, delimiter + 1);
		num = ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24)
			| ((long long int)1 << 6) | atoi(arg2); // Починаємо перетворення після символу "#"
	}
	else {
		// Символ "#"" не знайдено, використовуємо ціле arg2
		num = ((long long int)opcodeNum << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24)
			| atoi(arg2);
	}
	// | ((long long int)1 << 6)


};
int readAndParse(FILE*, char*, char*, char*, char*, char*);
int translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH], int labelAddress[MAXNUMLABELS], int, char*);
int isNumber(char*);
void testRegArg(char*);
void testRegArgNew(char*);
void testAddrArg(char*);

long long int binary(long long int num) {
	long long int binaryNumber = 0, remainder, base = 1;

	while (num > 0) {
		remainder = num % 2;
		binaryNumber = binaryNumber + remainder * base;
		num = num / 2;
		base = base * 10;
	}

	return binaryNumber;
}

int
main(int argc, char* argv[])
{
	char* inFileString, * outFileString;
	FILE* inFilePtr, * outFilePtr;
	int address;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH], argTmp[MAXLINELENGTH];
	int i;
	int numLabels = 0;
	long long int num;
	int addressField;

	char labelArray[MAXNUMLABELS][MAXLABELLENGTH];
	int labelAddress[MAXNUMLABELS];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
			argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* map symbols to addresses 

	/* assume address start at 0 
	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
		address++) {
		/*
		printf("%d: label=%s, opcode=%s, arg0=%s, arg1=%s, arg2=%s\n",
			address, label, opcode, arg0, arg1, arg2);
		*/

		/* check for illegal opcode 
		if (strcmp(opcode, "add") && strcmp(opcode, "nand") &&
			strcmp(opcode, "lw") && strcmp(opcode, "sw") &&
			strcmp(opcode, "beq") && strcmp(opcode, "jalr") &&
			strcmp(opcode, "halt") && strcmp(opcode, "dec") &&
			strcmp(opcode, "div") && strcmp(opcode, "ximul") &&
			strcmp(opcode, "xor") && strcmp(opcode, "shl") &&
			strcmp(opcode, "mov") && strcmp(opcode, "jmae") &&
			strcmp(opcode, "jmnge") && strcmp(opcode, "bt") &&
			strcmp(opcode, "cmp") && strcmp(opcode, "rcl") &&
			strcmp(opcode, ".fill")) {
			printf("error: unrecognized opcode %s at address %d\n", opcode, address);
			exit(1);
		}

		/* check register fields 
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
			!strcmp(opcode, "div") || !strcmp(opcode, "shl") ||
			!strcmp(opcode, "rcl")) {
			testRegArg(arg2);
		}
		if (!strcmp(opcode, "dec") || !strcmp(opcode, "mov")) {
			testRegArg(arg0);
		}
		if (!strcmp(opcode, "ximul") || !strcmp(opcode, "xor") || !strcmp(opcode, "mov")) {
			testRegArgNew(arg2);
		}

		/* check addressField 
		if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") ||
			!strcmp(opcode, "beq") || !strcmp(opcode, "jmb") ||
			!strcmp(opcode, "jmae") || !strcmp(opcode, "jmnge")) {
			testAddrArg(arg2);
		}
		if (!strcmp(opcode, ".fill")) {
			testAddrArg(arg0);
		}


		/* check for enough arguments *
		if ((strcmp(opcode, "halt")
			&& strcmp(opcode, ".fill") && strcmp(opcode, "jalr")
			&& strcmp(opcode, "dec") && strcmp(opcode, "bt")
			&& strcmp(opcode, "cmp") && arg2[0] == '\0') ||
			(!strcmp(opcode, "jalr") && arg1[0] == '\0') ||
			(!strcmp(opcode, "cmp") && arg1[0] == '\0') ||
			(!strcmp(opcode, "bt") && arg1[0] == '\0') ||
			(!strcmp(opcode, ".fill") && arg0[0] == '\0') ||
			(!strcmp(opcode, "dec") && arg0[0] == '\0')) {
			printf("error at address %d: not enough arguments\n", address);
			exit(2);
		}

		if (label[0] != '\0') {
			/* check for labels that are too long *
			if (strlen(label) >= MAXLABELLENGTH) {
				printf("label %d too long\n", address);
				exit(2);
			}

			/* make sure label starts with letter *
			if (!sscanf(label, "%[a-zA-Z]", argTmp)) {
				printf("label doesn't start with letter\n");
				exit(2);
			}

			/* make sure label consists of only letters and numbers *
			sscanf(label, "%[a-zA-Z0-9]", argTmp);
			if (strcmp(argTmp, label)) {
				printf("label has character other than letters and numbers\n");
				exit(2);
			}

			/* look for duplicate label *
			for (i = 0; i < numLabels; i++) {
				if (!strcmp(label, labelArray[i])) {
					printf("error: duplicate label %s at address %d\n",
						label, address);
					exit(1);
				}
			}
			/* see if there are too many labels *
			if (numLabels >= MAXNUMLABELS) {
				printf("error: too many labels (label=%s)\n", label);
				exit(2);
			}

			strcpy(labelArray[numLabels], label);
			labelAddress[numLabels++] = address;
		}
	}


	/* now do second pass (print machine code, with symbols filled in as
		addresses) */
		/* now do second pass (print machine code, with symbols filled in as
		addresses) 
	rewind(inFilePtr);

	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
		address++) {
		if (!strcmp(opcode, "add")) {
			num = writeInstructionTypeR(ADD, arg0, arg1, arg2);
			//num = ((long long int)ADD << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24)| atoi(arg2);
			printf("ADD: %lld\n", num);
		}
		else if (!strcmp(opcode, "nand")) {
			num = ((long long int)NAND << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24) | atoi(arg2);
			printf("ADD: %lld\n", num);
		}
		else if (!strcmp(opcode, "div")) {
			num = writeInstructionTypeR(DIV, arg0, arg1, arg2);
			printf("div: %lld\n", num);
		}
		else if (!strcmp(opcode, "ximul")) {
			num = writeInstructionTypeR(XIMUL, arg0, arg1, arg2);
			//num = ((long long int)ADD << 36) | ((long long int)atoi(arg0) << 30) | ((long long int)atoi(arg1) << 24)| atoi(arg2);
			printf("ADD: %lld\n", num);
		}
		else if (!strcmp(opcode, "xor")) {
			num = writeInstructionTypeR(XOR, arg0, arg1, arg2);
		}
		else if (!strcmp(opcode, "jalr")) {

			num = (JALR << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17);
		}
		else if (!strcmp(opcode, "inc")) {
			num = (ADD << 25) | (atoi(arg0) << 21);
		}
		else if (!strcmp(opcode, "ximul")) {
			num = (XIMUL << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
			printf("Ximul: %d\n", num);
		}
		/*else if (!strcmp(opcode, "xsub")) {
			num = (XSUB << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
		}
		else if (!strcmp(opcode, "xor")) {
			num = (XOR << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
		}
		else if (!strcmp(opcode, "shl")) {
			num = (SHL << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
		}
		else if (!strcmp(opcode, "cmple")) {
			num = (CMPLE << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
			38
		}
		else if (!strcmp(opcode, "cmp")) {
			num = (CMP << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17);
		}
		else if (!strcmp(opcode, "rcr")) {
			num = (RCR << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
				| atoi(arg2);
		}
		else if (!strcmp(opcode, "stc")) {
			num = (STC << 25);
		}
		else if (!strcmp(opcode, "setIR")) {
			num = (SETIR << 25) | (atoi(arg0) << 21);
		}
		else if (!strcmp(opcode, "setBR")) {
			num = (SETBR << 25) | (atoi(arg0) << 21);
		}*
		else if (!strcmp(opcode, "halt")) {
			num = (HALT << 25);
			printf("halt: %d\n", num);
		}
		else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") ||
			!strcmp(opcode, "jmb") || !strcmp(opcode, "jmne")
			|| !strcmp(opcode, "beq")) {
			/* if arg2 is symbolic, then translate into an address *
			if (!isNumber(arg2)) {
				addressField = translateSymbol(labelArray, labelAddress,
					numLabels, arg2);
				/*
				printf("%s being translated into %d\n", arg2, addressField);
				*
				if (!strcmp(opcode, "beq") || !strcmp(opcode, "jmb")
					|| !strcmp(opcode, "jmne")) {
					addressField = addressField - address - 1;
				}
			}
			else {
				addressField = atoi(arg2);
			}
			if (addressField < -32768 || addressField > 32767) {
				printf("error: offset %d out of range\n", addressField);
				exit(1);
			}
			/* truncate the offset field, in case it's negative *
			addressField = addressField & 0xFFFF;
			if (!strcmp(opcode, "jmb")) {
				num = (JMAE << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
					| addressField;
			}
			else if (!strcmp(opcode, "jmne")) {
				num = (JMNGE << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
					| addressField;
			}
			else if (!strcmp(opcode, "beq")) {
				num = (BEQ << 25) | (atoi(arg0) << 21) | (atoi(arg1) << 17)
					| addressField;
			}
			else {
				/* lw or sw *
				if (!strcmp(opcode, "lw")) {
					num = (LW << 25) | (atoi(arg0) << 21) |
						(atoi(arg1) << 17) | addressField;

				}

				else {
					num = (SW << 25) | (atoi(arg0) << 21) |
						(atoi(arg1) << 17) | addressField;
				}
			}
		}
		//***************************************************
		else if (!strcmp(opcode, "load") || !strcmp(opcode, "store")) { ///////
			/* if arg1 is symbolic, then translate into an address *
			if (!isNumber(arg1)) {
				addressField = translateSymbol(labelArray, labelAddress,
					numLabels, arg1);
			}
			else {
				addressField = atoi(arg1);
			}
			if (addressField < -32768 || addressField > 32767) {
				printf("error: offset %d out of range\n", addressField);
				exit(1);
			}
			/* truncate the offset field, in case it's negative *
			addressField = addressField & 0xFFFF;
			if (!strcmp(opcode, "load")) {
				num = (ADD << 25) | (atoi(arg0) << 21) | addressField;
			}
			else {
				num = (STORE << 25) | (atoi(arg0) << 21) | addressField;
			}
		}
		//***************************************************
		else if (!strcmp(opcode, ".fill")) {
			if (!isNumber(arg0)) {
				num = translateSymbol(labelArray, labelAddress, numLabels,
					arg0);
			}
			else {
				num = atoi(arg0);
			}
		}
		// printf("(address %d): %d (hex 0x%x)\n", address, num, num); 
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
 *
int
readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0,
	char* arg1, char* arg2)
{
	char line[MAXLINELENGTH];
	char* ptr = line;

	/* delete prior values *
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file *
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file *
		return(0);
	}

	/* check for line too long *
	if (strlen(line) == MAXLINELENGTH - 1) {
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? *
	ptr = line;
	if (sscanf(ptr, "%[^\t\n ]", label)) {
		/* successfully read label; advance pointer over the label *
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 *
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		opcode, arg0, arg1, arg2);
	return(1);
}

int
translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH],
	int labelAddress[MAXNUMLABELS], int numLabels, char* symbol)
{
	int i;

	/* search through address label table *
	for (i = 0; i < numLabels && strcmp(symbol, labelArray[i]); i++) {
	}

	if (i >= numLabels) {
		printf("error: missing label %s\n", symbol);
		exit(1);
	}

	return(labelAddress[i]);
}

int
isNumber(char* string)
{
	/* return 1 if string is a number *
	int i;
	return((sscanf(string, "%d", &i)) == 1);
}

/*
 * Test register argument; make sure it's in range and has no bad characters.
 *
void testRegArg(char* arg)
{
	int num;
	char c;

	if (atoi(arg) < 0 || atoi(arg) > 63) {
		printf("error: register out of range\n");
		exit(2);
	}
	if (sscanf(arg, "%d%c", &num, &c) != 1) {
		printf(" problem on arg\n");
		exit(2);
	}
}

void testRegArgNew(char* arg) {
	int num;
	char c;

	// Check if the input starts with '#', and if so, skip it
	if (arg[0] == '#') {
		arg++;  // Move the pointer one position forward to skip the '#'
	}
	// Convert the remaining string to an integer
	num = atoi(arg);
	// Check if the converted number is within the valid range
	if (atoi(arg) < 0 || atoi(arg) > 63) {
		printf("error: register out of range\n");
		exit(2);
	}
	if (sscanf(arg, "%d%c", &num, &c) != 1) {
		printf(" problem!\n");
		exit(2);
	}
}

/*
 * Test addressField argument.
 *
void testAddrArg(char* arg)
{
	int num;
	char c;

	/* test numeric addressField *
	* 
	if (isNumber(arg)) {
		if (sscanf(arg, "%d%c", &num, &c) != 1) {
			printf("bad character in addressField\n");
			exit(2);
		}
	}
}
*/