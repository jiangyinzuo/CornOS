%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR

; 输出背景色绿色，前景色红色，并且跳动的字符串"1 MBR"
mov byte [gs:0x10],'2'
mov byte [gs:0x11],0xA4     ; A表示绿色背景闪烁，4表示前景色为红色

mov byte [gs:0x12],' '
mov byte [gs:0x13],0xA4

mov byte [gs:0x14],'L'
mov byte [gs:0x15],0xA4

mov byte [gs:0x16],'O'
mov byte [gs:0x17],0xA4

mov byte [gs:0x18],'A'
mov byte [gs:0x19],0xA4

mov byte [gs:0x1a],'D'
mov byte [gs:0x1b],0xA4

mov byte [gs:0x1c],'E'
mov byte [gs:0x1d],0xA4

mov byte [gs:0x1e],'R'
mov byte [gs:0x1f],0xA4

jmp $		       ; 通过死循环使程序悬停在此
