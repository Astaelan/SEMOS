#pragma once

#include <string>
#include <map>
#include <array>

namespace SEMOS
{
    namespace Core
    {
        class FileSystem
        {
        public:
            static const uint16_t MaxDescriptors = 256;
            static const uint16_t ReservedDescriptors = 32;

            struct Descriptor;

            typedef uint32_t (*OpenHandler)(Descriptor* pDescriptor, const std::string& pPath, int32_t pFlags, mode_t pMode);
            typedef uint32_t (*CloseHandler)(Descriptor* pDescriptor);
            typedef off_t (*LSeekHandler)(Descriptor* pDescriptor, off_t pOffset, int32_t pWhence);
            typedef uint32_t (*WriteHandler)(Descriptor* pDescriptor, const void* pData, size_t pLength);
            typedef uint32_t (*ReadHandler)(Descriptor* pDescriptor, void* pData, size_t pLength);

            struct Descriptor
            {
                uint8_t Index;
                bool Active;
                uint16_t Device;
                uint16_t BlockStart;
                uint16_t Mode;
                uint32_t TotalSize;
                uint32_t BlockSize;
                uint32_t BlockCount;
                bool TerminalStream;
                uint32_t Offset;
                std::string Path;
                CloseHandler Close;
                LSeekHandler LSeek;
                WriteHandler Write;
                ReadHandler Read;
            };

            static void Initialize();
            static FileSystem* GetFileSystem(const std::string& pRoot);
            static Descriptor* GetDescriptor(uint16_t pIndex);

            virtual int32_t Open(Descriptor* pDescriptor, const std::string& pPath, int32_t pFlags, mode_t pMode) = 0;

        private:
            typedef std::map<std::string, FileSystem*> FileSystemMap;
            typedef std::array<Descriptor, MaxDescriptors> DescriptorArray;

            static FileSystemMap sFileSystems;
            static DescriptorArray sDescriptors;

            bool Register(const std::string& pRoot);
        };
    }
}
