extern "C" {
#include <signal.h>
#include <errno.h>
#include <types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>

#undef errno
extern int errno;

void * __dso_handle = nullptr;

int open(const char * pathname, int flags, mode_t mode);
}

#include <Core/DeviceManager.h>
#include <Core/MultiBoot.h>
#include <Core/FileSystem.h>
#include <Hardware/RTC.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

void Halt() { __asm("hlt"); }

void Panic(const char * pMessage)
{
    __asm("cli");
	DeviceManager::GetConsole().Clear(Console::CreateAttributes(Console::Color::DarkBlack, Console::Color::LightRed));
	DeviceManager::GetConsole().WriteLine(pMessage);
	while (TRUE) Halt();
}


void * sbrk(ptrdiff_t pAdjustment)
{
	static uint8_t memoryMapIndex;
	MultiBoot::MemoryBlock * memoryMap = MultiBoot::GetMemoryBlock(memoryMapIndex);

	if (pAdjustment >= 0)
	{
		uint8_t originalIndex = memoryMapIndex;
		while ((memoryMap->Length - memoryMap->Used) < pAdjustment &&
               memoryMapIndex < MultiBoot::GetMemoryBlockCount())
		{
			++memoryMapIndex;
			++memoryMap;
		}
		if (memoryMapIndex >= MultiBoot::GetMemoryBlockCount())
		{
			memoryMapIndex = originalIndex;
			Panic("PANIC: Insufficient memory");
		}
		memoryMap->Used += pAdjustment;
		return reinterpret_cast<void *>(memoryMap->Address + (memoryMap->Used - pAdjustment));
	}
	memoryMap->Used += pAdjustment;
	if (memoryMap->Used == 0 && memoryMapIndex > 0) --memoryMapIndex;

    return nullptr;
}

_PTR _malloc_r(_reent * pReentrant, size_t pSize)
{
    if (pReentrant) { }
    return malloc(pSize);
}

_PTR _calloc_r(_reent * pReentrant, size_t pCount, size_t pSize)
{
    if (pReentrant) { }
    return calloc(pCount, pSize);
}

void _free_r(_reent * pReentrant, _PTR pMemory)
{
    if (pReentrant) { }
    free(pMemory);
}

_PTR _realloc_r(_reent * pReentrant, _PTR pMemory, size_t pSize)
{
    if (pReentrant) { }
    return realloc(pMemory, pSize);
}

int gettimeofday(timeval * pTime,
                 void * pTimeZone)
{
	if (pTime)
	{
		pTime->tv_sec = RTC::GetSecondsSinceEpoch();
		pTime->tv_usec = 0;
	}
	if (pTimeZone)
	{
		timezone * tz = reinterpret_cast<timezone *>(pTimeZone);
		tz->tz_minuteswest = 0;
		tz->tz_dsttime = 0;
	}
	return 0;
}

int open(const char * pPath, int pFlags, mode_t pMode)
{
    char pathBuffer[MAXPATHLEN];
    if (pPath[0] != '/')
    {
        strcpy(pathBuffer, "/SYSTEM/");
        strcat(pathBuffer, pPath);
        pPath = pathBuffer;
    }
    uint16_t descriptorIndex = FileSystem::ReservedDescriptors;
    bool found = false;
    while (descriptorIndex < FileSystem::MaxDescriptors &&
           !(found = !FileSystem::GetDescriptor(descriptorIndex)->Active)) ++descriptorIndex;
    if (!found)
    {
        errno = ENFILE;
        return -1;
    }
    FileSystem::Descriptor * descriptor = FileSystem::GetDescriptor(descriptorIndex);

    size_t rootLength = 1;
    while (pPath[rootLength] != '/') ++rootLength;
    std::string pathRoot(pPath, rootLength);
    FileSystem * fileSystem = FileSystem::GetFileSystem(pathRoot);
    if (!fileSystem)
    {
        errno = EACCES;
        return -1;
    }
    return fileSystem->Open(descriptor, pPath, pFlags, pMode);
}

int close(int pDescriptorIndex)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active ||
        !descriptor->Close)
    {
        errno = EBADF;
        return -1;
    }
    return descriptor->Close(descriptor);
}


int fstat(int pDescriptorIndex,
          struct stat * pStats)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    pStats->st_dev = descriptor->Device;
    pStats->st_ino = descriptor->BlockStart;
    pStats->st_mode = descriptor->Mode;
    if (pStats->st_mode & S_IFREG) pStats->st_nlink = 1;
    else pStats->st_rdev = pStats->st_dev;
    pStats->st_size = descriptor->TotalSize;
    pStats->st_blksize = descriptor->BlockSize;
    pStats->st_blocks = descriptor->BlockCount;
    return 0;
}

int stat(const char * pPath,
         struct stat * pStats)
{
    FileSystem::Descriptor * descriptor = nullptr;
    int descriptorIndex = open(pPath, 0, 0);
    if (descriptorIndex < 0 ||
        descriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(descriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    pStats->st_dev = descriptor->Device;
    pStats->st_ino = descriptor->BlockStart;
    pStats->st_mode = descriptor->Mode;
    if (pStats->st_mode & S_IFREG) pStats->st_nlink = 1;
    else pStats->st_rdev = pStats->st_dev;
    pStats->st_size = descriptor->TotalSize;
    pStats->st_blksize = descriptor->BlockSize;
    pStats->st_blocks = descriptor->BlockCount;
    close(descriptorIndex);
    return 0;
}

int isatty(int pDescriptorIndex)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return 0;
    }
    if (!descriptor->TerminalStream)
    {
        errno = EINVAL;
        return 0;
    }
    return 1;
}

int write(int pDescriptorIndex,
          const void * pData,
          size_t pLength)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    if (!descriptor->Write)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->Write(descriptor, pData, pLength);
}

off_t lseek(int pDescriptorIndex,
            off_t pOffset,
            int pWhence)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    if (!descriptor->LSeek)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->LSeek(descriptor, pOffset, pWhence);
}

int read(int pDescriptorIndex,
         void * pData,
         size_t pLength)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (pDescriptorIndex < 0 ||
        pDescriptorIndex >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(pDescriptorIndex)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    if (!descriptor->Read)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->Read(descriptor, pData, pLength);
}

void abort()
{
    Panic("Aborted");
    while(true);
}

long sysconf(int pSetting)
{
    if (pSetting == _SC_PAGE_SIZE) return 1;
    errno = EINVAL;
    return -1;
}
