#include <iostream>
#include <fstream>

#include <iomanip>
#include <string>

#include "Exceptions/Exceptions.h"
#include "BootSector/BootSector.h"
#include "DiskImg/DiskImg.h"

int main(int argc, char ** argv) {
	if (argc != 2)
		throw ArgumentsException();
	
	std::ifstream diskImgFile;
	diskImgFile.open(*(++argv), std::ios::binary);
	if (!diskImgFile.is_open())
		throw DiskFileImageException(std::string{ *argv });

	DiskImg diskImg(diskImgFile);

	std::cout << diskImg.getBootSector() << std::endl;
	std::cout << std::endl;

	diskImg.traverse(std::cout);

	return 0;
}