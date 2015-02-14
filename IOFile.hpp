#ifndef IOFILE_HPP
	#define IOFILE_HPP
	
	#include "Exceptions.hpp"

	#include <fstream>
	#include <iostream>
	#include <string>
	#include <vector>
	/**
		gets characters from a file and saves them into a vector
		@param: filename
	*/
	std::vector<unsigned char> getTextChars(std::string filename); 
	/**
		writes a binary file from a vector with unsigned chars
		@param: filename, vector of unsigned chars
	*/
	void writeFileFromVec(std::string filename, std::vector<unsigned char> chars);
	/**
		writes a file from a string (no longer in use, only for testing purposes)
		@param: filename, text
	*/
	void writeFileFromString(std::string filename, std::string text);
	
#endif
