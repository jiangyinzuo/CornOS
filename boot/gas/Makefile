hd60m.img: mbr.S
	mkdir bin
	as $< -o bin/mbr.o
	ld -Ttext 0x7c00 --oformat=binary bin/mbr.o -o bin/mbr.bin
	dd if=bin/mbr.bin of=bin/$@
	rm bin/mbr.o bin/mbr.bin
clean:
	rm -rf ./bin