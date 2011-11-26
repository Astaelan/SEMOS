#pragma once

#include <list>

#include <Core/Device.h>
#include <Core/COMPortLogger.h>

namespace SEMOS
{
    namespace Core
    {
        class DeviceManager
        {
        public:
            static void Initialize();

            static bool IsMemoryAvailable(uint32_t pAddress, uint32_t pLength);
            static bool IsIOPortAvailable(uint16_t pIOPort);

            static bool Register(Device* pDevice);
            static bool RegisterCOMPortLogger(COMPortLogger* pCOMPortLogger);
            static void Unregister(Device* pDevice);
            static void UnregisterCOMPortLogger(COMPortLogger* pCOMPortLogger);

            static void COMPortLoggersWriteByte(uint8_t pByte);
            static void COMPortLoggersWriteString(const char* pString);
            static void COMPortLoggersWriteLine(const char* pLine);

        private:
            typedef std::list<Device*> DeviceList;
            typedef std::list<COMPortLogger*> COMPortLoggerList;

            static DeviceList sDevices;
            static COMPortLoggerList sCOMPortLoggers;

            DeviceManager() { }
        };
    }
}
