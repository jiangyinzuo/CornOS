hd60m.img: mbr.S
	if [!-d bin]; then mkdir "bin"; fi
	nasm -f bin $< -o bin/mbr.bin
	dd if=bin/mbr.bin of=bin/$@ bs=512 count=1 conv=notrunc
	rm bin/mbr.bin

mbr_vedio.img: mbr_video.S
	if [!-d bin]; then mkdir "bin"; fi
	nasm -f bin $< -o bin/mbr_vedio.bin
	dd if=bin/mbr_vedio.bin of=bin/$@ bs=512 count=1 conv=notrunc
	rm bin/mbr_vedio.bin

clean:
	rm -rf ./bin

IMG=hd60m.img
# make qemu IMG=mbr_vedio.img
qemu:
	qemu-system-x86_64 -hda bin/$(IMG)

BOCHSRC=bochsrc
#  make bochs BOCHSRC=bochsrc_vedio
bochs:
	bochs -f $(BOCHSRC)
