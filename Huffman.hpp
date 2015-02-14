#ifndef HUFFMAN_HPP
	#define HUFFMAN_HPP
	
	#include <sstream>
	#include <iomanip> 
	#include <algorithm>

	#include "Entropy.hpp"
	#include "TreeNode.hpp"

	template <typename T> class Huffman{

		private:
			std::vector<TreeNode<T>*> nodeList;	// a vector with all the tree nodes
			std::vector<TreeNode<T>*> headerInf; // copy of nodeList vector (since nodeList changes during runtime), used to set the tree header in a compressed file
			std::vector<T> dataVec; // a vector with all the data of the tree, in one index is one data from a tree node saved
			std::string filename; // the file name that was read from
			Entropy *ent; // Entropy object
			TreeNode<T>* huffmanTree; // tree
			double averageWordSize;	// average word size of the file (L)
			double redundancy; // redundancy of the file (R)
			double compression; // compression of the file in % (fileSizeAfter/fileSizeBefore)
			int fileSizeBefore; // file size before the compression
			int fileSizeAfter; // file size after the compression
			bool decode; // flag to see if the file shall be compressed or decompressed

		public:

			Huffman(){}; // constructor
			
			/**
				Decompresses a '.huffman' file
				@param: filename
			*/
			
			void decompressFile(std::string filename){
				std::vector<unsigned char> myChars;
				try{
					myChars = getTextChars(filename);
				} catch (...) {
					throw eFileNotFoundException();
				}
				if(filename.substr(filename.find_last_of(".") + 1) == "huffman"){
					bool decode = true;
					this->filename = filename;
					std::string buffer;
					for(unsigned int i = 0; i < 15; i++){
						buffer += myChars[i];
					}
					if(buffer == "HuffmanHeader::"){
						if(!this->nodeList.empty()) this->nodeList.clear();
						if(this->huffmanTree != NULL) this->huffmanTree == NULL;
						std::vector<unsigned char> headerData;
						int headerSize;
						for(unsigned int i = 15; i < myChars.size()-14;i++){
							std::stringstream ss;
							ss << myChars[i] << myChars[i+1] << myChars[i+2] << myChars[i+3] << myChars[i+4]
								<< myChars[i+5] << myChars[i+6] << myChars[i+7] << myChars[i+8] << myChars[i+9]
								<< myChars[i+10] << myChars[i+11] << myChars[i+12] << myChars[i+13];
							if(ss.str() == "HuffmanHeader\n"){
								headerSize = i + 13;
								break;
							} else {
								headerData.push_back(myChars[i]);
							}
						}
						std::string headerDataString;
						for(unsigned int i = 0; i < headerData.size(); i++) headerDataString += headerData[i];
						std::string delimiter = "::";
						size_t pos = 0;
						std::string token;
						std::vector<std::string> tokens;
						while((pos = headerDataString.find(delimiter)) != std::string::npos){
							token = headerDataString.substr(0, pos);
							tokens.push_back(token);
							headerDataString.erase(0, pos + delimiter.length());
						}
						for(unsigned int i = 1; i < tokens.size(); i++){
							CharInf<unsigned char> data;
							data.charName = tokens[i].at(0);
							tokens[i].erase(tokens[i].begin(),tokens[i].begin()+2);
							data.charNum = atoi(tokens[i].c_str());
							TreeNode<T>* newNode = new TreeNode<T>(data);
							this->nodeList.push_back(newNode);
						}
						std::vector<unsigned char> encodedText;
						for(unsigned int i = headerSize + 1; i < myChars.size(); i++) encodedText.push_back(myChars[i]);
						this->setHuffmanTree();
						this->huffmanCoding();
						this->setDataVec(this->huffmanTree);
						std::sort(this->dataVec.begin(), this->dataVec.end());
						filename.erase(filename.end() - filename.substr(filename.find_last_of(".")+1).size() - 1, filename.end());
						std::string newFileName = filename + "Decoded." + tokens[0];
						writeFileFromVec(newFileName, this->decodeEncryptedBinary(encodedText));
					} else {
						throw eHeaderNotFoundException();
					}
				} else {
					throw eInvalidFileFormatException();
				}
			};
			
			/**
				compresses a huffman file
				@param: filename
			*/
			
			void compressFile(std::string filename){
				try{
					bool decode = false;
					this->filename = filename;
					this->ent = new Entropy(filename);
					Entropy::CharInfMap charInfMapInf = this->ent->getCharInfMapInf();
					for(Entropy::CharInfMapIt iterator = charInfMapInf.begin(); iterator != charInfMapInf.end(); iterator++){
						this->setNodeList(iterator->second);
					}
					this->headerInf = this->nodeList;
					this->setHuffmanTree();
					this->huffmanCoding();
					this->setDataVec(this->huffmanTree);
					std::sort(this->dataVec.begin(), this->dataVec.end());
					this->setAverageWordSize();
					this->setRedundancy();
					this->setCompression();
					filename.erase(filename.end() - filename.substr(filename.find_last_of(".") + 1).size(), filename.end());
					std::string newFileName = filename + "huffman";
					writeFileFromVec(newFileName, this->encryptedTextToBinary());
				} catch (...) {
					throw eFileNotFoundException();
				}
			};

			~Huffman(){
				delete this->ent;
				for(unsigned int i = 0; i < this->nodeList.size(); i++){
					delete nodeList[i];
				}
			};
			
			/**
				returns an Entropy object
			*/

			Entropy* getEnt(){
				return this->ent;
			};
			
			/**
				returns the average word size of the file (L)
			*/
			
			double getAverageWordSize(){
				return this->averageWordSize;
			};
			
			/**
				returns the redundancy of the file (R)
			*/
			
			double getRedundancy(){
				return this->redundancy;
			};
			
			/**
				returns a vector with the nodes of the huffman tree
			*/

			std::vector<TreeNode<T>*> getNodeList(){
				return this->nodeList;
			};
			
			/**
				sets the node list
			*/

			void setNodeList(T data){
				this->nodeList.push_back(new TreeNode<T>(data));
			};
			
			/**
				returns the node with the smallest value for the entropy compression with the Huffman tree
			*/

			TreeNode<T>* getSmallestNode(){
				int pos = 0;
				TreeNode<T>* smallestNode = this->nodeList[pos];
				for(unsigned int i = 0; i < this->nodeList.size(); i++){
					if((this->nodeList[i]->getData().charNum) < (smallestNode->getData().charNum)){
						smallestNode = this->nodeList[i];
						pos = i;
					}
				}
				this->nodeList.erase(this->nodeList.begin() + pos);
				return smallestNode;
			};
			
			/**
				sets the Huffman tree
			*/

			void setHuffmanTree(){
				TreeNode<T>* leftNode;
				TreeNode<T>* rightNode;
				TreeNode<T>* newNode;
				CharInf<unsigned char> data;

				while(this->nodeList.size() != 1){
					leftNode = this->getSmallestNode();
					rightNode = this->getSmallestNode();
					
					data.charName = leftNode->getData().charName + rightNode->getData().charName;
					data.charNum = leftNode->getData().charNum + rightNode->getData().charNum;
					data.charProb = leftNode->getData().charProb + rightNode->getData().charProb;
					data.charNat = leftNode->getData().charNat + rightNode->getData().charNat;
					newNode = new TreeNode<T>(data);
					
					newNode->setLeft(leftNode);
					newNode->setRight(rightNode);

					this->nodeList.push_back(newNode);
				}
				this->huffmanTree = this->nodeList[0];
			};
			
			/**
				returns the set Huffman tree
			*/
			
			TreeNode<T>* getHuffmanTree(){
				return this->huffmanTree;
			}
			
			/**
				sets the bitsets for each character in the huffman tree
				@param: tree node, code of tree node
			*/
			
			void encryption(TreeNode<T>* actNode, std::string code){
				CharInf<unsigned char> newData;
				
				if(actNode->getLeft() != NULL && actNode->getRight() != NULL){
					newData.charName = actNode->getLeft()->getData().charName;
					newData.charNum = actNode->getLeft()->getData().charNum;
					newData.charProb = actNode->getLeft()->getData().charProb;
					newData.charNat = actNode->getLeft()->getData().charNat;
					newData.charBitset = code.append("0");
					actNode->getLeft()->setData(newData);
					code.erase(code.end()-1);
					
					newData.charName = actNode->getRight()->getData().charName;
					newData.charNum = actNode->getRight()->getData().charNum;
					newData.charProb = actNode->getRight()->getData().charProb;
					newData.charNat = actNode->getRight()->getData().charNat;
					newData.charBitset = code.append("1");
					actNode->getRight()->setData(newData);
					code.erase(code.end()-1);
					
					this->encryption(actNode->getLeft(), code.append("0"));
					code.erase(code.end()-1);
					this->encryption(actNode->getRight(), code.append("1"));
					code.erase(code.end()-1);
				}
			};
			
			/**
				calls the function 'encryption' with parameters
			*/
			
			void huffmanCoding(){
				this->encryption(this->getHuffmanTree(),"");
			};
			
			/**
				sets the vector with all the data of the tree, in one index is one data from a tree node saved
				@param: actNode - current node
			*/

			void setDataVec(TreeNode<T>* actNode){
				if(actNode != NULL){
					if(actNode->getLeft() == NULL && actNode->getRight() == NULL){
						this->dataVec.push_back(actNode->getData());
					}
					this->setDataVec(actNode->getLeft());
					this->setDataVec(actNode->getRight());
				}
			};
			
			/**
				calculates the average word size of the file
			*/
			
			void setAverageWordSize(){
				this->averageWordSize = 0;
				for(unsigned int i = 0; i < this->dataVec.size(); i++){
					this->averageWordSize += (this->dataVec[i].charProb) * (this->dataVec[i].charBitset.size());
				}
			};
			
			/**
				calculates the redundancy of the file
			*/
			
			void setRedundancy(){
				this->redundancy = this->averageWordSize - this->ent->getEntropy();
			};
			
			/**
				saves the encoded bitstring of the file in a string (no longer in use, only for testing purposes)
			*/
			std::string encryptedText(){
				std::string myString;
				try{
					std::vector<unsigned char> textChars = this->getEnt()->getMyChars();
					for(unsigned int i = 0; i < textChars.size(); i++){
						for(unsigned int j = 0; j < this->dataVec.size(); j++){
							if(textChars[i] == this->dataVec[j].charName){
								myString += this->dataVec[j].charBitset;
							}
						}
					}
				} catch(...){
					throw eFileNotFoundException();
				}
				
				return myString;
			};
			
			/**
				calculates the compression in %
			*/
			
			void setCompression(){
				this->fileSizeBefore = this->ent->getMyChars().size() * 8;
				this->fileSizeAfter = this->encryptedText().size();
				this->compression = (((double) this->encryptedText().size()) / ((double)this->ent->getMyChars().size() * 8)) * 100;
			};
			
			/**
				returns the compression
			*/
			
			double getCompression(){
				return this->compression;
			};
			
			/**
				sets and returns a vector with chars with the encoded data (entropy compressed)
			*/
			
			std::vector<unsigned char> encryptedTextToBinary(){
				std::vector<unsigned char> myBytes;
				std::string treeString = this->setTreeStringToBeSaved();
				for(unsigned int i = 0; i < treeString.size(); i++) myBytes.push_back(treeString.at(i));
				std::string encryptedText = this->encryptedText();
				for(unsigned int i = 0; i < encryptedText.size(); i++){
					if(i % 8 == 0){
						unsigned char c = 0;
						myBytes.push_back(c);
					}
					if(encryptedText.at(i) == '1') myBytes[myBytes.size()-1] |= 1 << (i % 8);
				}
				return myBytes;
			};
			
			/**
				decodes the encrypted string from 'setEncryptedText' with the huffman tree (for testing purposes, no longer in use)
			*/
			std::string decodeEncryptedText(std::string encryptedText){
				std::string myDecodedText;
				TreeNode<T>* actNode = this->huffmanTree;
				for(int i = 0; i < encryptedText.size(); i++){
					if(actNode->getLeft() != NULL && actNode->getRight() != NULL){
						if(encryptedText.at(i) == '0') actNode = actNode->getLeft();
						if(encryptedText.at(i) == '1') actNode = actNode->getRight();
					} else {
						myDecodedText += actNode->getData().charName;
						actNode = this->huffmanTree;
						if(actNode->getLeft() != NULL && actNode->getRight() != NULL){
							if(encryptedText.at(i) == '0') actNode = actNode->getLeft();
							if(encryptedText.at(i) == '1') actNode = actNode->getRight();
						}
					}
				}
				
				return myDecodedText;
			};
			
			/**
				decodes the encrypted bytes with the huffman tree and returns the original values of the file
				@param: encryptedBytes - vector with coded bytes
			*/
			
			std::vector<unsigned char> decodeEncryptedBinary(std::vector<unsigned char> encryptedBytes){
				std::vector<unsigned char> myDecodedBytes;
				TreeNode<T>* actNode = this->huffmanTree;
				int counter = 0;
				while(myDecodedBytes.size() <= 5){
					for(unsigned int i = 0; i < encryptedBytes.size(); i++){
						for(unsigned int b = 0; b < 8; b++){
							if(actNode->getLeft() != NULL && actNode->getRight() != NULL){
								if(encryptedBytes[i] & (1 << b)){
									actNode = actNode->getRight();
								} else {
									actNode = actNode->getLeft();
								}
							} else {
								myDecodedBytes.push_back(actNode->getData().charName);
								counter++;
								actNode = this->huffmanTree;
								if(actNode->getLeft() != NULL && actNode->getRight() != NULL){
									if(encryptedBytes[i] & (1 << b)){
										actNode = actNode->getRight();
									} else {
										actNode = actNode->getLeft();
									}
								}
							}
						}
					}
				}
				return myDecodedBytes;
			};
			
			/**
				to print the information, when a file has been decoded successfully
			*/
			
			std::string decodingDataToString(){
				std::string myString;
				std::stringstream ss;
				if(this->decode){
					ss << "The file '" << this->filename << "' has been successfully decoded.\n"
						<< "\n\nC:\tAmount:\tCode:\n\n";
					for(unsigned int i = 0; i < this->dataVec.size(); i++){
						ss << "'" << this->dataVec[i].charName << "'" << "\t" << this->dataVec[i].charNum
							<<"\t" << this->dataVec[i].charBitset << "\n";
					}
				} else {
					throw eMustBeFileToBeDecompressedException();
				}
				myString += ss.str();
				return myString;
			};
			
			/**
				to print the information, when a file has been encoded successfully
			*/
			
			std::string encodingDataToString(){
				std::string myString;
				std::stringstream ss;
				if(!this->decode){
					ss << "Amount of different chars: " << this->ent->getAmountDifferentChars() 
						<< "\nEntropy(H) of '" << this->filename << "' : " << this->ent->getEntropy() 
						<< "\nAverage word size(L): " << this->averageWordSize 
						<< "\nRedundancy(R): " << this->redundancy
						<< "\nFile size before compression: " << this->fileSizeBefore << " bit (" << this->fileSizeBefore/8 << " byte)"
						<< "\nFile size after compression:  " << this->fileSizeAfter << " bit (" << this->fileSizeAfter/8 << " byte)"
						<< "\nCompression: " << this->compression << "% of original image size"
						<< "\n\nC:\tAmount:\tProbability:\tNat (Decision Content):\tCode:\n\n";
					for(unsigned int i = 0; i < this->dataVec.size(); i++){
						ss << "'" << this->dataVec[i].charName << "'" << "\t" << this->dataVec[i].charNum
							<<"\t" << std::fixed << std::setprecision(10) << this->dataVec[i].charProb
							<<"\t\t" << std::fixed << std::setprecision(10) << this->dataVec[i].charNat
							<<"\t" << this->dataVec[i].charBitset << "\n";
					}
				} else {
					throw eMustBeFileToBeCompressedException();
				}
				myString += ss.str();
				return myString;
			};
			
			/**
				sets the tree header in the compressed file
			*/
			
			std::string setTreeStringToBeSaved(){
				std::string treeStringToBeSaved = "HuffmanHeader::";
				std::stringstream ss;
				for(unsigned int i = 0; i < this->headerInf.size(); i++){
					ss << this->headerInf[i]->getData().charName << '=' << this->headerInf[i]->getData().charNum << "::";
				}
				treeStringToBeSaved += this->filename.substr(this->filename.find_last_of(".") + 1) + "::";
				treeStringToBeSaved += ss.str();
				treeStringToBeSaved += "HuffmanHeader\n";
				return treeStringToBeSaved;
			};
	};
#endif