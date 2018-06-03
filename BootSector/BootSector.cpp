#include "BootSector.h"
#include "../Exceptions/Exceptions.h"

void readBootSector(BootSector &bootSector, std::ifstream& file) {
	file.read(reinterpret_cast<char*>(&bootSector), static_cast<std::streamsize>(kBootSectorSize));
	if (file.gcount() < static_cast<std::streamsize>(kBootSectorSize)) {
		std::stringstream errorMessage;
		errorMessage << "Cannot read BootSector, only " << file.gcount() << " bytes are available" << std::endl;
		throw ReadException(errorMessage.str());
	}
}

std::ostream & operator<<(std::ostream & strm, const BootSector & bootSector){
	std::ios_base::fmtflags f(strm.flags());

	strm << "Sector size: " << bootSector.bytesPerSector << " bytes" << std::endl;
	strm << "Number of sectors in cluster: " << +bootSector.sectorsPerCluster << std::endl;
	strm << "Number of copies of FAT: " << +bootSector.numberOfFATs << std::endl;
	strm << "Number of root files: " << bootSector.rootEntries << std::endl;
	strm << "Number of sectors for reserved area: " << bootSector.reservedSectors << std::endl;

	strm << "Checking correct bootSignature in the end: ";
	strm << std::boolalpha << (bootSector.bootSignature == kCorrectBootSignature);

	strm.flags(f);

	return strm;
}
