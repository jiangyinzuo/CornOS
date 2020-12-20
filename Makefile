include tools/functions.mk

GCC_FLAGS := -I. -Iinclude -fno-builtin -fno-PIC -Wall -ggdb \
                 -m32 -gstabs -nostdinc -fno-stack-protector

ifndef TEST
SRC = main kernel arch lib
KERNEL_SRC = $(shell find $(SRC) -type f -regex '.*\.[S|c]')  # list of source files
KERNEL_OBJS = $(filter %.o,$(KERNEL_SRC:.S=.o) $(KERNEL_SRC:.c=.o))     # list of object files
else
SRC = kernel arch lib
KERNEL_SRC = $(shell find $(SRC) -type f -regex '.*\.[S|c]') $(TEST)  # list of source files
KERNEL_OBJS = $(filter %.o,$(KERNEL_SRC:.S=.o) $(KERNEL_SRC:.c=.o))     # list of object files
endif

OBJ = bin/obj

all: GCC_FLAGS += -D NDEBUG
all: corn.img

debug: corn.img

gdb: corn.img
	qemu-system-i386 -S -s -hda ./bin/$<

.PHONY: test

test:
	make -C ./test/lib test

corn.img: boot_block kernel_objs
	@ld -m elf_i386 -nostdlib -T tools/kernel.ld -o bin/corn_kernel \
 		$(patsubst %, bin/obj/%, $(KERNEL_OBJS))
	@dd if=/dev/zero of=bin/$@ count=10000
	@dd if=bin/boot_block of=bin/$@ conv=notrunc
	@dd if=bin/corn_kernel of=bin/$@ seek=1 conv=notrunc

#--------------- compile all kernel .o files -------------------

kernel_objs: $(KERNEL_OBJS)     # target
	@echo $(KERNEL_OBJS)
%.o: %.S
	@mkdir -p $(shell dirname $(OBJ)/$@)
	@gcc $(GCC_FLAGS) -o $(OBJ)/$@ -c $<
%.o: %.c
	@mkdir -p $(shell dirname $(OBJ)/$@)
	@gcc $(GCC_FLAGS) -o $(OBJ)/$@ -c $<

#--------------------------- boot loader ------------------------------

boot_block: bootasm.o bootmain.o mbr_sign
	@ld -m elf_i386 -nostdlib -N -T tools/boot.ld -e start \
		bin/obj/boot/bootasm.o bin/obj/boot/bootmain.o -o bin/obj/bootblock.o
	@objcopy -S -O binary bin/obj/bootblock.o bin/obj/bootblock.out
	@bin/tools/mbr_sign bin/obj/bootblock.out bin/$@

bootasm.o: boot
	@if [ ! -d "./bin/obj/boot" ]; then mkdir -p "bin/obj/boot";	fi
	@gcc $(GCC_FLAGS) -Os \
	 	-c boot/bootasm.S -o bin/obj/boot/bootasm.o

bootmain.o: boot
	@if [ ! -d "./bin/obj/boot" ]; then mkdir -p "bin/obj/boot";	fi
	@gcc -Iboot/ $(GCC_FLAGS) -Os \
		-c boot/bootmain.c -o bin/obj/boot/bootmain.o

mbr_sign: tools/mbr_sign.c
	@if [ ! -d "./bin/tools" ]; then mkdir -p "bin/tools";	fi
	@gcc -O2 $< -o ./bin/tools/$@

#----------------------------------------------------------------------

clean:
	rm -rf ./bin
	make -C ./test/lib clean
