start   lw      0      1       two      test duplicated labels
        lw      1      2       3
start   add     1      0       1
        beq     1      1       1
        jalr    1      2      
done    halt
two     .fill   2
