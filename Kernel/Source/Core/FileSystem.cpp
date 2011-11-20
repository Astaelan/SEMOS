extern "C" {
#include <stdio.h>
#include <sys/stat.h>

int	fileno(FILE *);
}

#include <Core/FileSystem.h>
#include <Hardware/VGAText.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

std::map<std::string, FileSystem*> * FileSystem::sFileSystems = nullptr;
FileSystem::Descriptor * FileSystem::sDescriptors = nullptr;

uint32_t STDINRead(FileSystem::Descriptor * pDescriptor, void * pData, size_t pLength)
{
    // make a keyboard buffer, filled via interrupts, need keyboard maps from system files
    if (pDescriptor && pData && pLength) { }
    return 0;
}

uint32_t STDOUTWrite(FileSystem::Descriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor) { }
    VGAText::WriteString((const char *)pData, pLength);
    return (int32_t)pLength;
}

uint32_t STDERRWrite(FileSystem::Descriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor) { }
    VGAText::WriteString((const char *)pData, pLength);
    return (int32_t)pLength;
}

void FileSystem::Initialize()
{
    sFileSystems = new std::map<std::string, FileSystem*>();
    sDescriptors = new Descriptor[MaxDescriptors]();

    setbuf(stdin, NULL);
    int32_t stdinIndex = fileno(stdin);
    sDescriptors[stdinIndex].Active = true;
    sDescriptors[stdinIndex].Device = stdinIndex;
    sDescriptors[stdinIndex].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[stdinIndex].TerminalStream = true;
    sDescriptors[stdinIndex].Read = &STDINRead;

    setbuf(stdout, NULL);
    int32_t stdoutIndex = fileno(stdout);
    sDescriptors[stdoutIndex].Active = true;
    sDescriptors[stdoutIndex].Device = stdoutIndex;
    sDescriptors[stdoutIndex].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[stdoutIndex].TerminalStream = true;
    sDescriptors[stdoutIndex].Write = &STDOUTWrite;

    setbuf(stderr, NULL);
    int32_t stderrIndex = fileno(stderr);
    sDescriptors[stderrIndex].Active = true;
    sDescriptors[stderrIndex].Device = stderrIndex;
    sDescriptors[stderrIndex].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[stderrIndex].TerminalStream = true;
    sDescriptors[stderrIndex].Write = &STDERRWrite;
}

bool FileSystem::Register(const std::string & pRoot)
{
    if (sFileSystems->find(pRoot) != sFileSystems->end()) return false;
    (*sFileSystems)[pRoot] = this;
    return true;
}
