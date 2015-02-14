#ifndef LOSSYCOMPRESSION_HPP
	#define LOSSYCOMPRESSION_HPP
	
	#include <math.h>
	#include <stdlib.h>
	#include <sstream>
	#include <algorithm>
	#include <limits>

	#include "IOFile.hpp"
	#include "MatrixOperations.hpp"
	#include "Huffman.hpp"
	
	#define PI 4.0*atan(1.0)
	#define TMATWIDTH 8
	#define TMATHEIGHT 8
	#define GRAYDIFF 128
	
	class LossyCompression{
	
		public:
			// struct with the RGB values and the operators
			struct RGB{
				double r, g, b;
				struct RGB operator*(RGB rhs){
					RGB rgb;
					rgb.r = r * rhs.r;
					rgb.g = g * rhs.g;
					rgb.b = b * rhs.b;
					return rgb;
				};
				struct RGB operator/(RGB rhs){
					RGB rgb;
					rgb.r = r / rhs.r;
					rgb.g = g / rhs.g;
					rgb.b = b / rhs.b;
					return rgb;
				};
				struct RGB operator*(double rhs){
					RGB rgb;
					rgb.r = r * rhs;
					rgb.g = g * rhs;
					rgb.b = b * rhs;
					return rgb;
				};
				struct RGB operator+(RGB rhs){
					RGB rgb;
					rgb.r = r + rhs.r;
					rgb.g = g + rhs.g;
					rgb.b = b + rhs.b;
					return rgb;
				};
				struct RGB operator-(RGB rhs){
					RGB rgb;
					rgb.r = r - rhs.r;
					rgb.g = g - rhs.g;
					rgb.b = b - rhs.b;
					return rgb;
				};
			};
			LossyCompression(); // constructor
			~LossyCompression(); // destructor
			/** 
				compresses a '.ppm' file with DCT and quantization
				@param: filename, scalar, scalar2
			*/
			void dataCompression(std::string filename, double scalar, double scalar2);
			void dataDecompression(std::string filename); /** decompresses a '.lcompress' file @param: filename*/
			void setTMat();	/** sets the transformation matrix*/
			void setTransTMat();	/** sets the (transformation matrix)^t*/
			void setImgRGBVec(std::vector<unsigned char> img); /** saves the rgb values of the image in a vector*/
			void setImgTileVec(); /** sets the 8x8 tiles of the image*/
			/**
				sets the quantization matrix
				@param: scalar, scalar2
			*/
			void setQuantizationMat(double scalar, double scalar2); 
			void calculateDCT(); /** DCT of the image*/
			void quantization(); /** quantization of the image*/
			void deQuantization(); /** dequantization of the image*/
			void calculateRMSE(); /** calculates the RMSE*/
			void setDifferenceImg(std::string filename); /** sets the difference image between the original image and the reconstructed image, @param: filename for new filename*/
			void setCompressedImg(std::string filename); /** sets the compressed image, @param: filename for new filename*/
			void setDecompressedImg(std::string filename); /** sets the decompressed image (reconstructed image), @param: filename for new filename*/
			void setDecompressedHeader(); /** sets the header for the reconstructed image*/
			void setCompressedImgValues(); /** saves the compressed image values to save them in the compressed image*/
			std::vector<unsigned char> getCompressedImg(); /** returns the compressed image*/
			double getRMSEMean(); /** returns the RMSE*/
			void setHeader(); /** reads the header from the image and saves it in the header struct with the header information*/
			std::string compressedDataToString(); /** prints the information when an image has been compressed successfully*/
			std::string decompressedDataToString(); /** prints the information when an image has been decompressed successfully*/
	
		private:
			std::string filename;
			bool compressed;
			double scalar;
			double scalar2;
			int addedValue;
			std::vector<unsigned char> img;
			std::vector<int> compressedImgValues;
			std::vector<unsigned char> differenceImg;
			std::vector<unsigned char> compressedImg;
			std::vector<unsigned char> decompressedImg;
			std::vector<std::vector<RGB> > tMat;
			std::vector<std::vector<RGB> > transTMat;
			std::vector<std::vector<RGB> > quantizationMat;
			std::vector<RGB> imgRGBVec;
			std::vector<std::vector<std::vector<RGB> > > imgTileVec;
			std::vector<std::vector<std::vector<RGB> > > DCTTileVec;
			std::vector<std::vector<std::vector<RGB> > > quantizationTileVec;
			std::vector<std::vector<std::vector<RGB> > > invDCTTileVec;
			std::vector<std::vector<std::vector<RGB> > > ERRTileVec;
			std::vector<double> RMSETileVec;
			double RMSEMean;
			struct Header{
				std::string magicNumber;
				int imgWidth;
				int imgHeight;
				int maxValue;
				int size;
			} header;
			Huffman<CharInf<unsigned char> > *huffman;
	};
	
#endif