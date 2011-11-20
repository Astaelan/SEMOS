extern "C" {
#include <signal.h>
#include <errno.h>
#include <types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>
#include <sys/time.h>
#include <sys/stat.h>

#undef errno
extern int  errno;

void *__dso_handle = NULL;

int kill(int pid, int sig);
INT32 open(const char * pathname, int flags, mode_t mode);
}

#include <Core/MultiBoot.h>
#include <Core/FileSystem.h>
#include <Hardware/RTC.h>
#include <Hardware/VGAText.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

void Halt() { __asm("hlt"); }

void Panic(const char * pMessage)
{
    __asm("cli");
	VGAText::Clear(VGAText::CreateAttributes(VGAText::Color::DarkBlack, VGAText::Color::LightRed));
	VGAText::WriteLine(pMessage);
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
		return (caddr_t)(memoryMap->Address + (memoryMap->Used - pAdjustment));
	}
	memoryMap->Used += pAdjustment;
	if (memoryMap->Used == 0 && memoryMapIndex > 0) --memoryMapIndex;

    return NULL;
}

_PTR _malloc_r (struct _reent *r, size_t sz)
{
    if (r) { }
    return malloc (sz);
}

_PTR _calloc_r (struct _reent *r, size_t a, size_t b)
{
    if (r) { }
    return calloc (a, b);
}

void _free_r (struct _reent *r, _PTR x)
{
    if (r) { }
    free (x);
}

_PTR _realloc_r (struct _reent *r, _PTR x, size_t sz)
{
    if (r) { }
    return realloc (x, sz);
}


INT32 gettimeofday(struct timeval * tv,
                   PVOID tz)
{
	if (tv)
	{
		tv->tv_sec = RTC_GetSecondsSinceEpoch();
		tv->tv_usec = 0;
	}
	if (tz)
	{
		struct timezone * temptz = (struct timezone*)tz;
		temptz->tz_minuteswest = 0;
		temptz->tz_dsttime = 0;
	}
	return 0;
}

INT32 open(const char * pathname, int flags, mode_t mode)
{
    char pathBuffer[MAXPATHLEN];
    if (pathname[0] != '/')
    {
        strcpy(pathBuffer, "/SYSTEM/");
        strcat(pathBuffer, pathname);
        pathname = pathBuffer;
    }
    uint16_t fd = FileSystem::ReservedDescriptors;
    bool found = false;
    while (fd < FileSystem::MaxDescriptors &&
           !(found = !FileSystem::GetDescriptor(fd)->Active)) ++fd;
    if (!found)
    {
        errno = ENFILE;
        return -1;
    }
    FileSystem::Descriptor * descriptor = FileSystem::GetDescriptor(fd);

    size_t rootLength = 1;
    while (pathname[rootLength] != '/') ++rootLength;
    std::string pathRoot(pathname, rootLength);
    FileSystem * fileSystem = FileSystem::GetFileSystem(pathRoot);
    if (!fileSystem)
    {
        errno = EACCES;
        return -1;
    }
    return fileSystem->Open(descriptor, pathname, flags, mode);
}

INT32 close(INT32 fd)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
        !descriptor->Active ||
        !descriptor->Close)
    {
        errno = EBADF;
        return -1;
    }
    return descriptor->Close(descriptor);
}


INT32 fstat(INT32 fd,
            struct stat *buf)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    buf->st_dev = descriptor->Device;
    buf->st_ino = descriptor->BlockStart;
    buf->st_mode = descriptor->Mode;
    if (buf->st_mode & S_IFREG) buf->st_nlink = 1;
    else buf->st_rdev = buf->st_dev;
    buf->st_size = descriptor->TotalSize;
    buf->st_blksize = descriptor->BlockSize;
    buf->st_blocks = descriptor->BlockCount;
    return 0;
}

INT32 stat(const char * path,
           struct stat * buf)
{
    FileSystem::Descriptor * descriptor = nullptr;
    INT32 fd = open(path, 0, 0);
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
        !descriptor->Active)
    {
        errno = EBADF;
        return -1;
    }
    buf->st_dev = descriptor->Device;
    buf->st_ino = descriptor->BlockStart;
    buf->st_mode = descriptor->Mode;
    if (buf->st_mode & S_IFREG) buf->st_nlink = 1;
    else buf->st_rdev = buf->st_dev;
    buf->st_size = descriptor->TotalSize;
    buf->st_blksize = descriptor->BlockSize;
    buf->st_blocks = descriptor->BlockCount;
    close(fd);
    return 0;
}

INT32 isatty(INT32 fd)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
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

INT32 write(int fd,
            const void * buf,
            size_t count)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
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
    return descriptor->Write(descriptor, buf, count);
}

off_t lseek(INT32 fd,
            off_t offset,
            INT32 whence)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
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
    return descriptor->LSeek(descriptor, offset, whence);
}

int read(int fd,
         void * buf,
         size_t count)
{
    FileSystem::Descriptor * descriptor = nullptr;
    if (fd < 0 ||
        fd >= FileSystem::MaxDescriptors ||
        !(descriptor = FileSystem::GetDescriptor(fd)) ||
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
    return descriptor->Read(descriptor, buf, count);
}

char * getcwd(char * buf, size_t size)
{
    buf[0] = '/';
    buf[1] = 0x00;
    if (size) { }
    return buf;
}

void exit(int status)
{
    if (status) { }
    while (TRUE);
}

void _exit(int status)
{
    if (status) { }
    while (TRUE);
}

int	glob(const char * pattern, int flags, int (*errfunc)(const char * epath, int eerrno), glob_t * pglob)
{
    printf("GLOB!\n");
    if (pattern && flags && errfunc && pglob) { }
    return GLOB_NOSPACE;
}

void globfree(glob_t * pglob)
{
    printf("GLOBFREE!\n");
    if (pglob) { }
}

int getpid()
{
    return 1;
}

int kill(int pid,
         int sig)
{
  if (pid && sig) { }
  errno = EINVAL;
  return -1;
}

long sysconf(int name)
{
    if (name == _SC_PAGE_SIZE) return 1;
    errno = EINVAL;
    return -1;
}
