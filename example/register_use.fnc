.data
    X: 69
    Y: 42

.text
PSH 29
LOAD RA [X] ; The assembler will replace X with 69
PSH [Y] ; The assembler will replace Y with 42
OUT RA ; should output 69 from register A
OUT ; should output 0 from top of the stack
HLT

; 0 From stack, or none
; 1 From register
; 2 From immediate value
; 3 From memory address