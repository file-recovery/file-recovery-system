#include "common/FAT32_BPB.h"
#include "common/FAT32_Dir.h"
#include "common/RecoveryHeader.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

const uint32_t PARENT_FLAG = 0xFFFFFFFF;
std::string formatFilename(const FAT32_DirEntry &entry)
{
    uint8_t cleanName[11];
    std::copy(std::begin(entry.name), std::end(entry.name), cleanName);
    if (cleanName[0] == 0xE5)
    {
        cleanName[0] = '_';
    }
    std::string base(reinterpret_cast<const char *>(cleanName), 8);
    base.erase(base.find_last_not_of(' ') + 1);
    std::string ext(reinterpret_cast<const char *>(cleanName + 8), 3);
    ext.erase(ext.find_last_not_of(' ') + 1);

    if (!ext.empty())
    {
        return base + "." + ext;
    }
    return base;
}

void listDirectory(FAT32_Directory &dir, uint32_t clusterNumber)
{
    std::cout << std::left
              << std::setw(25) << "[Name]"
              << std::setw(12) << "[Type]"
              << std::setw(15) << "[Size]"
              << "[Status]" << std::endl;           // add back status when determined recoverable or not
    std::cout << std::string(65, '-') << std::endl; // just terminal formatting

    dir.walkDirectory(clusterNumber, [&dir](const FAT32_DirEntry &entry)
                      {
        if (entry.name[0] == '.') return;
        bool isDel = dir.isDeleted(entry);
        bool isDir = dir.isDirectory(entry);

        if(isDel && isDir) return;
        if(!isDel && !isDir) return;

        std::string filename = formatFilename(entry);
        std::string typeStr = dir.isDirectory(entry)? "<DIR>":"<FILE>";

        std::cout << std::left 
                  << std::setw(25) << filename 
                  << std::setw(12) << typeStr 
                  << entry.fileSize << " B" << std::endl; }); // terminal formatting
}

uint32_t changeDirectory(FAT32_Directory &dir, uint32_t currentCluster, std::string &arg)
{
    if (arg == ".")
        return currentCluster;

    uint32_t targetCluster = 0;

    dir.walkDirectory(currentCluster, [&dir, &arg, &targetCluster](const FAT32_DirEntry &entry)
                      {
        if(dir.isDirectory(entry)){
            if(formatFilename(entry) == arg){
                targetCluster = dir.getFirstCluster(entry);    
            }
        } });
    if (arg == ".." && targetCluster == 0) // need to fix
    {
        return PARENT_FLAG; // constant flag
    }

    return targetCluster;
}

void directoryNav(FAT32_Directory &dir)
{
    uint32_t currentCluster = dir.getRootCluster();

    while (true)
    {
        std::cout << "\nFAT32 Shell: [Current Cluster]:" << currentCluster << std::endl;
        std::string command, arg;
        std::cin >> command;

        if (command == "ls")
        {
            listDirectory(dir, currentCluster);
        }

        else if (command == "cd")
        {
            std::cin >> arg;
            uint32_t nextCluster = changeDirectory(dir, currentCluster, arg);

            if (arg == ".." && (nextCluster == 0 || nextCluster == PARENT_FLAG))
            {
                currentCluster = dir.getRootCluster();
            }
            else if (nextCluster == 0 || nextCluster == PARENT_FLAG)
            {
                std::cerr << "ERROR: Directory not found\n";
            }
            else
            {
                currentCluster = nextCluster;
            }
        }
        else if (command == "exit")
        {
            std::cout << "Exiting...\n";
            break;
        }
        else
        {
            std::cerr << "ERROR: Unknown command\n";
        }
    }
}
