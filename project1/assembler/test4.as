        lw      0   1   num2        reg1 = mem[num2]
        lw      0   2   num100      reg2 = mem[num100]
        beq     0   1   case1       if reg1 == 0 goto case1
        lw      0   3   num1        reg3 = mem[num1]
        beq     1   3   case2       if reg1 == reg3 goto case2
        lw      0   3   num2        reg3 = mem[num2]
        beq     1   3   case3       if reg1 == reg3 goto case3
        add     0   0   1           reg1 = 0
        beq     0   0   done        goto done
case1   add     0   2   1           reg1 = reg2
        beq     0   0   done        goto done
case2   noop                        do nothing
        beq     0   0   done        goto done
case3   add     1   2   1           reg1 = reg1 + reg2
        beq     0   0   done        goto done
done    halt                        end of program
num1    .fill   1
num2    .fill   2
num100  .fill   100
