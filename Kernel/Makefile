GCC = ..\Tools\bin\i686-elf-gcc
AS  = ..\Tools\bin\i686-elf-as
LD  = ..\Tools\bin\i686-elf-ld
RM  = ..\Tools\bin\rm
MKISOFS  = ..\Tools\bin\mkisofs
MKDIR = ..\Tools\bin\mkdir
OUT = ..\Build\boot\semos.bin
GCCARGS = -I..\Libs\Core\Include -I ..\Libs\Runtime\Include -I.\Include -std=c99 -Wall -Wextra -Werror -fno-builtin -nostdlib -fpack-struct=1 -nostartfiles -nodefaultlibs -o
LIB = ..\Libs\Runtime\Runtime.lib ..\Libs\Core\LibGcc.lib ..\Libs\Core\LibC.lib ..\Libs\Core\LibM.lib
OBJ = Object\Boot.o Object\GDT.o Object\IDT.o Object\IDTStubs.o Object\MBoot.o Object\Kernel.o Object\SystemCalls.o Object\FileSystem.o \
	  Object\Utility_COMPortLogger.o Object\Utility_List.o \
	  Object\Hardware_ATA_ATADevice.o Object\Hardware_ATA_ATAPIDevice.o \
	  Object\Hardware_GDT.o Object\Hardware_IDT.o Object\Hardware_PIC.o Object\Hardware_PIT.o Object\Hardware_RTC.o \
	  Object\Hardware_SystemPartition.o Object\Hardware_VGAText.o

build: $(OBJ)
  $(RM) -f $(OUT)
  $(MKDIR) -p ..\Build\boot\grub
  $(LD) -T Source\Kernel.ld -o $(OUT) Object\*.o $(LIB)
  copy /Y ..\Tools\grub\stage2_eltorito ..\Build\boot\grub
  copy /Y ..\Tools\grub\menu.lst ..\Build\boot\grub
  $(MKISOFS) -V SEMOS -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ..\SEMOS.iso ..\Build

clean:
  $(RM) -f $(OUT) Object\*.o

rebuild: clean build

Object\Boot.o: Source\Boot.asm
  $(AS) -o $@ $?

Object\IDT.o: Source\IDT.asm
  $(AS) -o $@ $?

Object\IDTStubs.o: Source\IDTStubs.asm
  $(AS) -o $@ $?

Object\GDT.o: Source\GDT.asm
  $(AS) -o $@ $?

Object\MBoot.o: Source\MBoot.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Kernel.o: Source\Kernel.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\SystemCalls.o: Source\SystemCalls.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\FileSystem.o: Source\FileSystem.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_GDT.o: Source\Hardware\GDT.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_IDT.o: Source\Hardware\IDT.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_PIC.o: Source\Hardware\PIC.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_PIT.o: Source\Hardware\PIT.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_RTC.o: Source\Hardware\RTC.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_VGAText.o: Source\Hardware\VGAText.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_SystemPartition.o: Source\Hardware\SystemPartition.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_ATA_ATADevice.o: Source\Hardware\ATA\ATADevice.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_ATA_ATAPIDevice.o: Source\Hardware\ATA\ATAPIDevice.c
  $(GCC) $(GCCARGS) $@ -c $?
  
Object\Utility_COMPortLogger.o: Source\Utility\COMPortLogger.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Utility_List.o: Source\Utility\List.c
  $(GCC) $(GCCARGS) $@ -c $?
