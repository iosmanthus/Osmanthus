OS_NAME=Osmanthus
KERNEL_NAME=osmanthus
ISO_DIR=isodir
ISO_NAME=${KERNEL_NAME}.iso

BUILD=build
SCRIPTS=scripts
BOOT=boot
KERNEL=kernel
DRIVER=driver
LIB=lib
INCLUDE=include

ASM=nasm
CC=gcc
LD=ld

CFLAGS=-c -m32 -Wextra -Wall \
					-ggdb -gstabs+ \
					-nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
					-Xassembler --32 \
					-I${INCLUDE} \
					-I${KERNEL} \
					-I${DRIVER}

LDFLAGS = -T $(SCRIPTS)/link.ld -m elf_i386 -nostdlib
ASFLAGS = -f elf

C_SOURCES=${wildcard ${KERNEL}/*.c ${DRIVER}/*.c ${LIB}/*.c}
C_OBJ=${C_SOURCES:.c=.o}
ASM_SOURCES=${wildcard ${BOOT}/*.asm}
ASM_OBJ=${ASM_SOURCES:.asm=.o}

.PHONY: all
all :  ${BUILD} ${KERNEL_NAME}
	@mkdir -p ${BUILD}/${ISO_DIR}/boot/grub
	@cp ${BUILD}/${KERNEL_NAME} ${BUILD}/${ISO_DIR}/boot/
	@sed 's,os-name,${OS_NAME},g; s,kernel-name,${KERNEL_NAME},g' \
		${SCRIPTS}/grub.cfg > ${BUILD}/${ISO_DIR}/boot/grub/grub.cfg
	@echo -e "\033[0;31mMakeing Kernel ISO \033[0m"
	@grub-mkrescue -o ${BUILD}/${ISO_NAME} ${BUILD}/${ISO_DIR} > /dev/null 2>&1

${BUILD}:
	@echo -e "\033[0;34mCreating 'build' directory\033[0m"
	@mkdir -p $@

${KERNEL_NAME} : ${ASM_OBJ} ${C_OBJ}
	@echo -e "\033[0;34mLinking kernel \033[0m"
	@${LD} ${LDFLAGS} $^ -o ${BUILD}/$@

%.o : %.c
	@echo -e "\033[0;32mCompiling module :\t\033[0m" $<
	@${CC} ${CFLAGS} $< -o $@

%.o : %.asm
	@echo -e "\033[0;32mAssembling module :\t\033[0m" $<
	@${ASM} ${ASFLAGS} $< -o $@

.PHONY: run
run:
	@make --no-print-directory
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@qemu-system-i386 \
		${BUILD}/${ISO_NAME} > /dev/null 2>&1

.PHONY: debug
debug:
	@make --no-print-directory
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@qemu-system-i386 -s -S \
		${BUILD}/${ISO_NAME} > /dev/null 2>&1 &
	@echo -e "\033[1;33mStarting GDB\033[0m"
	@gdb -x ${SCRIPTS}/gdbinit
	@echo -e "\033[1;31mFinished\033[0m"

.PHONY: clean
clean:
	@echo -e "\033[0;34mCleaning \033[0m"
	@rm -rf \
		build \
		*/*.o
	@echo -e "\033[0;31mFinished\033[0m"

.PHONY: upload
upload:
	@echo -e "\033[0;34mUploading to Github\033[0m"
	@proxychains4 -q git push git@github.com:iosmanthus/Osmanthus.git dev-iosmanthus
