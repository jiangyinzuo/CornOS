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

mbr_disk.img:
	if [ ! -d "./bin" ]; then mkdir "bin";	fi
	dd if=/dev/zero of=./bin/mbr_disk.img count=10000
	nasm -I include/ mbr_disk.S -o ./bin/mbr_disk.bin
	dd if=./bin/mbr_disk.bin of=./bin/mbr_disk.img bs=512 count=1 conv=notrunc
	rm bin/mbr_disk.bin

	nasm -I include/ loader.S -o ./bin/loader.bin
	# 第二个扇区
	dd if=./bin/loader.bin of=./bin/mbr_disk.img bs=512 count=1 seek=2 conv=notrunc
	rm bin/loader.bin

mbr_proc.img:
	if [ ! -d "./bin" ]; then mkdir "bin";	fi
	dd if=/dev/zero of=./bin/mbr_proc.img count=10000
	nasm -I include/ ./proc_mode/mbr.S -o ./bin/mbr_disk.bin
	dd if=./bin/mbr_disk.bin of=./bin/mbr_proc.img bs=512 count=1 conv=notrunc
	rm bin/mbr_disk.bin

	nasm -I include/ ./proc_mode/loader.S -o ./bin/loader.bin
	# 第二个扇区
	dd if=./bin/loader.bin of=./bin/mbr_proc.img bs=512 count=4 seek=2 conv=notrunc
	rm bin/loader.bin

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
