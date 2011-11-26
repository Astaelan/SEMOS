#include <Core/Console.h>
#include <Core/DeviceManager.h>

using namespace SEMOS;
using namespace SEMOS::Core;

DeviceManager::DeviceList DeviceManager::sDevices;
DeviceManager::COMPortLoggerList DeviceManager::sCOMPortLoggers;

void DeviceManager::Initialize()
{
    new(&sDevices) DeviceList;
    new(&sCOMPortLoggers) COMPortLoggerList;
}

bool DeviceManager::IsMemoryAvailable(uint32_t pAddress, uint32_t pLength)
{
    Device* device = nullptr;
    for (DeviceList::iterator it = sDevices.begin(); it != sDevices.end(); ++it)
    {
        device = *it;
        if (device->HasMemoryConflict(pAddress, pLength)) return false;
    }
    return true;
}

bool DeviceManager::IsIOPortAvailable(uint16_t pIOPort)
{
    Device* device = nullptr;
    for (DeviceList::iterator it = sDevices.begin(); it != sDevices.end(); ++it)
    {
        device = *it;
        if (device->HasIOPortConflict(pIOPort)) return false;
    }
    return true;
}

bool DeviceManager::Register(Device* pDevice)
{
    if (!pDevice->Initialize()) return false;
    sDevices.push_back(pDevice);
    return true;
}

bool DeviceManager::RegisterCOMPortLogger(COMPortLogger* pCOMPortLogger)
{
    if (!Register(pCOMPortLogger)) return false;
    sCOMPortLoggers.push_back(pCOMPortLogger);
    return true;
}

void DeviceManager::Unregister(Device* pDevice)
{
    pDevice->Cleanup();
    sDevices.remove(pDevice);
}

void DeviceManager::UnregisterCOMPortLogger(COMPortLogger* pCOMPortLogger)
{
    Unregister(pCOMPortLogger);
    sCOMPortLoggers.remove(pCOMPortLogger);
}

void DeviceManager::COMPortLoggersWriteByte(uint8_t pByte)
{
    for (COMPortLoggerList::iterator it = sCOMPortLoggers.begin(); it != sCOMPortLoggers.end(); ++it)
    {
        (*it)->WriteByte(pByte);
    }
}

void DeviceManager::COMPortLoggersWriteString(const char* pString)
{
    for (COMPortLoggerList::iterator it = sCOMPortLoggers.begin(); it != sCOMPortLoggers.end(); ++it)
    {
        (*it)->WriteString(pString);
    }
}

void DeviceManager::COMPortLoggersWriteLine(const char* pLine)
{
    for (COMPortLoggerList::iterator it = sCOMPortLoggers.begin(); it != sCOMPortLoggers.end(); ++it)
    {
        (*it)->WriteLine(pLine);
    }
}
