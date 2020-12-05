## 打开A20地址线的方法

1. 调用BIOS中断就可以实现A20 Gate的控制功能。 这个BIOS中断为 INT 15h, AX=2401h。被称为Fast A20。
```asm
movw $0x2401, %ax

int $0x15
```

2. 《操作系统真象还原》
```asm
in al, 0x92
or al, 0000_0010B
out 0x92, al
```

3. Ucore
```asm
    # Enable A20:
    #  For backwards compatibility with the earliest PCs, physical
    #  address line 20 is tied low, so that addresses higher than
    #  1MB wrap around to zero by default. This code undoes this.
seta20.1:
    inb $0x64, %al                                  # Wait for not busy(8042 input buffer empty).
    testb $0x2, %al
    jnz seta20.1

    movb $0xd1, %al                                 # 0xd1 -> port 0x64
    outb %al, $0x64                                 # 0xd1 means: write data to 8042's P2 port

seta20.2:
    inb $0x64, %al                                  # Wait for not busy(8042 input buffer empty).
    testb $0x2, %al
    jnz seta20.2

    movb $0xdf, %al                                 # 0xdf -> port 0x60
    outb %al, $0x60                                 # 0xdf = 11011111, means set P2's A20 bit(the 1 bit) to 1
```