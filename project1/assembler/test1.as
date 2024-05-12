	lw      0   1   num1    load reg1 with 15 (symbolic address)
        lw      0   2   11      load reg2 with 85 (numberic address)
        add     0   3   0       reg3 = 0
        lw      0   4   num3    load reg4 with -96
start   nor     1   2   5       reg5 = reg1 nor reg2
        beq     4   5   2       if reg4 = reg5, goto PC + 1 + 2
        lw      0   6   9       load reg6 with 9
        jalr    6   7           store PC + 1 to reg7, goto reg6(9) address
        add     3   5   3       add reg3 = reg3 + reg5
done    halt                    end of program
num1    .fill   15
num2    .fill   85
num3    .fill   -96
