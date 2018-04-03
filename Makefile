OS_NAME=Osmanthus
KERNEL_NAME=osmanthus
ISO_DIR=isodir
ISO_NAME=${KERNEL_NAME}.iso

BUILD=build
SCRIPTS=scripts
BOOT=boot
INIT=init
KERNEL=kernel
DRIVER=driver
LIB=lib
INCLUDE=include

ASM=yasm
CC=gcc
LD=ld

REPO=git@github.com:iosmanthus/Osmanthus.git
BRANCH=iosmanthus


CFLAGS=-c -m32 -Wextra -Wall \
					-nostdinc -ffreestanding -fno-builtin -fno-stack-protector -fno-pie\
					-Xassembler --32 \
					-I${INCLUDE}
RFLAGS=-DNDEBUG -O2
DFLAGS=-g -ggdb -gstabs+

LDFLAGS = -T $(SCRIPTS)/link.ld -m elf_i386 -nostdlib
ASFLAGS = -f elf

C_SOURCES=${wildcard ${INIT}/*.c ${KERNEL}/*.c ${DRIVER}/*.c ${LIB}/*.c}

C_OBJ=$(addprefix ${BUILD}/,${C_SOURCES:.c=_c.o})

ASM_SOURCES=${wildcard ${BOOT}/*.asm ${INIT}/*.asm}

ASM_OBJ=$(addprefix ${BUILD}/,${ASM_SOURCES:.asm=_asm.o})

.PHONY: all
all : CFLAGS+=${DFLAGS}
all :  ${BUILD} ${KERNEL_NAME}
	@cp ${BUILD}/${KERNEL_NAME} ${BUILD}/${ISO_DIR}/boot/
	@sed 's,os-name,${OS_NAME},g; s,kernel-name,${KERNEL_NAME},g' \
		${SCRIPTS}/grub.cfg > ${BUILD}/${ISO_DIR}/boot/grub/grub.cfg
	@echo -e "\033[0;31mMakeing Kernel ISO \033[0m"
	@grub-mkrescue -o ${BUILD}/${ISO_NAME} ${BUILD}/${ISO_DIR} > /dev/null 2>&1

${BUILD}:
	@echo -e "\033[0;34mBuilding 'build' directory\033[0m"
	@mkdir -p $@/${ISO_DIR}/boot/grub \
		$@/${BOOT} \
		$@/${KERNEL} \
		$@/${DRIVER} \
		$@/${LIB} \
		$@/${INIT}


${KERNEL_NAME} : ${ASM_OBJ} ${C_OBJ}
	@echo -e "\033[0;34mLinking kernel \033[0m"
	@${LD} ${LDFLAGS} $^ -o ${BUILD}/$@

${BUILD}/%_asm.o : %.asm
	@echo -e "\033[0;32mAssembling module :\t\033[0m" $<
	@${ASM} ${ASFLAGS} $< -o $@

${BUILD}/%_c.o : %.c
	@echo -e "\033[0;32mCompiling module :\t\033[0m" $<
	@${CC} ${CFLAGS} $< -o $@

.PHONY: run
run: CFLAGS += ${RFLAGS}
run:
	@make CFLAGS="${CFLAGS}" --no-print-directory
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@qemu-system-i386 \
		${BUILD}/${ISO_NAME} > /dev/null 2>&1

.PHONY: release
release: CFLAGS += ${RFLAGS}
release:
	@echo -e "\033[0;34mMaking release version\033[0m"
	@make CFLAGS="${CFLAGS}" --no-print-directory

.PHONY: debug
debug: CFLAGS += ${DFLAGS}
debug:
	@echo -e "\033[0;34mMaking debug version\033[0m"
	@make CFLAGS="${CFLAGS}" --no-print-directory


.PHONY: gdb
gdb:
	@echo -e "\033[1;33mStarting GDB\033[0m"
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@gdb -x ${SCRIPTS}/gdbinit
	@echo -e "\033[1;31mFinished\033[0m"
	#@qemu-system-i386 -s -S \
		#${BUILD}/${ISO_NAME} > /dev/null 2>&1 &

gdbgui:
	@echo -e "\033[1;33mStarting GDB GUI\033[0m"
	@echo -e "\033[0;34mStarting QEMU\033[0m"
	@gdbgui -x ${SCRIPTS}/gdbinit
	@echo -e "\033[1;31mFinished\033[0m"
	#@qemu-system-i386 -s -S \
		#${BUILD}/${ISO_NAME} > /dev/null 2>&1 &

.PHONY: clean
clean:
	@echo -e "\033[0;34mCleaning \033[0m"
	@rm -rf \
		build/*/*.o
	@echo -e "\033[0;31mFinished\033[0m"

.PHONY: upload
upload:
	@echo -e "\033[0;34mUploading to Github\033[0m"
	@proxychains -q git push ${REPO} ${BRANCH}
