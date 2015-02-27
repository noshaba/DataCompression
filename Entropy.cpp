#include "Entropy.hpp"
#include <cstdlib>

Entropy::Entropy(std::string filename) {
	try{
		this->myChars = getTextChars(filename);
		entropy = 0;
		for(unsigned int i = 0; i < myChars.size(); i++){
			charInfMapInf[myChars[i]].charNum++;			// get amount of each character from text
		}
		for(CharInfMapIt iterator = charInfMapInf.begin(); iterator != charInfMapInf.end(); iterator++){
			iterator->second.charName = iterator->first;
			iterator->second.charProb = (double) iterator->second.charNum/(myChars.size()); // get probability of each char
			iterator->second.charNat = log2(1/(iterator->second.charProb));	// get Nat of each char
			entropy = entropy + (iterator->second.charNat)*(iterator->second.charProb);
		}
		amountDifferentChars = charInfMapInf.size();
	} catch(...){
		throw;
	}
};

Entropy::~Entropy(){};

Entropy::CharInfMap Entropy::getCharInfMapInf(){
	return this->charInfMapInf;
};

double Entropy::log2(double n){
	return log(n)/log(2.0);
};

double Entropy::getEntropy(){
	return this->entropy;
};

int Entropy::getAmountDifferentChars(){
	return this->amountDifferentChars;
};

std::vector<unsigned char> Entropy::getMyChars(){
	return this->myChars;
};
