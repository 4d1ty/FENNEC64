; A smooth-moving pixel animation

.data
    X:      400        ; current position
    Y:      300
    Z:      0          ; previous X
    COLOR:  16777215   ; white
    BLACK:  0

.text
MAIN:
    JMP LOOP
LOOP:
    ; Store old X into Z
    LOAD [Z] [X]        ; Z = X
    ; Increment X by 10
    ADD [X] 10
    ; Draw new pixel
    DRAWPIX [X] [Y] [COLOR]
    ; Erase old pixel
    DRAWPIX [Z] [Y] [BLACK]
    ; Check boundary
    CMP [X] 800
    JZ RESETX
    JMP LOOP
RESETX:
    LOAD [X] 0
    JMP LOOP
