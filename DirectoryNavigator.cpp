#include "common/FAT32_BPB.h"
#include "common/FAT32_Dir.h"
#include "common/RecoveryHeader.h"
#include <iostream>

void listDirectory(FAT32_Directory &dir, uint32_t clusterNumber)
{
}

uint32_t changeDirectory(FAT32_Directory &dir, uint32_t clusterNumber, std::string &arg)
{

    return clusterNumber;
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
            if (nextCluster != 0)
            {
                currentCluster = nextCluster;
            }
            else
            {
                std::cerr << "ERROR: Directory not found\n";
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
