#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

extern "C" {
#include <types.h>
#include <sys/stat.h>
}
#include <Utility/List.h>

#define FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT     256
#define FILESYSTEM_FILEDESCRIPTOR_RESERVED      32

struct FileDescriptor;

typedef INT32 (*FileSystemOpenHandler)(struct FileDescriptor * pDescriptor, const char * pPath, INT32 pFlags, mode_t pMode);
typedef INT32 (*FileSystemCloseHandler)(struct FileDescriptor * pDescriptor);
typedef off_t (*FileSystemLSeekHandler)(struct FileDescriptor * pDescriptor, off_t pOffset, INT32 pWhence);
typedef INT32 (*FileSystemWriteHandler)(struct FileDescriptor * pDescriptor, const void * pData, size_t pLength);
typedef INT32 (*FileSystemReadHandler)(struct FileDescriptor * pDescriptor, void * pData, size_t pLength);

typedef struct FileSystem
{
    char * Root;
    FileSystemOpenHandler OpenHandler;
} FileSystem;

typedef struct FileDescriptor
{
    BYTE Index;
    BOOL Active;
    UINT16 Device;
    UINT16 BlockStart;
    UINT16 Mode;
    UINT32 TotalSize;
    UINT32 BlockSize;
    UINT32 BlockCount;
    BOOL TerminalStream;
    UINT32 Offset;
    char * Path;
    FileSystemCloseHandler CloseHandler;
    FileSystemLSeekHandler LSeekHandler;
    FileSystemWriteHandler WriteHandler;
    FileSystemReadHandler ReadHandler;
} FileDescriptor;

extern List gFileSystems;
extern FileDescriptor gFileSystemFileDescriptors[];

void FileSystem_Initialize();
BOOL FileSystem_Register(const char * pRoot, FileSystemOpenHandler pOpenHandler);

#endif