#include "DataCompression.hpp"

DataCompression::DataCompression(){
	this->welcomeText = "Welcome!\n";
	this->menuText = "Press 'E' for lossless Entropy compression of a file (Entropy compression with Huffman tree). This option creates a '.huffman' file from your file.\nPress 'H' to decompress a '.huffman' file.\nPress 'L' for lossy image compression of a Portable Pixmap (P6). This option will create an '.lcompress' file and the '.huffman' file of it.\nIt will also create a '..DiffImage.ppm' which shows the difference between the original image and the image after decoding the created '.lcompress' file (gray 128 as 0).\nPress 'D' to decompress a '.lcompress' file.\nPress 'Q' to quit.\n";
};

DataCompression::~DataCompression(){
	delete this->hc;
	delete this->lc;
};

std::string DataCompression::getWelcomeText(){
	return this->welcomeText;
};

void DataCompression::menu(){
	std::cout << this->menuText << std::endl;
	char input = ' ';
	std::cin >> input;
	switch(input){
		case 'e': this->entropyCompression();
			break;
		case 'h': this->entropyDecompression();
			break;
		case 'l': this->lossyCompression();
			break;
		case 'd': this->decompressLossy();
			break;
		case 'q': exit(0);
			break;
		default: std::cout << "Unknown input\n";
			this->menu();
			break;
	}
};

void DataCompression::entropyCompression(){
	this->hc = new Huffman<CharInf<unsigned char> >();
	std::cout << "Enter the file name of your file or the path to your file you would like to compress (name should have no whitespaces):" << std::endl;
	std::string path;
	std::cin >> path;
	try{
		this->hc->compressFile(path);
		std::cout << hc->encodingDataToString() << std::endl;
	} catch (...){
		throw;
	}
	this->menu();
};

void DataCompression::entropyDecompression(){
	this->hc = new Huffman<CharInf<unsigned char> >();
	std::cout << "Enter the file name of your '.huffman' file or the path to your '.huffman' file you would like to decompress (name should have no whitespaces):" << std::endl;
	std::string path;
	std::cin >> path;
	try{
		this->hc->decompressFile(path);
		std::cout << hc->decodingDataToString() << std::endl;
	} catch (...){
		throw;
	}
	this->menu();
};

void DataCompression::lossyCompression(){
	this->lc = new LossyCompression();
	std::cout << "Enter the file name of your file or the path to your '.ppm' file you would like to compress (name should have no whitespaces):" << std::endl;
	std::string path;
	std::cin >> path;
	std::cout << "Enter a scalar to scale the quantization matrix (scalar must be >= 0):" << std::endl;
	std::cout << "The higher the scalar is, the lesser compressed the image will be. Recommended scalar is 0.1." << std::endl;
	double scalar1;
	std::cin >> scalar1;
	std::cout << "Enter a second scalar which scales rows and cols with the index > 0 depending on the row and col with the index 0 of the matrix." << std::endl;
	std::cout << "This scalar must be >= 1. Recommended scalar is 1." << std::endl;
	double scalar2;
	std::cin >> scalar2;
	try{
		this->lc->dataCompression(path, scalar1, scalar2);
		std::cout << this->lc->compressedDataToString() << std::endl;
	} catch (...) {
		throw;
	}
	this->menu();
};

void DataCompression::decompressLossy(){
	this->lc = new LossyCompression();
	std::cout << "Enter the file name of your '.lcompress' file or the path to your '.lcompress' file you would like to decompress (name should have no whitespaces):" << std::endl;
	std::string path;
	std::cin >> path;
	try{
		this->lc->dataDecompression(path);
		std::cout << this->lc->decompressedDataToString() << std::endl;
	} catch (...) {
		throw;
	}
	this->menu();
};