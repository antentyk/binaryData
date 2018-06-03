#include <cstdio>
#include <cstring>

#include "Exceptions.h"

ReadException::ReadException(std::string message) :
	message_(message)
{}

const char * ReadException::what() noexcept {
	return message_.c_str();
}

DiskFileImageException::DiskFileImageException(std::string fileName):
	fileName_(fileName)
{}

const char * DiskFileImageException::what() noexcept {
	return std::strcat("Cannot open ", fileName_.c_str());
}

const char * ArgumentsException::what() noexcept {
	return "Invalid number of arguments";
}

const char * CursorMoveException::what() noexcept {
	return "Cannot advance cursor so much";
}

const char * NextClusterException::what() noexcept {
	return "Cannot find suitable value for next cluster";
}