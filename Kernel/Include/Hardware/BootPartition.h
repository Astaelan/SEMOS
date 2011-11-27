#pragma once

#include <Core/FileSystem.h>

namespace SEMOS
{
    namespace Hardware
    {
        class BootPartition : public SEMOS::Core::FileSystem
        {
        public:
            static void Initialize();
            int32_t Open(Descriptor* pDescriptor, const std::string& pPath, int32_t pFlags, mode_t pMode);

        private:
            struct Cache;
            typedef std::map<std::string, Cache*> CacheMap;

            struct Cache
            {
                uint8_t EntrySize;
                uint32_t LBA;
                uint32_t Length;
                uint8_t Flags;
                const char* Identifier;
                CacheMap Entries;
            };
        };
    }
}
