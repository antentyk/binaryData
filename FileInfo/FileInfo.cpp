#include <sstream>
#include <iomanip>
#include <cstring>

#include "FileInfo.h"
#include "../Exceptions/Exceptions.h"

bool isDirectory(const FileInfo &fileInfo) { return (fileInfo.attributes & kDirectoryIndicator) > 0; }
bool isSystem(const FileInfo &fileInfo) { return (fileInfo.attributes & kSystemIndicator) > 0; }
bool isHidden(const FileInfo &fileInfo) { return (fileInfo.attributes & kHiddenIndicator) > 0; }
bool isArchive(const FileInfo &fileInfo) { return (fileInfo.attributes & kArchiveIndicator) > 0; }

bool isCurrentDir(FileInfo &fileInfo){
	return isDirectory(fileInfo) && (getFullFilename(fileInfo) == "."); 
}
bool isParentDir(FileInfo &fileInfo){
	return isDirectory(fileInfo) && (getFullFilename(fileInfo) == ".."); 
}

std::string getFullFilename(const FileInfo &fileInfo){
	std::string result = "";

	for(size_t i = 0; i < kFileNameSize; ++i){
		if(fileInfo.name[i] == ' ' || fileInfo.name[i] == 0)
			break;
		result.push_back(fileInfo.name[i]);
	}
	
	if(!isDirectory(fileInfo)){	
		result.push_back('.');

		for(size_t i = 0; i < kFileExtensionSize; ++i){
			if(fileInfo.extension[i] == ' ' || fileInfo.extension[i] == 0)
				break;
			result.push_back(fileInfo.extension[i]);
		}
	}
	
	return result;
}

bool isFake(const FileInfo &fileInfo){
	const char *p = reinterpret_cast<const char *>(&fileInfo);
	for(size_t i = 0; i < kFileInfoSize; ++i)
		if(*(p++) != 0)
			return false;
	return true;
}

void readFileInfo(FileInfo &fileInfo, std::ifstream &file) {
	file.read(reinterpret_cast<char*>(&fileInfo), kFileInfoSize);

	if (file.gcount() < static_cast<std::streamsize>(kFileInfoSize)) {
		std::stringstream errorMessage;
		errorMessage << "Cannot read FileInfo, only " << file.gcount() << " bytes are available" << std::endl;
		throw ReadException(errorMessage.str());
	}
}

std::ostream& operator << (std::ostream &strm, const FileInfo &fileInfo) {
	std::ios_base::fmtflags f(strm.flags());

	strm << std::setw(kFileNameW) << getFullFilename(fileInfo) << " ";

	strm << std::setw(kFileSizeW);
	if(isDirectory(fileInfo))
		strm << "/";
	else
		strm << fileInfo.size;
	strm << " ";

	strm << std::setw(kFileCreationDateW) << fileInfo.creationDate << " ";
	strm << std::setw(kFileCreationTimeW) << fileInfo.creationTime << " ";
	strm << std::setw(kFileModificationDateW) << fileInfo.lastModificationDate << " ";
	strm << std::setw(kFileModificationTimeW) << fileInfo.lastModificationTime << " ";
	strm << std::setw(kFileFirstClusterNumW) << fileInfo.firstClasterLowOrder << " ";
	
	if (isSystem(fileInfo))
		strm << "system ";
	if (isHidden(fileInfo))
		strm << "hidden ";
	if (isArchive(fileInfo))
		strm << "archive ";

	strm.flags(f);
	return strm;
}

std::string informationString() {
	std::stringstream result;

	result << std::setw(kFileNameW) << "Filename" << " ";
	result << std::setw(kFileSizeW) << "File size" << " ";
	result << std::setw(kFileCreationDateW) << "Creation date" << " ";
	result << std::setw(kFileCreationTimeW) << "Creation time" << " ";
	result << std::setw(kFileModificationDateW) << "Modification date" << " ";
	result << std::setw(kFileModificationTimeW) << "Modification time" << " ";
	result << std::setw(kFileFirstClusterNumW) << "First cluster" << " ";
	result << "Attributes";

	return result.str();
}