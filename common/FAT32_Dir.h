#ifndef FAT32_DIR_H
#define FAT32_DIR_H

#include "FAT32_BPB.h"
#include "FAT32_FAT.h"

#include <cstdint>
#include <functional>

#pragma pack(push, 1)

struct FAT32_DirEntry
{
    uint8_t name[11];        // 0x00
    uint8_t attr;            // 0x0B
    uint8_t ntRes;           // 0x0C
    uint8_t crtTimeTenth;    // 0x0D
    uint16_t crtTime;        // 0x0E
    uint16_t crtDate;        // 0x10
    uint16_t lstAccDate;     // 0x12
    uint16_t firstClusterHi; // 0x14
    uint16_t wrtTime;        // 0x16
    uint16_t wrtDate;        // 0x18
    uint16_t firstClusterLo; // 0x1A
    uint32_t fileSize;       // 0x1C
};

#pragma pack(pop)

class FAT32_Directory
{
private:
    FAT32_BPB &BPB;
    FAT32_FAT &FAT;

public:
    FAT32_Directory(FAT32_BPB& bpb, FAT32_FAT& fat);
    uint32_t getRootCluster() const;
    uint32_t getFirstCluster(const FAT32_DirEntry& entry);

    bool isEndOfDirectory(const FAT32_DirEntry &entry);
    bool isDeleted(const FAT32_DirEntry &entry);
    bool isLFN(const FAT32_DirEntry &entry);
    bool isDirectory(const FAT32_DirEntry &entry);

    void walkDirectory(uint32_t startCluster, std::function<void(const FAT32_DirEntry &)> callback);
};

#endif