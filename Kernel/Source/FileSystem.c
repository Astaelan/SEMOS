#include <stdio.h>
#include <FileSystem.h>
#include <Hardware/VGAText.h>

int	fileno(FILE *);

FileDescriptor gFileSystemFileDescriptors[FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT];

void FileSystem_Initialize()
{
    setbuf(stdin, NULL);
    INT32 stdinDescriptor = fileno(stdin);
    gFileSystemFileDescriptors[stdinDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stdinDescriptor].Device = stdinDescriptor;
    gFileSystemFileDescriptors[stdinDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stdinDescriptor].TerminalStream = TRUE;

    setbuf(stdout, NULL);
    INT32 stdoutDescriptor = fileno(stdout);
    gFileSystemFileDescriptors[stdoutDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stdoutDescriptor].Device = stdoutDescriptor;
    gFileSystemFileDescriptors[stdoutDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stdoutDescriptor].TerminalStream = TRUE;

    setbuf(stderr, NULL);
    INT32 stderrDescriptor = fileno(stderr);
    gFileSystemFileDescriptors[stderrDescriptor].Active = TRUE;
    gFileSystemFileDescriptors[stderrDescriptor].Device = stderrDescriptor;
    gFileSystemFileDescriptors[stderrDescriptor].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    gFileSystemFileDescriptors[stderrDescriptor].TerminalStream = TRUE;
}
