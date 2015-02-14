#ifndef MYSTRUCTS_HPP
	#define MYSTRUCTS_HPP
	#include <string>

	template <typename T> struct CharInf {
		T charName;						// character
		int charNum;					// number of one char in a text
		double charProb;				// probability of that char
		double charNat;					// Nat of that char
		std::string charBitset;			// set bit for char
		
		// to sort a CharInf structs
		bool operator < (const CharInf &struc) const {
			return (charNum > struc.charNum);
		}
	};

#endif