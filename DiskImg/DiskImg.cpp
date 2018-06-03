#include <stack>
#include <utility>

#include "DiskImg.h"
#include "../Exceptions/Exceptions.h"

DiskImg::DiskImg(std::ifstream & file):
	file_(file)
{
	readBootSector(bootSector_, file_);
}

void DiskImg::moveToCluster(uint16_t clusterNum) {
	std::streampos result = 0;

	result += 1LL * bootSector_.reservedSectors * bootSector_.bytesPerSector;
	result += 1LL * bootSector_.numberOfFATs * bootSector_.sectorPerFat * bootSector_.bytesPerSector;

	if (clusterNum != 0) {
		result += 1LL * bootSector_.rootEntries * kFileInfoSize;
		clusterNum -= 2;
		result += 1LL * clusterNum * bootSector_.sectorsPerCluster * bootSector_.bytesPerSector;
	}

	file_.seekg(result);
	if (file_.tellg() != result)
		throw CursorMoveException();
}

void DiskImg::moveToFat(uint16_t clusterNum) {
	std::streampos result = 0;

	result += 1LL * bootSector_.reservedSectors * bootSector_.bytesPerSector;

	result += (1LL * clusterNum) * 2;

	file_.seekg(result);
	if (file_.tellg() != result)
		throw CursorMoveException();
}

uint16_t DiskImg::nextCluster(uint16_t clusterNum) {
	moveToFat(clusterNum);

	uint16_t result;
	file_.read(reinterpret_cast<char*>(&result), 2);

	if(0xFFF8 <= result && result <= 0xFFFF)
		return clusterNum;
	if(0x0002 <= result && result <= 0xFFEF)
		return result;
	
	throw NextClusterException();
}

void DiskImg::traverse(std::ostream &strm) {
	std::streampos result = 0;

	result += 1LL * bootSector_.reservedSectors * bootSector_.bytesPerSector;
	result += 1LL * bootSector_.numberOfFATs * bootSector_.sectorPerFat * bootSector_.bytesPerSector;

	result += 1LL * bootSector_.rootEntries * kFileInfoSize;

	file_.seekg(result);
	if (file_.tellg() != result)
		throw CursorMoveException();

	result -= 1LL * bootSector_.rootEntries * kFileInfoSize;
	file_.seekg(result);

	std::stack<std::pair<FileInfo, size_t>> s;

	for(size_t i = 0; i < bootSector_.rootEntries; ++i){
		FileInfo fileInfo;
		readFileInfo(fileInfo, file_);
		if(isFake(fileInfo))
			break;
		s.push(std::make_pair(fileInfo, 0));
	}

	strm << informationString() << std::endl;

	while(!s.empty()){
		size_t depth = s.top().second;
		FileInfo parent = s.top().first;
		s.pop();

		for(size_t i = 0; i < kFileNameW * depth; ++i)
			strm << '-';
		if(depth > 0)
			strm << '>';
		
		strm << parent << std::endl;

		if(isDirectory(parent)){
			auto currentCluster = parent.firstClasterLowOrder;

			while(true){
				moveToCluster(currentCluster);

				size_t currentRead = 0;
				while(currentRead + kFileInfoSize < bootSector_.sectorsPerCluster * bootSector_.bytesPerSector){
					FileInfo child;
					readFileInfo(child, file_);
					currentRead += kFileInfoSize;
					if(isFake(child) || isCurrentDir(child) || isParentDir(child))
						continue;
					s.push(std::make_pair(child, depth + 1));
				}

				if(nextCluster(currentCluster) == currentCluster)
					break;
				currentCluster = nextCluster(currentCluster);
			}
		}
	}
}
