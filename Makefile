CC      = gcc
CFLAGS  = -m32 -fno-stack-protector
LD      = ld
NASM    = nasm
ISO     = PRos.iso

OBJFILES = \
	kasm.o  \
	kc.o

kernel: $(OBJFILES)
	@echo "Compiling kernel..."
	$(LD) -m elf_i386 -T link.ld -o $@ $^ -z execstack

kasm.o: kernel.asm
	@echo "Compiling bootloader..."
	$(NASM) -f elf32 $< -o $@

kc.o: kernel.c
	@echo "Compiling C code..."
	$(CC) $(CFLAGS) -c $< -o $@

iso: kernel
	@echo "Creating ISO image..."
	mkdir -p iso/boot/grub
	cp kernel iso/boot/kernel.bin
	
	echo "set timeout_style=hidden" > iso/boot/grub/grub.cfg
	echo "set timeout=0" >> iso/boot/grub/grub.cfg
	echo "set default=0" >> iso/boot/grub/grub.cfg
		
	echo "menuentry 'PRoX operation system 1.2' {" >> iso/boot/grub/grub.cfg
	echo "    multiboot /boot/kernel.bin" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg

	
	grub-mkrescue -o $(ISO) iso

run: iso
	@echo "Running on qemu..."
	qemu-system-i386 -hda 1.img -cdrom $(ISO)

clean:
	@echo "Deleting .o files and ISO image..."
	rm -f $(OBJFILES) kernel
	rm -f $(ISO)
	rm -rf iso

