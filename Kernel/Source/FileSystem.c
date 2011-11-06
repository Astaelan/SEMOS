#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <FileSystem.h>
#include <Hardware/VGAText.h>

int	fileno(FILE *);
char * strdup(const char *);
int	strcasecmp(const char *, const char *);

INT32 FileSystem_STDINRead(FileDescriptor * pDescriptor, void * pData, size_t pLength);
INT32 FileSystem_STDOUTWrite(FileDescriptor * pDescriptor, const void * pData, size_t pLength);
INT32 FileSystem_STDERRWrite(FileDescriptor * pDescriptor, const void * pData, size_t pLength);

List gFileSystems;
FileDescriptor gFileSystemFileDescriptors[FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT];

void FileSystem_Initialize()
{
    for (UINT32 index = 0; index < FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT; ++index) gFileSystemFileDescriptors[index].Index = index;

    setbuf(stdin, NULL);
    INT32 stdinDescriptor = fileno(stdin);
    gFileSystemFileDescriptors[stdinDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stdinDescriptor].Device = stdinDescriptor;
    gFileSystemFileDescriptors[stdinDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stdinDescriptor].TerminalStream = TRUE;
    gFileSystemFileDescriptors[stdinDescriptor].ReadHandler = &FileSystem_STDINRead;

    setbuf(stdout, NULL);
    INT32 stdoutDescriptor = fileno(stdout);
    gFileSystemFileDescriptors[stdoutDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stdoutDescriptor].Device = stdoutDescriptor;
    gFileSystemFileDescriptors[stdoutDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stdoutDescriptor].TerminalStream = TRUE;
    gFileSystemFileDescriptors[stdoutDescriptor].WriteHandler = &FileSystem_STDOUTWrite;

    setbuf(stderr, NULL);
    INT32 stderrDescriptor = fileno(stderr);
    gFileSystemFileDescriptors[stderrDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stderrDescriptor].Device = stderrDescriptor;
    gFileSystemFileDescriptors[stderrDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stderrDescriptor].TerminalStream = TRUE;
    gFileSystemFileDescriptors[stderrDescriptor].WriteHandler = &FileSystem_STDERRWrite;
}

BOOL FileSystem_Register(const char * pRoot,
                         FileSystemOpenHandler pOpenHandler)
{
    for (Node * node = gFileSystems.Head; node; node = node->Next) if (!strcasecmp(pRoot, ((FileSystem *)node->Data)->Root)) return FALSE;
    FileSystem * fs = (FileSystem *)malloc(sizeof(FileSystem));
    fs->Root = strdup(pRoot);
    fs->OpenHandler = pOpenHandler;
    List_Add(&gFileSystems, fs);
    return TRUE;
}

INT32 FileSystem_STDINRead(FileDescriptor * pDescriptor, void * pData, size_t pLength)
{
    // make a keyboard buffer, filled via interrupts, need keyboard maps from system files
    if (pDescriptor && pData && pLength) { }
    return 0;
}

INT32 FileSystem_STDOUTWrite(FileDescriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor) { }
    VGAText_WriteString((PSTRING)pData, pLength);
    return (INT32)pLength;
}

INT32 FileSystem_STDERRWrite(FileDescriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor) { }
    VGAText_WriteString((PSTRING)pData, pLength);
    return (INT32)pLength;
}

