#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <types.h>
#include <sys/stat.h>

#define FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT     256
#define FILESYSTEM_FILEDESCRIPTOR_RESERVED      32

typedef struct {
    BOOL   Active;
    UINT16 Device;
    UINT16 BlockStart;
    UINT16 Mode;
    UINT32 TotalSize;
    UINT32 BlockSize;
    UINT32 BlockCount;
    BOOL   TerminalStream;
} FileDescriptor;

extern FileDescriptor gFileSystemFileDescriptors[];

void FileSystem_Initialize();

#endif