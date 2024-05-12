        lw 0 1 staddr   reg1 = mem[staddr] = addr
        lw 0 2 num1     reg2 = mem[num1] = 1
        lw 0 5 num5     reg5 = 5
for1    sw 1 3 0        mem[reg1] = reg3 addr = 0
        add 1 2 1       reg1 = reg1 + reg2 
        add 3 2 3       reg3 = reg3 + reg2 = 1
        beq 3 5 end1
        beq 0 0 for1
end1    lw 0 1 staddr   reg1 = mem[staddr] = addr
        lw 0 4 num4     reg4 = 4
for2    lw 1 3 0        reg3 = mem[reg1]
        beq 3 4 done
        add 1 2 1       reg1 = reg1 + 1
        beq 0 0 for2 
done    halt       
staddr .fill addr
num1   .fill 1
num4   .fill 4
num5   .fill 5 
addr   .fill 0
       .fill 0
       .fill 0
       .fill 0
       .fill 0
