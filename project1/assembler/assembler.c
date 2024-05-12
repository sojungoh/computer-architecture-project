/* Assembler code fragment for LC-2K
 * author: sojung oh (2020042879)
 * final date: 2024-05-12
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

typedef struct {
	char* label;
	int addr;
} symbol;

int PC = -1;
int labelCnt = 0;
symbol* symbolTable[MAXLINELENGTH];

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int convertNum(int num);
int convertOpcodeToNum(char *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

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

	while(1) {
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			/* reached end of file */
			break;
		}

		PC += 1;

		/* TODO: error checking */
		
		/* duplicated labels */
		for(int i = 0; i < labelCnt; ++i) {
			symbol *p = symbolTable[i];
			if(!strcmp(p->label, label)) {
				printf("error: duplicated labels\n");
				exit(1);
			}
		}

		int op = convertOpcodeToNum(opcode);

		/* unrecognized opcodes */
		if(op == -1) {
			printf("error: unrecognized opcodes\n");
			exit(1);
		}
		
		
		/* offesetFields(16bit) overflow */
		if(op == 2 || op == 3 || op == 4) {
			if(isNumber(arg2)) {
				int a = atoi(arg2);
				if(a < -32768 || a > 32767) {
					printf("error: offset overflow\n");
					exit(1);
				} 
			}
		}

		/* register error */
		if(op < 6) {
			if(!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: non-integer register\n");
				exit(1);
			}
			
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7) {
				printf("error: register out of range\n");
				exit(1);
			}

			if(op == 0 || op == 1) {
				if(!isNumber(arg2)) {
					printf("error: non-integer register\n");
					exit(1);
				}
				if(atoi(arg2) < 0 || atoi(arg2) > 7) {
					printf("error: register out of range\n");
					exit(1);
				}
			}
		}

		/* TODO: Phase-1 label calculation */
		/* if there is a label, store its address in symbol table*/
		if(label[0] != '\0') {
			symbolTable[labelCnt] = (symbol*)malloc(sizeof(symbol));
			symbol* p = symbolTable[labelCnt];
			p->label = (char*)malloc(sizeof(label));
			strcpy(p->label, label);
			p->addr = PC;
			labelCnt++;
		}
	}
	

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);
	PC = -1;

	while(1) {
		/* TODO: Phase-2 generate machine codes to outfile */
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			/* reached end of file */
			break;
		}

		/*  error checking
		 *  label undefined 
		 */
		int chk = 0;
		for(; chk < labelCnt; ++chk) {
			symbol *p = symbolTable[chk];
			if(!strcmp(label, p->label)) {
				chk = 1;
				break;
			}
		}
		if(!chk) {
			printf("error: label undefined\n");
			exit(1);
		}

		PC += 1;

		int machine_code = 0;
		int op = convertOpcodeToNum(opcode);
		
		if (op <= 5) {
			int extendOp = op << 22;
			machine_code |= extendOp;		
			
			int warg0 = atoi(arg0);
			int warg1 = atoi(arg1);

			warg0 = warg0 << 19;
			warg1 = warg1 << 16;
			
			machine_code |= warg0;
			machine_code |= warg1;

			int warg2 = 0;
			/* R-type instruction */
			if(op == 0 || op == 1) {
				warg2 = atoi(arg2);
			}
			/* I-type instruction */
			else if(op <= 4) {
				if(isNumber(arg2)) {
					warg2 = atoi(arg2);
					warg2 = convertNum(warg2);	
				}
				else {
					for(int i = 0; i < labelCnt; ++i) {
						symbol *p = symbolTable[i];
						
						if(!strcmp(arg2, p->label)) {
							/* beq offset calculation */
							if(op == 4) {
								// offset = dst - (PC + 1)
								warg2 = p->addr - (PC + 1);
								warg2 = convertNum(warg2);
							}
							else {
								warg2 = p->addr;
							}
							break;
						}
					}
				}
			}
			machine_code |= warg2;
		}
		/* O-type instruction */
		else if(op <= 7) {
			op = op << 22;
			machine_code |= op;	
		}
		/* .fill instruction */
		else {
			if(isNumber(arg0)) {
				machine_code = atoi(arg0);
			}
			else {
				for(int i = 0; i < labelCnt; ++i) {
					symbol *p = symbolTable[i];
					if(!strcmp(arg0, p->label)) {
						machine_code = p->addr;
						break;
					}
				}
			}
		}

		fprintf(outFilePtr, "%d\n", machine_code);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
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
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int convertNum(int num)
{
	int mask = 65535;
	/* convert a 32-bit number into a 16-bit Linux integer */
	if(num & (1 << 31)) {
		num &= mask;
	}
	return (num);
}

int convertOpcodeToNum(char *opcode) {
	int ret;

	if(!strcmp(opcode, "add")) {
		ret = 0;
	}
	else if(!strcmp(opcode, "nor")) {
		ret = 1;
	}
	else if(!strcmp(opcode, "lw")) {
		ret = 2;
	}
	else if(!strcmp(opcode, "sw")) {
		ret = 3;
	}
	else if(!strcmp(opcode, "beq")) {
		ret = 4;
	}
	else if(!strcmp(opcode, "jalr")) {
		ret = 5;
	}
	else if(!strcmp(opcode, "halt")) {
		ret = 6;
	}
	else if(!strcmp(opcode, "noop")) {
		ret = 7;
	}
	else if(!strcmp(opcode, ".fill")) {
		ret = 8;
	}
	else {
		ret = -1;
	}
	return ret;
}
