/* LC-2K Instruction-level simulator 
 * author: sojung oh (2020042879)
 * final date: 2024-05-11
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    /* initialize all registers and the program counter to 0*/
    state.pc = 0;
    for(int i = 0; i < NUMREGS; ++i)
        state.reg[i] = 0;
    
    int instCnt = 0;

    while(1) {
        printState(&state);

        fflush(stdout);

        /* calculate opcode */
        int inst = state.mem[state.pc];
        int op = inst >> 22;

        /* calculate fields */
        int ra, rb, rd, offset;
        ra = (inst >> 19) & 7;
        rb = (inst >> 16) & 7;
        
        if(op <= 1)
            rd = inst & 7;
        else if(op <= 4)
            offset = convertNum(inst & 65535);
        
        /* perform operation */
        switch(op) {
            case 0: // add
                state.reg[rd] = state.reg[ra] + state.reg[rb];
                state.pc += 1;
                break;
            case 1: // nor
                state.reg[rd] = ~(state.reg[ra] | state.reg[rb]);
                state.pc += 1;
                break;
            case 2: // lw
                state.reg[rb] = state.mem[state.reg[ra] + offset];
                state.pc += 1;
                break;
            case 3: // sw
                state.mem[state.reg[ra] + offset] = state.reg[rb];
                state.pc += 1;
                break;
            case 4: // beq
                state.pc = (state.reg[ra] == state.reg[rb]) ? state.pc + 1 + offset : state.pc + 1;
                break;
            case 5: // jalr
                state.reg[rb] = state.pc + 1;
                state.pc = state.reg[ra];
                break;
            case 6: // halt
                state.pc += 1;
                break;
            case 7: // noop
                break;
            default:
                fprintf(stderr, "unrecognized opcode");
                exit(1);
        }

        instCnt += 1;

        /* halt a program */
        if(op == 6) {
            printf("machine halted\n");
            printf("total of %d instructions executed\n", instCnt);
            printf("final state of machine:\n");
            printState(&state);
            break;
        }
    }
    
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
