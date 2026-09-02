#include "common/FAT32_BPB.h"

#pragma pack(push, 1)
struct FAT32_Sector
{
    uint8_t jmpBoot[3];         // 0x00
    uint8_t OEMName[8];         // 0x03
    uint16_t bytesPerSec;       // 0x0B
    uint8_t secPerCluster;      // 0x0D
    uint16_t reservedSecCount;  // 0x0E
    uint8_t fatCount;           // 0x10
    uint16_t rootEntCnt;        // 0x11
    uint16_t totSec16;          // 0x13
    uint8_t media;              // 0x15
    uint16_t fatSize16;         // 0x16
    uint16_t secPerTrk;         // 0x18
    uint16_t numHeads;          // 0x1A
    uint32_t hiddSec;           // 0x1C
    uint32_t totSec32;          // 0x20
    uint32_t fatSize32;         // 0x24
    uint16_t extFlags;          // 0x28
    uint16_t fsVer;             // 0x2A
    uint32_t rootClusterNumber; // 0x2C
    uint16_t fsInfoSector;      // 0x30 //addresses wrong probably check again below this
    uint16_t backupBootSector;  // 0x32 backup, important if sector 0 corrupted
    uint8_t reserved[12];       // 0x34
    uint8_t driveNumber;        // 0x40
    uint8_t reserved1;          // 0x41
    uint8_t bootSignature;      // 0x42
    uint32_t volumeID;          // 0x43
    uint8_t volumeLabel[11];    // 0x47
    uint8_t fileSysType[8];     // 0x52 (validity check)
    uint8_t bootCode[420];      // 0x5A padding mainly
    uint16_t bootSectorSig;     // 0x1FE
};
#pragma pack(pop)

FAT32_BPB::FAT32_BPB(const std::string &imagePath)
{
    file.open(imagePath, std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open disk image");
    }

    FAT32_Sector sector{};
    file.read(reinterpret_cast<char *>(&sector), sizeof(FAT32_Sector));
    if (!file)
    {
        throw std::runtime_error("Failed to read sector");
    }

    bytesPerSec = sector.bytesPerSec;
    secPerCluster = sector.secPerCluster;
    reservedSecCount = sector.reservedSecCount;
    fatCount = sector.fatCount;
    fatSize = sector.fatSize32;
    rootClusterNumber = sector.rootClusterNumber;

    firstDataSector = reservedSecCount + fatCount * fatSize;
    fatStartOffset = (uint64_t)(reservedSecCount)*bytesPerSec;
    dataStartOffset = (uint64_t)firstDataSector * bytesPerSec;
    clusterSize = (uint32_t)secPerCluster * bytesPerSec;
}

uint64_t FAT32_BPB::clusterToOffset(uint32_t clusterNum) const
{
    return dataStartOffset + (uint64_t)clusterSize * (clusterNum - 2); // first data cluster = 2;
}

void FAT32_BPB::printDetails() const
{
    std::cout << "Bytes Per Sector:    " << bytesPerSec << "\n"
              << "Sectors Per Cluster: " << (int)secPerCluster << "\n"
              << "Reserved Sectors:    " << reservedSecCount << "\n"
              << "Number of FATs:      " << (int)fatCount << "\n"
              << "FAT Size (Sectors):  " << fatSize << "\n"
              << "Root Cluster Num:    " << rootClusterNumber << "\n"
              << "Cluster Size:        " << clusterSize << "\n"
              << "Fat Start Offset:    " << fatStartOffset << "\n"
              << "First Data Sector:   " << firstDataSector << "\n"
              << "Data Start Offset:   " << dataStartOffset << "\n";
}
