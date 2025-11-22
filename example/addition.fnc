.text
    ; Push 67 into the stack
    LOAD RA 67
    ; Push 21 into the stack
    LOAD RB -21
    ; Add all the value in the stack and now the top of the stack is the sum of the previous values
    ADD RA RB ; RA = RA + RB
    ; Prints out the top of the stack
    OUT RA
    OUT [4096]
    ; Stops the furthers execution, this instruction is required
    HLT

; 0 From stack, or none
; 1 From register
; 2 From immediate value
; 3 From memory address