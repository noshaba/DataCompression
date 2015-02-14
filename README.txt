
Namen der Gruppenteilnehmer: Noshaba Cheema

Gesamtübersicht über die wesentlichen Funktionen:
(für jede Funktion der Header und eine Zeile Kommentar)
Exceptions.hpp:
	class eFileException{};
	class eFileNotFoundException : public eFileException{}; // file not found exception
	class eInvalidFileFormatException : public eFileException{}; // wrong file format
	class eHeaderNotFoundException : public eFileException{}; // header was not found
	class eMustBeFileToBeCompressedException : public eFileException{}; // must be a file to be compressed
	class eMustBeFileToBeDecompressedException : public eFileException{}; // must be a compressed file that shall be decompressed
	class eMagicNumberException : public eFileException{}; // wrong magic number of ppm

IOFile.hpp:
	std::vector<unsigned char> getTextChars(std::string filename); // gets characters from a file and saves them into a vector
	void writeFileFromVec(std::string filename, std::vector<unsigned char> chars); // writes a binary file from a vector with unsigned chars
	void writeFileFromString(std::string filename, std::string text); // writes a file from a string (no longer in use, only for testing purposes)

MyStructs.hpp:
	template <typename T> struct CharInf; // struct with the character information (character name, amount in the file, probability, decision conetent, new character bitset)

Entropy.hpp:
	CharInfMap getCharInfMapInf(); // returns the map with the character information (key: char, value: CharInf struct with the character information)
	double log2(double n); // log2 was used since MS Visual Studio 2010 did not know it
	double getEntropy(); // returns the entropy of the file (H)
	int getAmountDifferentChars(); // returns the amount of different chars in a file
	std::vector<unsigned char> getMyChars(); // returns the vector with all the chars from the read file

TreeNode.hpp:
	T getData(); // returns the data of a tree node
	void setData(T data); // sets the data of a tree node
	TreeNode* getLeft(); // gets the left node of the current node
	TreeNode* getRight(); // gets the right node of the current node
	void setLeft(TreeNode* left); // sets the left node of the current node
	void setRight(TreeNode* right); // sets the right node of the current node
	T iterThroughTree(); // iterates through the tree and returns the data of the last nodes

Huffman.hpp:
	void decompressFile(std::string filename); // decompresses a '.huffman' file
	void compressFile(std::string filename); //compressed a file with the huffman tree
	Entropy* getEnt(); // returns an Entropy object
	double getAverageWordSize(); // returns the average word size of the file (L)
	double getRedundancy(); // returns the redundancy of the file (R)
	std::vector<TreeNode<T>*> getNodeList(); // returns a vector with the nodes of the huffman tree
	void setNodeList(T data); // sets the node list
	TreeNode<T>* getSmallestNode(); // returns the node with the smallest value for the entropy compression with the Huffman tree
	void setHuffmanTree(); // sets the Huffman tree
	TreeNode<T>* getHuffmanTree(); // returns the set Huffman tree
	void encryption(TreeNode<T>* actNode, std::string code); // sets the bitsets for each character in the huffman tree
	void huffmanCoding(); // calls the function 'encryption' with parameters
	void setDataVec(); // sets the vector with all the data of the tree, in one index is one data from a tree node saved
	void setAverageWordSize(); // calculates the average word size of the file
	void setRedundancy(); // calculates the redundancy of the file
	std::string encryptedText(); // saves the encoded bitstring of the file in a string (no longer in use, only for testing purposes)
	void setCompression(); // calculates the compression
	double getCompression(); // returns the compression
	std::vector<unsigned char> encryptedTextToBinary(); // sets and returns a vector with chars with the encoded data (entropy compressed)
	std::string decodeEncryptedText(std::string encryptedText); // decodes the encrypted string from 'setEncryptedText' with the huffman tree (for testing purposes, no longer in use)
	std::vector<unsigned char> decodeEncryptedBinary(std::vector<unsigned char> encryptedBytes) // decodes the encrypted bytes with the huffman tree and returns the original values of the file
	std::string decodingDataToString(); // to print the information, when a file has been decoded successfully
	std::string encodingDataToString(); // to print the information, when a file has been encoded successfully
	std::string setTreeStringToBeSaved(); // sets the tree header in the compressed file

LossyCompression.hpp:
	void dataCompression(std::string filename, double scalar, double scalar2); // compresses a '.ppm' file with DCT and quantization
	void dataDecompression(std::string filename); // decompresses a '.lcompress' file
	void setTMat();	// sets the transformation matrix
	void setTransTMat();	// sets the (transformation matrix)^t
	void setImgRGBVec(std::vector<unsigned char> img); // saves the rgb values of the image in a vector
	void setImgTileVec(); // sets the 8x8 tiles of the image
	void setQuantizationMat(double scalar, double scalar2); // sets the quantization matrix
	void calculateDCT(); // DCT of the image
	void quantization(); // quantization of the image
	void deQuantization(); // dequantization of the image
	void calculateRMSE(); // calculates the RMSE
	void setDifferenceImg(std::string filename); // sets the difference image between the original image and the reconstructed image
	void setCompressedImg(std::string filename); // sets the compressed image
	void setDecompressedImg(std::string filename); // sets the decompressed image (reconstructed image)
	void setDecompressedHeader(); // sets the header for the reconstructed image
	void setCompressedImgValues(); // saves the compressed image values to save them in the compressed image
	std::vector<unsigned char> getCompressedImg(); // returns the compressed image
	double getRMSEMean(); // returns the RMSE
	void setHeader(); // reads the header from the image and saves it in the header struct with the header information
	std::string compressedDataToString(); // prints the information when an image has been compressed successfully
	std::string decompressedDataToString(); // prints the information when an image has been decompressed successfully

MatrixOperations.hpp:
	template <typename T> std::vector<std::vector<T> > operator*(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2); // matrix multiplication
	template <typename T> std::vector<std::vector<T> > operator*(std::vector<std::vector<T> > mat, double scalar); // scales a matrix
	template <typename T> std::vector<std::vector<T> > operator&(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2); // matrix multiplication (component wise)
	template <typename T> std::vector<std::vector<T> > operator/(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2); // matrix division (component wise)	

DataCompression.hpp:
	void menu(); // shows the menu and the options of it
	void entropyCompression(); // entropy compression of a file
	void entropyDecompression(); // entropy decompression of a '.huffman' file
	void lossyCompression(); // lossy compression of a '.ppm' image
	void decompressLossy(); // decompression of '.lcompress' file
	std::string getWelcomeText(); // welcome text


Deklaration der Hilfsmittel:
Außer der Standard-Bibliothek wurden keine weiteren Hilfsmittel verwendet.

Angehängte Dateien:
DataCompression.cpp
DataCompression.hpp
Entropy.cpp
Entropy.hpp
Exceptions.hpp
Huffman.hpp
IOFile.cpp
IOFile.hpp
LossyCompression.cpp
LossyCompression.hpp
Main.cpp
MatrixOperations.hpp
MyStructs.hpp
TreeNode.hpp

lenaDiffImage.ppm
XDiffImage.ppm

DataCompression.exe

FRAGEBOGEN
------------------------------------------------------------
Übung 3 Entropie-Codierung eines Beispiel-Textes:
AB13432BB1BAAAZ11BA142111DCC9344AAAAAYAXXX

H: 3.12629
L: 3.16667
R: 0.0403769
Dateilänge in Bytes: original - 336 bit (42 byte), codiert - 133 bit (16 byte)

Codierungstabelle:
A: 10
1: 111
B: 011
4: 000
3: 1100
X: 1101
2: 0010
C: 0011
9: 01000
D: 01001
Y: 01010
Z: 01011
------------------------------------------------------------
Übung 4 Codierung eines Bitmaps:
lena.ppm oder X.ppm

lena.ppm:
H: 7.63828
L: 7.66437
R: 0.0260905
Dateilänge in Bytes: original - 1573120 bit (196640 byte), codiert - 1507122 bit (188390 byte)

x.ppm:
H: 4.07989
L: 4.11538
R: 0.0354924
Dateilänge in Bytes: original - 1664 bit (208 byte), codiert - 856 bit (107 byte)
------------------------------------------------------------
Übung 5 Verlustvolle Kompression mit DCT:
lena.ppm oder X.ppm

x.ppm:
H: 3.40422
L: 3.43779
R: 0.0335645
Dateilänge in Bytes: original - 1736 bit (217 byte), codiert - 746 bit (93 byte)
RMSE: 4.90185
Quantisierungsmatrix: 
	[1        0.875    0.75     0.625    0.5      0.375    0.25     0.125   ;
	 0.875    0.765625 0.65625  0.546875 0.4375   0.328125 0.21875  0.109375;
	 0.75     0.65625  0.5625   0.46875  0.375    0.28125  0.1875   0.09375 ;
	 0.625    0.546875 0.46875  0.390625 0.3125   0.234375 0.15625  0.078125;
	 0.5      0.4375   0.375    0.3125   0.25     0.1875   0.125    0.0625  ;
	 0.375    0.328125 0.28125  0.234375 0.1875   0.140625 0.09375  0.046875;
	 0.25     0.21875  0.1875   0.15625  0.125    0.09375  0.0625   0.03125 ;
(	 0.125    0.109375 0.09375  0.078125 0.0625   0.046875 0.03125  0.015625] + (1-i*j/64.0*scalar2) )* scalar
scalar = 0.1
scalar2 = 1

lena.ppm:
H: 2.92097
L: 2.93407
R: 0.0130996
Dateilänge in Bytes: original - 1573088 bit (196636 byte), codiert - 576944bit (72118 byte)
RMSE: 4.97114
Quantisierungsmatrix: 
	[1        0.875    0.75     0.625    0.5      0.375    0.25     0.125   ;
	 0.875    0.765625 0.65625  0.546875 0.4375   0.328125 0.21875  0.109375;
	 0.75     0.65625  0.5625   0.46875  0.375    0.28125  0.1875   0.09375 ;
	 0.625    0.546875 0.46875  0.390625 0.3125   0.234375 0.15625  0.078125;
	 0.5      0.4375   0.375    0.3125   0.25     0.1875   0.125    0.0625  ;
	 0.375    0.328125 0.28125  0.234375 0.1875   0.140625 0.09375  0.046875;
	 0.25     0.21875  0.1875   0.15625  0.125    0.09375  0.0625   0.03125 ;
(	 0.125    0.109375 0.09375  0.078125 0.0625   0.046875 0.03125  0.015625] + (1-i*j/64.0*scalar2) )* scalar
scalar = 0.1
scalar2 = 1
Differenzbild (bitte als ppm mit grau(128) als Nullwert)
-------------------------------------------------------------