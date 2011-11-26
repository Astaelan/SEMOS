extern "C" {
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
}

#include <Core/Console.h>
#include <Core/FileSystem.h>

using namespace SEMOS;
using namespace SEMOS::Core;

FileSystem::FileSystemMap FileSystem::sFileSystems;
FileSystem::DescriptorArray FileSystem::sDescriptors;

uint32_t STDINRead(FileSystem::Descriptor* pDescriptor, void* pData, size_t pLength)
{
    // make a keyboard buffer, filled via interrupts, need keyboard maps from system files
    if (pDescriptor && pData && pLength) { }
    return 0;
}

uint32_t STDOUTWrite(FileSystem::Descriptor* pDescriptor, const void* pData, size_t pLength)
{
    if (pDescriptor) { }
    Console::WriteString(reinterpret_cast<const char*>(pData), pLength);
    return pLength;
}

uint32_t STDERRWrite(FileSystem::Descriptor* pDescriptor, const void* pData, size_t pLength)
{
    if (pDescriptor) { }
    Console::WriteString(reinterpret_cast<const char*>(pData), pLength);
    return pLength;
}

void FileSystem::Initialize()
{
    new(&sFileSystems) FileSystemMap;
    sDescriptors.fill(Descriptor());

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

FileSystem* FileSystem::GetFileSystem(const std::string& pRoot)
{
    FileSystemMap::iterator it = sFileSystems.find(pRoot);
    if (it == sFileSystems.end()) return nullptr;
    return (*it).second;
}

FileSystem::Descriptor* FileSystem::GetDescriptor(uint16_t pIndex) { return &sDescriptors[pIndex]; }

bool FileSystem::Register(const std::string& pRoot)
{
    if (sFileSystems.find(pRoot) != sFileSystems.end()) return false;
    sFileSystems[pRoot] = this;
    return true;
}
