#ifndef RECOVERY_H
#define RECOVERY_H

#include <cstdint>
#include <string>

// CONSTANTS
const uint8_t DELETED_ENTRY_MARKER = 0xE5;
const uint8_t LFN_TAG = 0x0F;
const uint32_t FAT32_END_OF_CHAIN = 0x0FFFFFF8;
const uint32_t FAT32_BAD_CLUSTER = 0x0FFFFFF7;
const uint32_t FAT32_FREE_CLUSTER = 0x00000000;
const uint32_t FAT32_CLUSTER_MASK = 0x0FFFFFFF; // extract the cluster number using this mask
#endif