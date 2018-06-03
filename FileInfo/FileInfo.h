#ifndef FILEINFO_H_
#define FILEINFO_H_

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

constexpr size_t kFileInfoSize = 32;
constexpr uint8_t
	kDirectoryIndicator = 0x10,
	kSystemIndicator = 0x04,
	kHiddenIndicator = 0x02,
	kArchiveIndicator = 0x20;

constexpr size_t
	kFileNameSize = 8,
	kFileExtensionSize = 3;

constexpr size_t
	kFileNameW = 12,
	kFileSizeW = 10,
	kFileCreationDateW = 13,
	kFileCreationTimeW = 13,
	kFileModificationDateW = 17,
	kFileModificationTimeW = 17,
	kFileFirstClusterNumW = 13;

#pragma pack(push, 1)
struct FileInfo {
	char
		name[kFileNameSize],
		extension[kFileExtensionSize];
	uint8_t
		attributes,
		reserved,
		creationFracSeconds;
	uint16_t
		creationTime,
		creationDate,
		lastAccessDate,
		firstClasterHighOrder,
		lastModificationTime,
		lastModificationDate,
		firstClasterLowOrder;
	uint32_t
		size;
};
#pragma pack(pop)

bool isDirectory(const FileInfo &fileInfo);
bool isSystem(const FileInfo &fileInfo);
bool isHidden(const FileInfo &fileInfo);
bool isArchive(const FileInfo &fileInfo);
std::string getFullFilename(const FileInfo &fileInfo);
bool isFake(const FileInfo &fileInfo);

bool isCurrentDir(FileInfo &fileInfo);
bool isParentDir(FileInfo &fileInfo);

void readFileInfo(FileInfo &fileInfo, std::ifstream &file);

std::ostream& operator<<(std::ostream &strm, const FileInfo &fileInfo);

std::string informationString();

#endif