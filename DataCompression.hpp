#ifndef DATACOMPRESSION_HPP
	#define DATACOMPRESSION_HPP
	
	#include "Huffman.hpp"
	#include "LossyCompression.hpp"

	class DataCompression {
		private:
			std::string welcomeText;
			std::string menuText;
			Huffman<CharInf<unsigned char> > *hc;
			LossyCompression *lc;
		public:
			DataCompression();
			~DataCompression();
			void menu(); /** shows the menu and the options of it*/
			void entropyCompression(); /** entropy compression of a file*/
			void entropyDecompression(); /** entropy decompression of a '.huffman' file*/
			void lossyCompression(); /** lossy compression of a '.ppm' image*/
			void decompressLossy(); /** decompression of '.lcompress' file*/
			std::string getWelcomeText(); /** returns the welcome text*/
	};
#endif