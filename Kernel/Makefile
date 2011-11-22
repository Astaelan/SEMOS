SDK = ..\SDK
BIN = $(SDK)\bin
GDC = $(BIN)\i686-elf-gdc
AS  = $(BIN)\i686-elf-as
LD  = $(BIN)\i686-elf-ld
RM  = $(BIN)\rm
MKISOFS  = $(BIN)\mkisofs
MKDIR = $(BIN)\mkdir
OUT = ..\Build\boot\semos.bin
LDFLAGS = -L$(SDK)\i686-elf\lib -L$(SDK)\lib\gcc\i686-elf\4.6.2 -T Source\Kernel.ld
LIBS = -lgdruntime -lgphobos2 -lgcc -lc -lm
OBJ = Object\Boot.o Object\Kernel.o

build:
  $(MKDIR) -p Object
  $(MKDIR) -p ..\Build\boot\grub
  $(MAKE) kernel

kernel: $(OBJ)
  $(LD) $(LDFLAGS) -o $(OUT) $(OBJ) $(LIBS)
  copy /Y $(SDK)\grub\stage2_eltorito ..\Build\boot\grub
  copy /Y $(SDK)\grub\menu.lst ..\Build\boot\grub
  $(MKISOFS) -V SEMOS -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ..\SEMOS.iso ..\Build

clean:
  $(RM) -f $(OUT) Object\*.o

rebuild: clean build

Object\Boot.o: Source\Boot.asm
  $(AS) -o $@ $?

Object\Kernel.o: Source\Kernel.d
  $(GDC) -o $@ -c $?
