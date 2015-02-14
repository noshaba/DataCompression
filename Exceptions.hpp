#ifndef EXCEPTIONS_HPP
	#define EXCEPTIONS_HPP
	#include <string>
	class eFileException{};
	class eFileNotFoundException : public eFileException{}; // file not found exception
	class eInvalidFileFormatException : public eFileException{}; // wrong file format
	class eHeaderNotFoundException : public eFileException{}; // header was not found
	class eMustBeFileToBeCompressedException : public eFileException{}; // must be a file to be compressed
	class eMustBeFileToBeDecompressedException : public eFileException{}; // must be a compressed file that shall be decompressed
	class eMagicNumberException : public eFileException{};  // wrong magic number of ppm
#endif