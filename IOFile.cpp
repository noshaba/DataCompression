#include "IOFile.hpp"
#include <cstdlib>

std::vector<unsigned char> getTextChars(std::string filename){
	std::fstream myfile(filename.c_str(), std::ios::in | std::ios::binary);
	
	std::vector<unsigned char> textChars;
	if(myfile.is_open() && myfile.good()){
		if(filename.substr(filename.find_last_of(".") + 1) == "txt" || filename.substr(filename.find_last_of(".") + 1) == "huffman"){
			unsigned int length;
			myfile.seekg (0, myfile.end);
			length = myfile.tellg();			// needed since the stream method "eof()" reads the control characters at the end of an ASCII coded file, too, and converts them to chars
			myfile.seekg (0, myfile.beg);
			while (textChars.size() < length){
				textChars.push_back(myfile.get());		// get characters from file
			}
		} else {
			while(!myfile.eof()){						// the control characters are needed in binary files
				textChars.push_back(myfile.get());
			}
		}
		myfile.close();
	} else {
		throw;
	}
	
	return textChars;
};

void writeFileFromVec(std::string filename, std::vector<unsigned char> chars){
	std::fstream outputFile(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc); // trunc = creates new file if file doesn't exist
	if(outputFile.is_open()){
		for(int i = 0; i < chars.size(); i++){
			outputFile.put(chars[i]);
		}
		outputFile.close();
	}
};

// only for testing, no longer in use

void writeFileFromString(std::string filename, std::string text){
	std::fstream outputFile(filename.c_str(), std::ios::out | std::ios::trunc);
	if(outputFile.is_open()){
		outputFile << text;
		outputFile.close();
	}
};
