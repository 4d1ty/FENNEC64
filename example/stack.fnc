.text
    ; Push 10 into the stack
    PSH 10
    ; Push 20 into the stack
    PSH 20
    ; Pop the top of the stack into register A
    POP RA
    ; Pop the top of the stack into register B
    POP RB
    ; Add the two registers and store the result in register A
    ADD RA RB ; RA = RA + RB
    ; Push the result from register A back into the stack
    PSH RA
    ; Output the top of the stack (should be 30)
    OUT
    ; Halt the program
    HLT

; 0 From stack, or none
; 1 From register
; 2 From immediate value
; 3 From memory address
