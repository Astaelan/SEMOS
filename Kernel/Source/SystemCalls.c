#include <errno.h>
#include <types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <MBoot.h>
#include <FileSystem.h>
#include <Hardware/RTC.h>
#include <Hardware/VGAText.h>

#undef errno
extern int  errno;

void Halt() { __asm("hlt"); }

void Panic(const PSTRING pMessage)
{
    __asm("cli");
	VGAText_Clear(VGATEXT_ATTRIBUTES(VGATEXT_ATTRIBUTE_DARK_BLACK, VGATEXT_ATTRIBUTE_LIGHT_RED));
	VGAText_WriteLine(pMessage);
	while (TRUE) Halt();
}


PVOID sbrk(ptrdiff_t pAdjustment)
{
	static BYTE mmapActiveIndex;
	MBootMMapAvailable * mmapActive = gMBootMMapAvailable + mmapActiveIndex;
	if (pAdjustment >= 0)
	{
		BYTE originalIndex = mmapActiveIndex;
		while ((mmapActive->Length - mmapActive->Used) < (UINT32)pAdjustment && mmapActiveIndex < gMBootMMapAvailableCount)
		{
			++mmapActiveIndex;
			++mmapActive;
		}
		if (mmapActiveIndex >= gMBootMMapAvailableCount)
		{
			mmapActiveIndex = originalIndex;
			Panic("PANIC: Insufficient memory");
		}
		mmapActive->Used += pAdjustment;
		return (caddr_t)(mmapActive->Address + (mmapActive->Used - pAdjustment));
	}
	mmapActive->Used += pAdjustment;
	if (mmapActive->Used == 0 && mmapActiveIndex > 0) --mmapActiveIndex;
    return NULL;
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

INT32 fstat(INT32 fd,
            struct stat *buf)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active)
    {
        errno = EBADF;
        return -1;
    }
    buf->st_dev = gFileSystemFileDescriptors[fd].Device;
    buf->st_ino = gFileSystemFileDescriptors[fd].BlockStart;
    buf->st_mode = gFileSystemFileDescriptors[fd].Mode;
    if (buf->st_mode & S_IFREG) buf->st_nlink = 1;
    else buf->st_rdev = buf->st_dev;
    buf->st_size = gFileSystemFileDescriptors[fd].TotalSize;
    buf->st_blksize = gFileSystemFileDescriptors[fd].BlockSize;
    buf->st_blocks = gFileSystemFileDescriptors[fd].BlockCount;
    return 0;
}

INT32 write(int fd,
            const void * buf,
            size_t count)
{
    if (fd) { }
    VGAText_WriteString((PSTRING)buf, count);
    return (INT32)count;
}

INT32 close(INT32 fd)
{
    Panic("Stopping Close");
    if (fd) { }
    errno = EBADF;
    return -1;
}

INT32 isatty(INT32 fd)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active)
    {
        errno = EBADF;
        return 0;
    }
    if (!gFileSystemFileDescriptors[fd].TerminalStream)
    {
        errno = EINVAL;
        return 0;
    }
    return 1;
}

off_t lseek(INT32 fd,
            off_t offset,
            INT32 whence)
{
    Panic("Stopping LSeek");
    if (fd && offset && whence) { }
    return 0;
}

int read(int fd,
         void * buf,
         size_t count)
{
    Panic("Stopping Read");
    if (fd && buf && count) { }
    return 0;
}
