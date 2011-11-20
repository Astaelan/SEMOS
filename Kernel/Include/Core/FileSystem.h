#pragma once

#include <string>
#include <map>

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

            typedef uint32_t (*OpenHandler)(Descriptor * pDescriptor, const std::string & pPath, int32_t pFlags, mode_t pMode);
            typedef uint32_t (*CloseHandler)(Descriptor * pDescriptor);
            typedef off_t (*LSeekHandler)(Descriptor * pDescriptor, off_t pOffset, int32_t pWhence);
            typedef uint32_t (*WriteHandler)(Descriptor * pDescriptor, const void * pData, size_t pLength);
            typedef uint32_t (*ReadHandler)(Descriptor * pDescriptor, void * pData, size_t pLength);

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

        private:
            static std::map<std::string, FileSystem*> * sFileSystems;
            static Descriptor * sDescriptors;

            bool Register(const std::string & pRoot);

        public:
            static void Initialize();
            static FileSystem * GetFileSystem(const std::string & pRoot)
            {
                std::map<std::string, FileSystem*>::iterator it = sFileSystems->find(pRoot);
                if (it == sFileSystems->end()) return nullptr;
                return (*it).second;
            }
            static Descriptor * GetDescriptor(uint16_t pIndex) { return &sDescriptors[pIndex]; }

            virtual int32_t Open(Descriptor * pDescriptor, const std::string & pPath, int32_t pFlags, mode_t pMode) = 0;
        };
    }
}
