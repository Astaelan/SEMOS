GCC = ..\Tools\bin\i686-elf-g++
AS  = ..\Tools\bin\i686-elf-as
LD  = ..\Tools\bin\i686-elf-ld
RM  = ..\Tools\bin\rm
MKISOFS  = ..\Tools\bin\mkisofs
MKDIR = ..\Tools\bin\mkdir
OUT = ..\Build\boot\semos.bin
GCCARGS = -I..\Libs\Core\Include -I..\Libs\Core\Include\c++ -I.\Include -include cstdint -std=c++0x -Wall -Wextra -Werror -fno-builtin -nostdlib -fpack-struct=1 -nostartfiles -nodefaultlibs -o
LIB = ..\Libs\Core\LibSTDC++.lib ..\Libs\Core\LibC.lib ..\Libs\Core\LibM.lib ..\Libs\Core\LibGcc.lib
OBJ = Object\Kernel.o \
	  Object\Core_Boot.o Object\Core_GDTUpdate.o Object\Core_GDT.o Object\Core_IDTUpdate.o Object\Core_IDTStubs.o Object\Core_IDT.o \
	  Object\Core_MultiBoot.o Object\Core_DLMalloc.o Object\Core_SystemCalls.o Object\Core_FileSystem.o \
	  Object\Hardware_ATA_ATADevice.o Object\Hardware_ATA_ATAPIDevice.o \
	  Object\Hardware_PIC.o Object\Hardware_PIT.o Object\Hardware_RTC.o \
	  Object\Hardware_SystemPartition.o Object\Hardware_VGAText.o \
	  Object\Utility_COMPortLogger.o Object\Utility_List.o

build:
  $(MKDIR) -p Object
  $(MKDIR) -p ..\Build\boot\grub
  $(MAKE) kernel

kernel: $(OBJ)
  $(LD) -T Source\Kernel.ld -o $(OUT) Object\*.o $(LIB)
  copy /Y ..\Tools\grub\stage2_eltorito ..\Build\boot\grub
  copy /Y ..\Tools\grub\menu.lst ..\Build\boot\grub
  $(MKISOFS) -V SEMOS -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ..\SEMOS.iso ..\Build

clean:
  $(RM) -f $(OUT) Object\*.o

rebuild: clean build

Object\Kernel.o: Source\Kernel.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_Boot.o: Source\Core\Boot.asm
  $(AS) -o $@ $?

Object\Core_GDTUpdate.o: Source\Core\GDTUpdate.asm
  $(AS) -o $@ $?

Object\Core_GDT.o: Source\Core\GDT.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_IDTUpdate.o: Source\Core\IDTUpdate.asm
  $(AS) -o $@ $?

Object\Core_IDTStubs.o: Source\Core\IDTStubs.asm
  $(AS) -o $@ $?

Object\Core_IDT.o: Source\Core\IDT.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_MultiBoot.o: Source\Core\MultiBoot.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_DLMalloc.o: Source\Core\DLMalloc.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_SystemCalls.o: Source\Core\SystemCalls.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Core_FileSystem.o: Source\Core\FileSystem.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_PIC.o: Source\Hardware\PIC.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_PIT.o: Source\Hardware\PIT.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_RTC.o: Source\Hardware\RTC.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_VGAText.o: Source\Hardware\VGAText.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_SystemPartition.o: Source\Hardware\SystemPartition.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_ATA_ATADevice.o: Source\Hardware\ATA\ATADevice.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Hardware_ATA_ATAPIDevice.o: Source\Hardware\ATA\ATAPIDevice.c
  $(GCC) $(GCCARGS) $@ -c $?
  
Object\Utility_COMPortLogger.o: Source\Utility\COMPortLogger.cpp
  $(GCC) $(GCCARGS) $@ -c $?

Object\Utility_List.o: Source\Utility\List.c
  $(GCC) $(GCCARGS) $@ -c $?
