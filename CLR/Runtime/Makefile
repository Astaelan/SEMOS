GCC = ..\..\Tools\bin\i686-elf-gcc
AS  = ..\..\Tools\bin\i686-elf-as
AR  = ..\..\Tools\bin\i686-elf-ar
RM  = ..\..\Tools\bin\rm
MKDIR = ..\..\Tools\bin\mkdir
GCCARGS = -I..\..\Libs\Core\Include -I.\Include -std=c99 -Wall -Wextra -Werror -Wno-unused -Wno-pointer-sign -Wno-missing-field-initializers -Wno-sign-compare -fno-builtin -nostdlib -fpack-struct=1 -nostartfiles -nodefaultlibs -o
OBJ = Object\CLIFile.o Object\Delegate.o Object\DNA.o Object\Finalizer.o Object\Generics.o Object\Heap.o Object\InternalCall.o \
      Object\JIT.o Object\JIT_Execute.o Object\MetaData.o Object\MetaData_Fill.o Object\MetaData_Search.o Object\MethodState.o \
#	  Object\PInvoke.o \
	  Object\RVA.o Object\Sys.o Object\Thread.o Object\Type.o \
	  Object\System.Array.o \
	  Object\System.Char.o Object\System.Console.o \
	  Object\System.DateTime.o \
	  Object\System.Diagnostics.Debugger.o \
	  Object\System.Environment.o Object\System.Enum.o \
	  Object\System.GC.o \
	  Object\System.IO.FileInternal.o \
	  Object\System.Math.o \
#	  Object\System.Net.Dns.o \
#	  Object\System.Net.Sockets.Socket.o \
	  Object\System.Object.o \
	  Object\System.Runtime.CompilerServices.RuntimeHelpers.o \
	  Object\System.RuntimeType.o \
	  Object\System.String.o \
	  Object\System.Threading.Interlocked.o Object\System.Threading.Monitor.o Object\System.Threading.Thread.o \
	  Object\System.Type.o \
	  Object\System.ValueType.o \
	  Object\System.WeakReference.o

build: $(OBJ)
  $(MKDIR) -p ..\..\Libs\Runtime
  $(MKDIR) -p ..\..\Libs\Runtime\Include
  copy /Y Include ..\..\Libs\Runtime\Include
  $(AR) -rs ..\..\Libs\Runtime\Runtime.lib Object\*.o

clean:
  $(RM) -rf ..\..\Libs\Runtime Object\*.o

rebuild: clean build

#Object\Boot.o: Source\Boot.asm
#  $(AS) -o $@ $?

Object\CLIFile.o: Source\CLIFile.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Delegate.o: Source\Delegate.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\DNA.o: Source\DNA.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Finalizer.o: Source\Finalizer.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Generics.o: Source\Generics.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Heap.o: Source\Heap.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\InternalCall.o: Source\InternalCall.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\JIT.o: Source\JIT.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\JIT_Execute.o: Source\JIT_Execute.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\MetaData.o: Source\MetaData.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\MetaData_Fill.o: Source\MetaData_Fill.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\MetaData_Search.o: Source\MetaData_Search.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\MethodState.o: Source\MethodState.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\PInvoke.o: Source\PInvoke.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\RVA.o: Source\RVA.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Sys.o: Source\Sys.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Thread.o: Source\Thread.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\Type.o: Source\Type.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Array.o: Source\System.Array.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Char.o: Source\System.Char.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Console.o: Source\System.Console.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.DateTime.o: Source\System.DateTime.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Diagnostics.Debugger.o: Source\System.Diagnostics.Debugger.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Environment.o: Source\System.Environment.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Enum.o: Source\System.Enum.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.GC.o: Source\System.GC.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.IO.FileInternal.o: Source\System.IO.FileInternal.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Math.o: Source\System.Math.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Net.Dns.o: Source\System.Net.Dns.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Net.Sockets.Socket.o: Source\System.Net.Sockets.Socket.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Object.o: Source\System.Object.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Runtime.CompilerServices.RuntimeHelpers.o: Source\System.Runtime.CompilerServices.RuntimeHelpers.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.RuntimeType.o: Source\System.RuntimeType.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.String.o: Source\System.String.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Type.o: Source\System.Type.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Threading.Interlocked.o: Source\System.Threading.Interlocked.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Threading.Monitor.o: Source\System.Threading.Monitor.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.Threading.Thread.o: Source\System.Threading.Thread.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.ValueType.o: Source\System.ValueType.c
  $(GCC) $(GCCARGS) $@ -c $?

Object\System.WeakReference.o: Source\System.WeakReference.c
  $(GCC) $(GCCARGS) $@ -c $?

