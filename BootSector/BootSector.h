#ifndef BOOT_SECTOR_H_
#define BOOT_SECTOR_H_

#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>

constexpr size_t kBootSectorSize = 512;
constexpr uint16_t kCorrectBootSignature = 0xaa55;

#pragma pack(push, 1)
struct BootSector {
	char
		bootCode[3],
		EOMName[8];
	uint16_t bytesPerSector; //
	uint8_t sectorsPerCluster; //
	uint16_t reservedSectors; //
	uint8_t numberOfFATs; //
	uint16_t
		rootEntries,
		sectorsNum;
	uint8_t mediaType;
	uint16_t
		sectorPerFat, //
		sectorsPerTrack,
		numberOfHeads;
	uint32_t
		hiddenSectors,
		bigTotalSectors; //
	uint8_t
		physicalDiskNumber,
		currentHead,
		signature;
	uint32_t volumeSerialNumber;
	char volumeLabel[11];
	char systemID[8];
	char bootloaderCode[448];
	uint16_t bootSignature;
};
#pragma pack(pop)

void readBootSector(BootSector &bootSector, std::ifstream& file);

std::ostream& operator<<(std::ostream &strm, const BootSector &bootSector);

#endif