#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <string>
#include <exception>

class ReadException : public std::exception {
public:
	ReadException(std::string message);
	const char* what() noexcept;
private:
	std::string message_;
};

class DiskFileImageException : public std::exception {
public:
	DiskFileImageException(std::string fileName);
	const char* what() noexcept;
private:
	std::string fileName_{ "" };
};

class ArgumentsException : public std::exception {
public:
	const char * what() noexcept;
};

class CursorMoveException : public std::exception {
public:
	const char *what() noexcept;
};

class NextClusterException : public std::exception {
public:
	const char *what() noexcept;
};

#endif