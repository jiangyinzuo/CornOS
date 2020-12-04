.code16
.text
.globl _start
_start:
    
    movb $'H', %al
    movb $0x0e, %ah
    int  $0x10

    movb $'e', %al
    movb $0x0e, %ah
    int  $0x10

    movb $'l', %al
    movb $0x0e, %ah
    int  $0x10

    movb $'l', %al
    movb $0x0e, %ah
    int  $0x10
    
    movb $'o', %al
    movb $0x0e, %ah
    int  $0x10
    
    # append MBR magic number 0x55aa
    . = _start + 510
    .byte 0x55
    .byte 0xaa
