#pragma once
#pragma pack(push,1)
#include <iostream>
#include <Windows.h>

struct all_FS_boot_for_name
{
    BYTE jumpCode[3];
    BYTE OEM_Name[8];
    WORD noMatter1;
    BYTE noMatter2;
    WORD noMatter3;
    BYTE noMatter4[3];
    WORD noMatter5;
    BYTE noMatter6;
    WORD noMatter7;
    WORD noMatter8;
    WORD noMatter9;
    DWORD noMatter10;
    DWORD noMatter11;
    DWORD noMatter12;
    ULONGLONG noMatter13;
    ULONGLONG noMatter14;
    ULONGLONG noMatter15;
    DWORD noMatter16;
    BYTE noMatter17;
    BYTE noMatter18[3];
    ULONGLONG noMatter19;
    DWORD noMatter20;
    BYTE noMatter21[426];
    WORD noMatter22;
};
