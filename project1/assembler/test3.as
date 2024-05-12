        lw      0   1   num3    reg1 = mem[num3] = 3
        lw      0   2   num5    reg2 = mem[num5] = 5
        lw      0   3   staddr  reg3 = 8
        sw      0   1   arg1    mem[arg1] = reg1
        sw      0   2   arg2    mem[arg2] = reg2
        jalr    3   4           reg4 = PC + 1 goto reg3
        lw      0   5   ret     reg5 = mem[ret]
        beq     0   0   5       goto PC + 1 + 5
func    lw      0   1   arg1    reg1 = mem[arg1]
        lw      0   2   arg2    reg2 = mem[arg2]
        add     1   2   3       reg3 = reg1 + reg2
        sw      0   3   ret     mem[ret] = reg3
        jalr    4   3           reg3 = PC + 1 goto reg4
        halt                    end of program
num3    .fill   3
num5    .fill   5
staddr  .fill   func
arg1    .fill   0
arg2    .fill   0    
ret     .fill   0
