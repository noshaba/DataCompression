#ifndef ENTROPY_HPP
	#define ENTROPY_HPP

	#include <map>
	#include <math.h>

	#include "IOFile.hpp"
	#include "MyStructs.hpp"

	class Entropy {

		public:
			typedef std::map<unsigned char, CharInf<unsigned char> > CharInfMap; // defines map type
			typedef std::map<unsigned char, CharInf<unsigned char> >::iterator CharInfMapIt; // defines iterator type for map
			Entropy(std::string filename);	// constructor
			~Entropy();	// destructor
			CharInfMap getCharInfMapInf();	/** returns the map with the character information (such as entropy etc.)*/
			double log2(double n);	/** log2 was used since MS Visual Studio 2010 did not know it*/
			double getEntropy();	/** returns the entropy of the file*/
			int getAmountDifferentChars();/** returns the amount of different chars in a file*/
			std::vector<unsigned char> getMyChars();	/** returns the vector with all the chars from the read file*/

		private:
			std::vector<unsigned char> myChars;	// vector with all the chars from the read file
			CharInfMap charInfMapInf;	// map for the character information (key: char, value: CharInf struct with the information)
			double entropy;	// entropy of file
			int amountDifferentChars;	// amount of the different chars in the file
	};
#endif
