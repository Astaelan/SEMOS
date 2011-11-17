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
}

#include <MBoot.h>
#include <FileSystem.h>
#include <Hardware/RTC.h>
#include <Hardware/VGAText.h>

#undef errno
extern int  errno;

int strncasecmp(const char *, const char *, size_t);

extern "C" {
int kill(int pid, int sig);
}

void Halt() { __asm("hlt"); }

void Panic(const char * pMessage)
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

    //printf("sbrk(%d)\n", (int)pAdjustment);
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
    char pathBuffer[256];
    if (pathname[0] != '/')
    {
        strcpy(pathBuffer, "/SYSTEM/");
        strcat(pathBuffer, pathname);
        pathname = pathBuffer;
    }
    BYTE fd = 0;
    for (UINT32 index = FILESYSTEM_FILEDESCRIPTOR_RESERVED; index < FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT; ++index)
    {
        if (!gFileSystemFileDescriptors[index].Active)
        {
            fd = index;
            break;
        }
    }
    if (fd == 0)
    {
        errno = ENFILE;
        return -1;
    }
    FileDescriptor * descriptor = &gFileSystemFileDescriptors[fd];

    size_t pathLength = strlen(pathname);
    for (Node * node = gFileSystems.Head; node; node = node->Next)
    {
        FileSystem * fs = (FileSystem *)node->Data;
        size_t rootLength = strlen(fs->Root);
        if (rootLength >= pathLength) continue;
        if (!strncasecmp(fs->Root, pathname, rootLength)) return fs->OpenHandler(descriptor, pathname, flags, mode);
    }
    errno = EACCES;
    return -1;
}

INT32 close(INT32 fd)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active ||
        !gFileSystemFileDescriptors[fd].CloseHandler)
    {
        errno = EBADF;
        return -1;
    }
    return gFileSystemFileDescriptors[fd].CloseHandler(&gFileSystemFileDescriptors[fd]);
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

INT32 stat(const char * path,
           struct stat * buf)
{
    INT32 fd = open(path, 0, 0);
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
    close(fd);
    return 0;
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

INT32 write(int fd,
            const void * buf,
            size_t count)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active)
    {
        errno = EBADF;
        return -1;
    }
    FileDescriptor * descriptor = &gFileSystemFileDescriptors[fd];
    if (!descriptor->WriteHandler)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->WriteHandler(descriptor, buf, count);
}

off_t lseek(INT32 fd,
            off_t offset,
            INT32 whence)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active)
    {
        errno = EBADF;
        return -1;
    }
    FileDescriptor * descriptor = &gFileSystemFileDescriptors[fd];
    if (!descriptor->LSeekHandler)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->LSeekHandler(descriptor, offset, whence);
}

int read(int fd,
         void * buf,
         size_t count)
{
    if (fd < 0 ||
        fd >= FILESYSTEM_MAX_FILEDESCRIPTOR_COUNT ||
        !gFileSystemFileDescriptors[fd].Active)
    {
        errno = EBADF;
        return -1;
    }
    FileDescriptor * descriptor = &gFileSystemFileDescriptors[fd];
    if (!descriptor->ReadHandler)
    {
        errno = EINVAL;
        return -1;
    }
    return descriptor->ReadHandler(descriptor, buf, count);
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
