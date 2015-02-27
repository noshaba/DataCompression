#ifndef EXCEPTIONS_HPP
	#define EXCEPTIONS_HPP
	#include <string>
	class eFileException : public std::exception{
		public:
			eFileException(){}
			virtual const char* what() const throw(){
				return "Error with File.";
			}
	};
	class eFileNotFoundException : public eFileException{
		public:
			eFileNotFoundException(){}
			const char* what() const throw(){
				return "Error: File not found.";
			}
	}; // file not found exception
	class eInvalidFileFormatException : public eFileException{
		public:
			eInvalidFileFormatException(){}
			const char* what() const throw(){
				return "Error: Invalid file format.";
			}
	}; // wrong file format
	class eHeaderNotFoundException : public eFileException{
		public:
			eHeaderNotFoundException(){}
			const char* what() const throw(){
				return "Error: Header not found.";
			}
	}; // header was not found
	class eMustBeFileToBeCompressedException : public eFileException{
		public:
			eMustBeFileToBeCompressedException(){}
			const char* what() const throw(){
				return "Error: Must be an uncompressed file.";
			}
	}; // must be a file to be compressed
	class eMustBeFileToBeDecompressedException : public eFileException{
		public:
			eMustBeFileToBeDecompressedException(){}
			const char* what() const throw(){
				return "Error: Must be a compressed file.";
			}
	}; // must be a compressed file that shall be decompressed
	class eMagicNumberException : public eFileException{
		public:
			eMagicNumberException(){}
			const char* what() const throw(){
				return "Error: Magic number must be P6.";
			}
	};  // wrong magic number of ppm
#endif