hd60m.img: mbr.S
	mkdir bin
	as $< -o bin/mbr.o
	ld -Ttext 0x7c00 --oformat=binary bin/mbr.o -o bin/mbr.bin
	dd if=bin/mbr.bin of=bin/$@
	rm bin/mbr.o bin/mbr.bin
clean:
	rm -rf ./bin


qemu:
	qemu-system-x86_64 -hda bin/hd60m.img

bochs:
	bochs -f bochsrc
