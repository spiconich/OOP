#pragma once
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <string>

class SystemType
{
public:
    virtual void info() = 0;
    virtual void showBoot(BYTE buffer[512]) = 0;
    virtual ~SystemType() {}
    
};

class NTFS : public SystemType
{
public:

    void info()
    {
        std::cout << "   Detected system type is NTFS !" << std::endl;

    };

    void showBoot(BYTE buffer[512])
    {
        typedef struct  Data_NTFS
        {
            BYTE jumpCode[3];//0x0000
            BYTE OEM_Name[8];//0x0003
            WORD bytesPerSector;//0x000B
            BYTE sectorPerCluster;//0x000D
            WORD reservedSectors;//0x000E
            BYTE shouldBeZero[3];
            WORD noUsed1;
            BYTE mediaDescriptor;//0x0015
            WORD shouldBeZero2;
            WORD sectorPerTrack;//0x0018
            WORD numOfHeaders;//0x001A
            DWORD hiddenSectors;
            DWORD noUsed2;
            DWORD noUsed3;
            ULONGLONG sectorsInTheVolume;//0x0028
            ULONGLONG lcnOfMFT;//0x0030
            ULONGLONG lcnOfSMFTMirr;//0x0038
            DWORD clusterPerMftRecord;//0x0040
            BYTE clusterPerIndexRecord;//0x0044
            BYTE noUsed4[3];
            ULONGLONG volumeID;//0x0048
            DWORD checksum;
            BYTE bigLoadingCode[426];
            WORD markerOfTheEnd;
        };

        Data_NTFS* ntfsStruc= reinterpret_cast<Data_NTFS*>(buffer);
        std::cout << "   --------------------------------------------------------" << std::endl;
        std::cout << "   File system type (0x03) : " << ntfsStruc->OEM_Name << std::endl;
        std::cout << "   Bytes per sector (0x0B) : " << ntfsStruc->bytesPerSector << std::endl;
        WORD toShowsectorPerCluster = ntfsStruc->sectorPerCluster;
        std::cout << "   Sectors per cluster (0x0D) : " << toShowsectorPerCluster << std::endl;
        WORD toShowmediaDescriptor = ntfsStruc->mediaDescriptor;
        std::cout << "   Media descriptor (0x15) : " << toShowmediaDescriptor << std::endl;
        std::cout << "   Sectors per track (0x18) : " << ntfsStruc->sectorPerTrack << std::endl;
        std::cout << "   Number of heads (0x1A) : " << ntfsStruc->numOfHeaders << std::endl;
        std::cout << "   Number of sectors in the volume (0x28) : " << ntfsStruc->sectorsInTheVolume << std::endl;
        std::cout << "   LCN of VCN 0 of the $MFT (0x30) : " << ntfsStruc->lcnOfMFT << std::endl;
        std::cout << "   LCN of VCN 0 of the $MFTMirr (0x38) : " << ntfsStruc->lcnOfSMFTMirr << std::endl;
        std::cout << "   Clusters per MFT Record (0x40) : " << ntfsStruc->clusterPerMftRecord << std::endl;
        WORD toShowclusterPerIndexRecord = ntfsStruc->clusterPerIndexRecord;
        std::cout << "   Clusters per Index Record  (0x44): " << toShowclusterPerIndexRecord << std::endl;
        std::cout << "   Volume serial number  (0x48): " << ntfsStruc->volumeID << std::endl;

    };

};

class Factory
{
public:
    virtual SystemType* createSystemType() = 0;
    virtual ~Factory() {}
};

class NTFSFactory : public Factory
{
public:
    SystemType* createSystemType()
    {
        return new NTFS;
    }
};

SystemType* desicionWhatToCreate(std::string OEM)
{
    std::string strNTFS = "NTFS";
    const char* cRezNTFS;
    const char* cStr = strNTFS.c_str();
    const char* cOEM = OEM.c_str();
    cRezNTFS = strstr(cOEM, cStr);
    if (cRezNTFS != NULL)
    {
        NTFSFactory* ntfs_factory = new NTFSFactory;
        return ntfs_factory->createSystemType();
    }
    else
    {
        return NULL;
    }

}