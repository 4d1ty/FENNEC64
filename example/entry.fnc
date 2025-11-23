; A simple assembly program to show the entry point
; .MAIN indicates the main entry point of the program, by default, all the code in .text section is in the MAIN entry point

.data
    ; This data section is defaulted to the MAIN entry point
    X: 20
    Y: 40

; The current section is still .data, to change it to text use .text directive

.text
    MAIN:
        OUT 60 ; Output the value 60
        ADD RA [X] ; Add the value at memory address X (20) to register A
        JMP END ; Jump to the end of the program
        OUT 70 ; Output the value 70 (This instruction will be skipped due to the JMP above)
        HLT ; Halt the program (This instruction will be skipped due to the JMP above)
    END:
        ; This text section is defaulted to the MAIN entry point
        OUT 80 ; Output the value 80
        ADD RA [Y] ; Add the value at memory address Y (40) to register A
        OUT RA ; Output the value in register A
        HLT ; Halt the program

; 0 From stack, or none
; 1 From register
; 2 From immediate value
; 3 From memory address
; 4 Procedure address