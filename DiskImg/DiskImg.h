#ifndef DISKIMG_H_
#define DISKIMG_H_

#include <iostream>
#include <fstream>
#include <cstdint>

#include "../BootSector/BootSector.h"
#include "../FileInfo/FileInfo.h"

class DiskImg {
public:
	DiskImg(std::ifstream &file);

	void moveToCluster(uint16_t clusterNum);
	uint16_t nextCluster(uint16_t clusterNum);

	void traverse(std::ostream &strm);

	inline const BootSector &getBootSector() { return bootSector_; }
private:
	void moveToFat(uint16_t sectorNum);

	BootSector bootSector_;
	std::ifstream &file_;

	uint16_t *fatTalbe_;
};

#endif