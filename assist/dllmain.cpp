#include "pch.h"
#include <iostream>
#include <string>
#include "dllClassesAndFactory.h"
#include "dllHeader.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
SystemType* fs;

std::string byte2ch(BYTE* data, int size)
{
    return std::string((char*)data, size);
}

extern "C" __declspec(dllexport) std::string tellMeFStype(bool* rez, bool* pointerRez, bool* readRez, std::string volumeLetter)
{
    *rez = false;
    *pointerRez = false;
    *readRez = false;
    std::string whatToReturn="Error";
    std::string fileName = "\\\\.\\";
    std::string fullPath = fileName + volumeLetter + ":";
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;
        ULONGLONG startOffset = 0;
        BYTE buffer[512];
        DWORD bytesToRead = 512;
        DWORD bytesRead;
        LARGE_INTEGER sectorOffset;
        sectorOffset.QuadPart = 0;
        all_FS_boot_for_name* pall_FS_boot_for_name = reinterpret_cast <all_FS_boot_for_name*> (buffer);
        unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, NULL, FILE_BEGIN);
        if (currentPosition == sectorOffset.LowPart)
        {
            *pointerRez = true;
            bool readResult = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
            if (readResult && bytesRead == bytesToRead)
            {
                *readRez = true;    
                whatToReturn = byte2ch(pall_FS_boot_for_name->OEM_Name, 8).c_str();
            };
        };
    }
    else
    {
        *rez = false;
    }

    CloseHandle(fileHandle);
    return whatToReturn;
}

extern "C" __declspec(dllexport) void CheckPhysDrives(bool* rez,int DriveNum,std::string fileName)
{

    std::string fullPath = fileName + std::to_string(DriveNum);
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;
    }
    else
    {
        *rez = false;      
    }

    CloseHandle(fileHandle);

}

extern "C" __declspec(dllexport) void CheckVolume(bool* rez,bool* pointerRez ,bool* readRez,std::string fileName,std::string letter,std::string* dllSysType)
{
    std::string fullPath = fileName + letter+":";
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;

        ULONGLONG startOffset = 0;
        BYTE buffer[512];
        DWORD bytesToRead = 512;
        DWORD bytesRead;
        LARGE_INTEGER sectorOffset;
        sectorOffset.QuadPart = 0;

        all_FS_boot_for_name* pall_FS_boot_for_name = reinterpret_cast <all_FS_boot_for_name*> (buffer);

        unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, &sectorOffset.HighPart, FILE_BEGIN);

        if (currentPosition == sectorOffset.LowPart)
        {
            *pointerRez = true;
            bool readResult = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
            if (readResult && bytesRead == bytesToRead)
            {
                *readRez = true;
                *dllSysType = byte2ch(pall_FS_boot_for_name->OEM_Name, 8).c_str();       
            };
        };
        
    }
    else
    {
        *rez = false;
    }

    CloseHandle(fileHandle);

}

extern "C" __declspec(dllexport) int tellMeFSTryResult( std::string systemTypeName)
{
    fs = desicionWhatToCreate(systemTypeName);
    if (fs == NULL)
    {
        return 0;
    }
    else
    {
        fs->info();
        return 1;
    };
}

extern "C" __declspec(dllexport) void showMeAllInfo(std::string volumeLetter)
{

    std::string fileName = "\\\\.\\";
    std::string fullPath = fileName + volumeLetter + ":";
    HANDLE fileHandle = CreateFileA(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ULONGLONG startOffset = 0;
    BYTE xBuffer[512];
    memset(xBuffer, 0, 512);
    DWORD bytesToRead = 512;
    DWORD bytesRead;
    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;
    unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, NULL, FILE_BEGIN);
    bool stopSpamTrash = ReadFile(fileHandle, &xBuffer, bytesToRead, &bytesRead, NULL);
    fs->showBoot(xBuffer);
}

