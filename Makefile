OS_NAME							= Osmanthus
KERNEL_NAME					= osmanthus
ISO_DIR							= isodir
ISO_NAME						= osmanthus.iso

BUILD_PATH					= build
SCRIPTS_PATH				= scripts
BOOT_PATH						= boot
KERNEL_PATH					= kernel
DRIVER_PATH					= driver
INCLUDE							= include

ASM     = nasm
CC      = gcc
LD      = ld

CFLAGS	= -c -m32 -Wextra -Wall \
					-ggdb -gstabs+ \
					-nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
					-Xassembler --32 \
					-I${INCLUDE} \
					-I${KERNEL_PATH} \
					-I${DRIVER_PATH}

LDFLAGS = -T $(SCRIPTS_PATH)/link.ld -m elf_i386 -nostdlib
ASFLAGS = -f elf

C_SOURCES					= ${wildcard ${KERNEL_PATH}/*.c ${DRIVER_PATH}/*.c}
C_OBJ							= ${C_SOURCES:.c=.o}
ASM_SOURCES				= ${wildcard ${BOOT_PATH}/*.asm}
ASM_OBJ						= ${ASM_SOURCES:.asm=.o}
#C-OBJ						= ${BUILD_PATH}/${notdir ${C-SOURCES:.c=.o}}
#ASM_OBJ					= ${BUILD_PATH}/${notdir ${ASM_SOURCES:.asm=.o}}

.PHONY: all
all : ${KERNEL_NAME}
	@mkdir -p ${BUILD_PATH}/${ISO_DIR}/boot/grub
	@cp ${BUILD_PATH}/$^ ${BUILD_PATH}/${ISO_DIR}/boot/
	@sed 's,os-name,${OS_NAME},g; s,kernel-name,${KERNEL_NAME},g' \
		${SCRIPTS_PATH}/grub.cfg > ${BUILD_PATH}/${ISO_DIR}/boot/grub/grub.cfg
	@echo -e "\033[0;31mMakeing Kernel ISO \033[0m"
	@grub-mkrescue -o ${BUILD_PATH}/${ISO_NAME} ${BUILD_PATH}/${ISO_DIR} > /dev/null 2>&1

${KERNEL_NAME} : ${ASM_OBJ} ${C_OBJ}
	@echo -e "\033[0;34mLinking kernel \033[0m"
	@${LD} ${LDFLAGS} $^ -o ${BUILD_PATH}/$@

#%.o : %.cpp
#	@echo -e "\033[0;32mCompiling module :\t\033[0m" $<
#	@${CC} ${CFLAGS} $< -o $@

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
		${BUILD_PATH}/osmanthus.iso > /dev/null 2>&1 &

#.PHONY: debug
#debug:
#	@make --no-print-directory
#	@echo -e "\033[0;34mStarting QEMU\033[0m"
#	@qemu-system-i386 -s -S \
#		${BUILD_PATH}/osmanthus.iso > /dev/null 2>&1 &
#	@echo -e "\033[1;33mStarting GDB\033[0m"
#	@gdb -x ${SCRIPTS_PATH}/gdbinit
#	@echo -e "\033[1;31mFinished\033[0m"

.PHONY: debug
debug:
	@make --no-print-directory
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@qemu-system-i386 -s -S \
		${BUILD_PATH}/osmanthus.iso > /dev/null 2>&1 &
	@echo -e "\033[1;33mStarting GDB\033[0m"
	@gdb -x ${SCRIPTS_PATH}/gdbinit
	@echo -e "\033[1;31mFinished\033[0m"

.PHONY: clean
clean:
	@echo -e "\033[0;34mCleaning \033[0m"
	@rm -rf \
		build/* \
		*/*.o
	@echo -e "\033[0;31mFinished\033[0m"

.PHONY: upload
upload:
	@echo -e "\033[0;34mUploading to Github\033[0m"
	@proxychains4 -q git push git@github.com:iosmanthus/Osmanthus.git dev-iosmanthus
