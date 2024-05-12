        lw      0   4   neg1        reg4 = mem[neg1]
	lw      0   5   num1        reg5 = mem[num1]
	lw      0   6   staddr      reg6 = mem[staddr] = 21
	lw      0   1   n           reg1 = mem[n]
for1    add     1   4   1           reg1 = reg1 + reg4
	beq     1   0   done1       if reg1 == 0 goto done1
for2    beq     2   1   done2       if reg1 == reg2 goto done2
        lw      6   3   0           reg3 = mem[reg6]
        lw      6   7   1           reg7 = mem[reg6 + 1]
        sw      6   7   0           mem[reg6] = reg7
        sw      6   3   1           mem[reg6 + 1] = reg3
        add     2   5   2           reg2 = reg2 + reg5
        add     6   5   6           reg6 = reg6 + reg5
        beq     0   0   for2        goto for2
done2   add     0   0   2           reg2 = 0
        lw      0   6   staddr      reg6 = mem[staddr] = 21
        beq     0   0   for1        goto for1
done1   halt                        end of the program
staddr  .fill   list
num1    .fill   1
neg1    .fill   -1
n       .fill   5
list    .fill   4
    	.fill   2
	.fill   1
	.fill   3
	.fill   5
