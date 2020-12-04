hd60m.img: mbr.S
	mkdir bin
	nasm -f bin $< -o bin/mbr.bin
	dd if=bin/mbr.bin of=bin/$@ bs=512 count=1 conv=notrunc
	rm bin/mbr.bin
clean:
	rm -rf ./bin

qemu:
	qemu-system-x86_64 -hda bin/hd60m.img

bochs:
	bochs -f bochsrc
