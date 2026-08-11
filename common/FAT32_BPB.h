#ifndef FAT32_BPB_H
#define FAT32_BPB_H

#include <string>
#include <cstdint>

class FAT32_BPB
{
public:
    uint16_t bytesPerSec;
    uint8_t secPerCluster;
    uint16_t reservedSecCount;
    uint8_t fatCount;
    uint32_t fatSize;
    uint32_t rootClusterNumber;

    explicit FAT32_BPB(const std::string &imagePath);
    void printDetails() const;
};

#endif
