.data
    H: 72
    I: 73
    EXCLAMATION: 33
    NEWLINE: 10

.text

MAIN:
    CALL PRINT_HI
    HLT

PRINT_HI:
    OUTC [H]
    OUTC [I]
    OUTC [EXCLAMATION]
    OUTC [NEWLINE]
    RET

