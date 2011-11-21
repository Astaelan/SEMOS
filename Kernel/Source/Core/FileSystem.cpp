extern "C" {
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
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
    VGAText::WriteString(reinterpret_cast<const char *>(pData), pLength);
    return pLength;
}

uint32_t STDERRWrite(FileSystem::Descriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor) { }
    VGAText::WriteString(reinterpret_cast<const char *>(pData), pLength);
    return pLength;
}

void FileSystem::Initialize()
{
    sFileSystems = new std::map<std::string, FileSystem*>();
    sDescriptors = new Descriptor[MaxDescriptors]();

    setbuf(stdin, nullptr);
    sDescriptors[STDIN_FILENO].Active = true;
    sDescriptors[STDIN_FILENO].Device = STDIN_FILENO;
    sDescriptors[STDIN_FILENO].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[STDIN_FILENO].TerminalStream = true;
    sDescriptors[STDIN_FILENO].Read = &STDINRead;

    setbuf(stdout, nullptr);
    sDescriptors[STDOUT_FILENO].Active = true;
    sDescriptors[STDOUT_FILENO].Device = STDOUT_FILENO;
    sDescriptors[STDOUT_FILENO].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[STDOUT_FILENO].TerminalStream = true;
    sDescriptors[STDOUT_FILENO].Write = &STDOUTWrite;

    setbuf(stderr, nullptr);
    sDescriptors[STDERR_FILENO].Active = true;
    sDescriptors[STDERR_FILENO].Device = STDERR_FILENO;
    sDescriptors[STDERR_FILENO].Mode = S_IFCHR | ((S_IREAD | S_IWRITE) >> 3) | ((S_IREAD | S_IWRITE) >> 6);
    sDescriptors[STDERR_FILENO].TerminalStream = true;
    sDescriptors[STDERR_FILENO].Write = &STDERRWrite;
}

bool FileSystem::Register(const std::string & pRoot)
{
    if (sFileSystems->find(pRoot) != sFileSystems->end()) return false;
    (*sFileSystems)[pRoot] = this;
    return true;
}
