#include <iostream>
#include <Windows.h>
#include <string>
#include "diffDef.h"

void showMe()
{
    std::cout << "\n" << "          Insert smth to exit programm" << std::endl;
    std::string x;
    std::cin >> x;
}

void letMeTry(int nTry, int maxTry)
{
    if (maxTry - nTry > 0)
    {
        std::cout << " You have " << maxTry - nTry << " more try..." << std::endl;
        std::cout << "\n" << "   Enter the volume you are interested in :  ";
    }
    else
    {
        std::cout << "   You have no more try...stopping programm" << std::endl;
        showMe();
    }
}

int calcAndShowAlldrives(checkPhys dllCheckPhysDrives)
{
    int foundDriveCount = 0;
    bool checkResult = false;
    std::string fileName = "\\\\.\\PhysicalDrive";
    std::cout << "       ------------------------------------" << std::endl;
    for (int DriveNum = 0; DriveNum < 30; DriveNum++)
    {
        checkResult = false;
        dllCheckPhysDrives(checkResult, DriveNum, fileName);
        if (checkResult == true)
        {
            std::cout << "       " << fileName << DriveNum << "             exist " << std::endl;
            foundDriveCount++;
        }
    }
    std::cout << "\n" << "       Summary physical drives founded : " << foundDriveCount << std::endl;
    std::cout << "       ------------------------------------\n" << "\n" << std::endl;
    return foundDriveCount;
}

int calcAndShowAllVolumes(checkVol dllCheckVolume)
{
    BYTE buffer[512];
    std::string fileName = "\\\\.\\";
    const int alphCount = 26;
    int foundVolumes = 0;
    std::string letters[alphCount] = { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };
    for (int letterNum = 0; letterNum < 26; letterNum++)
    {
        bool checkResult = false;
        bool checkPointer = false;
        bool checkReading = false;
        std::string sysType = "";
        dllCheckVolume(checkResult, checkPointer, checkReading,  fileName, letters[letterNum], sysType);
        if (checkResult == true)
        {
            std::cout << "       " << fileName << letters[letterNum] << "           ";
            foundVolumes++;
            if (checkPointer == false)
            {
                std::cout << "       GOT PROBLEMS WITH READING ITS SYS TYPE (SetFilePointer PROBLEM) " << std::endl;
            }
            else
            {
                if (checkReading == false)
                {
                    std::cout << "       GOT PROBLEMS WITH READING ITS SYS TYPE (Reading info PROBLEM) " << std::endl;
                }
                else
                {
                    std::cout << sysType << std::endl;
                }
            }
        }
    };
    std::cout << "\n" << "       Summary volume drives founded : " << foundVolumes << std::endl;
    std::cout << "       ------------------------------------\n" << "\n" << std::endl;
    return foundVolumes;
}

int main()
{
    HMODULE hLib = LoadLibrary(L"assist.dll");
   if (hLib == NULL)
    {
        std::cout << "No able to attach dll !" << std::endl;
        showMe();
    }
    else
    {         
        std::cout << "       ###########   MAIN MENU  ###########" << std::endl;
        checkPhys dllCheckPhysDrives = (checkPhys)GetProcAddress(hLib, "CheckPhysDrives");
        if (!dllCheckPhysDrives)
        {
            std::cout << "Error while getting func address (Phys)" << std::endl; 
            showMe();
        }
        else
        {
            if (calcAndShowAlldrives(dllCheckPhysDrives) == 0)
            {
                std::cout << "\n" << "  No drives exist, stopping programm..." << std::endl;
                showMe();
            }
            else
            {
                checkVol dllCheckVolume = (checkVol)GetProcAddress(hLib, "CheckVolume");
                if (!dllCheckVolume)
                {
                    std::cout << "Error while getting func address (Volumes)"<< std::endl;
                    showMe();
                }
                else
                {   
                    if (calcAndShowAllVolumes(dllCheckVolume) == 0)
                    {
                        std::cout << "\n" << "  No Volumes exist, stopping programm..." << std::endl;
                        showMe();
                    }
                    else
                    {   
                        tellMeFStype dlltellMeFStype = (tellMeFStype)GetProcAddress(hLib, "tellMeFStype");
                        if (!dlltellMeFStype)
                        {
                            std::cout << "Error while getting func address (dlltellMeFStype)" << std::endl;
                            showMe();
                        }
                        else
                        {
                            std::cout << "\n" << "   Enter the volume you are interested in (ONLY NTFS able atm) :  ";
                            bool checkResult;
                            bool checkPointer;
                            bool checkReading;
                            int nTry = 0;
                            const int maxTry = 3;
                            std::string choosenVolume = "";
                            tellMeFSTryResult dlltellMeFSTryResult = (tellMeFSTryResult)GetProcAddress(hLib, "tellMeFSTryResult");
                            if (!dlltellMeFStype)
                            {
                                std::cout << "Error while getting func address (dlltellMeFSTryResult)" << std::endl;
                                showMe();
                            }
                            else
                            {
                                int tryResultInfo=-1;
                                while (nTry < maxTry)
                                {
                                    std::cin >> choosenVolume;

                                    std::string systemTypeName = dlltellMeFStype(checkResult, checkPointer, checkReading, choosenVolume);
                                    if (checkResult && checkPointer && checkReading)
                                    {
                                        tryResultInfo = dlltellMeFSTryResult(systemTypeName);
                                        if (tryResultInfo == 0)
                                        {
                                            nTry++;
                                            std::cout << "   This system is not supported yet. Try again! " << std::endl;
                                            letMeTry(nTry, maxTry);
                                        }
                                        else
                                        {
                                            nTry = 3;
                                            showMeAllInfo dllshowMeAllInfo = (showMeAllInfo)GetProcAddress(hLib, "showMeAllInfo");
                                            if (!dllshowMeAllInfo)
                                            {
                                                std::cout << "Error while getting func address (dllshowMeAllInfo)" << std::endl;
                                                showMe();
                                            }
                                            else
                                            {                                               
                                                dllshowMeAllInfo(choosenVolume);
                                                showMe();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        nTry++;
                                        if (checkResult == false)
                                        {
                                            std::cout << "   Error: this path not found (cant open file). Check your input." << std::endl;
                                        }
                                        else
                                        {
                                            if (checkPointer == false)
                                            {
                                                std::cout << "   File: opened +" << std::endl;
                                                std::cout << "   Error: problems with setting pointer" << std::endl;
                                            }
                                            else
                                            {
                                                if (checkReading == false)
                                                {
                                                    std::cout << "   File: opened +" << std::endl;
                                                    std::cout << "   Pointer: set +" << std::endl;
                                                    std::cout << "   Error: problems with reading file" << std::endl;
                                                }
                                                else
                                                {
                                                    std::cout << "   File: opened +" << std::endl;
                                                    std::cout << "   Pointer: set +" << std::endl;
                                                    std::cout << "   File: read +" << std::endl;
                                                    std::cout << "   Error: it's not NTFS (only NTFS supported atm)." << std::endl;
                                                }
                                            }
                                        }
                                        letMeTry(nTry, maxTry);
                                    }
                                }
                            }
                        }
                    }
                }
            }
    FreeLibrary(hLib); 
       }
    }
}

