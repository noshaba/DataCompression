#include "LossyCompression.hpp"

LossyCompression::LossyCompression(){};

LossyCompression::~LossyCompression(){
	delete this->huffman;
};

void LossyCompression::dataCompression(std::string filename, double scalar, double scalar2){
	if(filename.substr(filename.find_last_of(".") + 1) == "pbm" || filename.substr(filename.find_last_of(".") + 1) == "pgm" || filename.substr(filename.find_last_of(".") + 1) == "ppm"){
		if(scalar >= 0 && scalar2 >= 1){
			this->compressed = false;
			this->filename = filename;
			this->scalar = scalar;
			this->scalar2 = scalar2;
			this->addedValue = 0;
			this->img = getTextChars(filename);
			this->setHeader();
			this->setTMat();
			this->setQuantizationMat(scalar, scalar2);
			this->setImgRGBVec(this->img);
			this->setImgTileVec();
			this->calculateDCT();
			this->quantization();
			this->calculateRMSE();
			this->setCompressedImg(filename);
			this->setDifferenceImg(filename);
		} else {
			std::cerr << "ERROR: Scalar 1 must be >= 0, scalar 2 must be >= 1." << std::endl;
		}
	} else {
		throw;
	}
};

void LossyCompression::dataDecompression(std::string filename){
	if(filename.substr(filename.find_last_of(".") + 1) == "lcompress"){
		this->compressed = true;
		this->scalar = 0;
		this->scalar2 = 0;
		this->addedValue = 0;
		try{
			this->img = getTextChars(filename);
			this->setHeader();
			this->setTMat();
			this->setQuantizationMat(this->scalar, this->scalar2);
			this->setImgRGBVec(this->img);
			this->setImgTileVec();
			this->deQuantization();
			this->setDecompressedImg(filename);
		} catch(...){
			throw;
		}
	} else {
		throw;
	}
};

void LossyCompression::setHeader(){

	this->header.imgWidth = this->header.imgHeight = this->header.maxValue = 0;
	std::string magicNumber = "";
	std::string buffer = "";
	
	for(unsigned int i = 0; i < 2; i++){
		magicNumber += this->img[i];
	}

	if(magicNumber == "P6"){
		this->header.magicNumber = magicNumber;
		for(unsigned int i = 2; i < this->img.size(); i++){
			// check for comments and delete them from the image vector
			if(this->img[i] == '#' && (this->header.imgWidth == 0 || this->header.imgHeight == 0 || this->header.maxValue == 0)){
				for(unsigned int j = i; j < this->img.size(); j++){
					if(this->img[j] == '\n' || this->img[j] == '\r'){
						this->img.erase(this->img.begin()+i, this->img.begin()+j);
						break;
					}
				}
			}
			// if this->img[i] is between the ASCII code of 0 and 9 or a . or / for floating values (. = 46, / = 47, 0 to 9 = 48 to 57)
			if(this->img[i] >= 46 && this->img[i] <= 57){
				buffer += this->img[i];
			}
			
			if(this->img[i] == ' ' || this->img[i] == '\t' || this->img[i] == '\n' || this->img[i] == '\r'){
				if(buffer != "" && this->header.imgWidth == 0){
					this->header.imgWidth = atoi(buffer.c_str());
					buffer = "";
					continue;
				} else if(buffer != "" && this->header.imgHeight == 0){
					this->header.imgHeight = atoi(buffer.c_str());
					buffer = "";
					continue;
				} else if(buffer != "" && this->header.maxValue == 0 && !this->compressed){
					this->header.maxValue = atoi(buffer.c_str());
					this->header.size = i+1;
					buffer = "";
					break;
				} else if(buffer != "" && this->header.maxValue == 0 && this->compressed){
					this->header.maxValue = atoi(buffer.c_str());
					buffer = "";
					continue;
				} else if(buffer != "" && this->scalar == 0 && this->compressed){
					this->scalar = atof(buffer.c_str());
					buffer = "";
					continue;
				} else if(buffer != "" && this->scalar2 == 0 && this->compressed){
					this->scalar2 = atof(buffer.c_str());
					buffer = "";
					continue;
				} else if(buffer != "" && this->addedValue == 0 && this->compressed){
					this->addedValue = atoi(buffer.c_str());
					this->header.size = i+1;
					buffer = "";
					break;
				}
			}
		}
	} else {
		throw;
	}
};

void LossyCompression::setTMat(){
	RGB rgbValue;
	for(unsigned int i = 0; i < TMATHEIGHT; i++){
		std::vector<RGB> row;
		for(unsigned int j = 0; j < TMATWIDTH; j++){
			if(j == 0){
				rgbValue.r = rgbValue.g = rgbValue.b = 1/sqrt(8);
				row.push_back(rgbValue);
			} else {
				rgbValue.r = rgbValue.g = rgbValue.b = 0.5*cos((2*i+1)*j*PI/16);
				row.push_back(rgbValue);
			}
		}
		this->tMat.push_back(row);
	}
	this->setTransTMat();
};

void LossyCompression::setTransTMat(){
	for(unsigned int i = 0; i < this->tMat.size(); i++){
		std::vector<RGB> row;
		for(unsigned int j = 0; j < this->tMat[i].size();j++){
			row.push_back(this->tMat[j][i]);
		}
		this->transTMat.push_back(row);
	}
};

void LossyCompression::setQuantizationMat(double scalar, double scalar2){
/*
	[1-0/8  1-1/8  1-2/8  1-3/8  1-4/8  1-5/8  1-6/8  1-7/8;
	 1-1/8
	 1-2/8       .                                      .
	 1-3/8
	 1-4/8                .                             .
	 1-5/8
	 1-6/8                         .                    .
	 1-7/8       .        .        .       (1-7/8) - 7*(1-7/8)/8]
	 
	 ==> Q(i,j)=(1-i/8)-j*((1-i/8)/8)  i,j = [0,7] in N
	 
	[1        0.875    0.75     0.625    0.5      0.375    0.25     0.125   ;
	 0.875    0.765625 0.65625  0.546875 0.4375   0.328125 0.21875  0.109375;
	 0.75     0.65625  0.5625   0.46875  0.375    0.28125  0.1875   0.09375 ;
	 0.625    0.546875 0.46875  0.390625 0.3125   0.234375 0.15625  0.078125;
	 0.5      0.4375   0.375    0.3125   0.25     0.1875   0.125    0.0625  ;
	 0.375    0.328125 0.28125  0.234375 0.1875   0.140625 0.09375  0.046875;
	 0.25     0.21875  0.1875   0.15625  0.125    0.09375  0.0625   0.03125 ;
	 0.125    0.109375 0.09375  0.078125 0.0625   0.046875 0.03125  0.015625] + (1-i*j/64.0*scalar2) )* scalar
	 
	 scalar2 is used scale the cols and rows from 1 to 7, lower values are scaled more than higher values but the value before and above the value at[i,j] are the amplitudes of the value at[i,j]
*/
	RGB rgbValue;
	for(unsigned int i = 0; i < TMATHEIGHT; i++){
		std::vector<RGB> row;
		for(unsigned int j = 0; j < TMATWIDTH; j++){
			rgbValue.r = rgbValue.g = rgbValue.b = scalar*(((1-i*0.125)-j*(1-i*0.125)/8)+(1-i*j/64.0*scalar2))/2.0;
			row.push_back(rgbValue);
		}
		this->quantizationMat.push_back(row);
	}
};

void LossyCompression::setImgRGBVec(std::vector<unsigned char> img){
	RGB rgbValue;
	img.erase(img.begin(), img.begin()+this->header.size);
	
	for(unsigned int i = 0; i < img.size(); i++){
		if(i % 3 == 0){
			rgbValue.r = img[i] - this->addedValue;
		} else if(i % 3 == 1){
			rgbValue.g = img[i] - this->addedValue;
		} else if(i % 3 == 2){
			rgbValue.b = img[i] - this->addedValue;
			this->imgRGBVec.push_back(rgbValue);
		}
	}
};

void LossyCompression::setImgTileVec(){
	std::vector<std::vector<RGB> >imgTile;
	std::vector<RGB> row;
	for(unsigned int i = 0; i < this->imgRGBVec.size(); i++){
		row.push_back(this->imgRGBVec[i]);
		if(row.size() == TMATWIDTH){
			imgTile.push_back(row);
			row.clear();
		}
		
		if(imgTile.size() == TMATHEIGHT){
			this->imgTileVec.push_back(imgTile);
			imgTile.clear();
		}
	}
	
	RGB rgbVal;
	rgbVal.r = rgbVal.g = rgbVal.b = 0;
	
	if(!imgTile.empty()){
		for(unsigned int i = row.size(); i < TMATWIDTH; i++){
			row.push_back(rgbVal);
		}
		imgTile.push_back(row);
		row.clear();
		if(imgTile.size() == TMATHEIGHT){
			this->imgTileVec.push_back(imgTile);
			imgTile.clear();
		} else {
			for(unsigned int i = 0; i < TMATWIDTH; i++){
				row.push_back(rgbVal);
			}
			for(unsigned int i = imgTile.size(); i < TMATHEIGHT; i++){
				imgTile.push_back(row);
			}
			this->imgTileVec.push_back(imgTile);
			imgTile.clear();
			row.clear();
		}
	}
};

void LossyCompression::calculateDCT(){
	std::vector<std::vector<RGB> > DCTMat;
	for(unsigned int i = 0; i < this->imgTileVec.size(); i++){
		DCTMat = this->transTMat * this->imgTileVec[i] * this->tMat;
		this->DCTTileVec.push_back(DCTMat);
	}
};

void LossyCompression::deQuantization(){
	std::vector<std::vector<RGB> > invDCTMatQERR;
	for(unsigned int l = 0; l < this->imgTileVec.size(); l++){
		invDCTMatQERR = this->tMat * (this->imgTileVec[l]/this->quantizationMat) * this->transTMat;
		this->invDCTTileVec.push_back(invDCTMatQERR);
	}
};

void LossyCompression::quantization(){
	std::vector<std::vector<RGB> > quantization;
	std::vector<std::vector<RGB> > invDCTMatQERR;
	std::vector<std::vector<RGB> > ERRMat;

	for(unsigned int l = 0; l < this->DCTTileVec.size(); l++){
		quantization = this->DCTTileVec[l]&this->quantizationMat;
		for(unsigned int i = 0; i < quantization.size(); i++){
			for(unsigned int j = 0; j < quantization[i].size(); j++){
				quantization[i][j].r = round(quantization[i][j].r);
				quantization[i][j].g = round(quantization[i][j].g);
				quantization[i][j].b = round(quantization[i][j].b);
			}
		}
		this->quantizationTileVec.push_back(quantization);
		invDCTMatQERR = this->tMat *(quantization/this->quantizationMat)* this->transTMat;
		this->invDCTTileVec.push_back(invDCTMatQERR);
		ERRMat = invDCTMatQERR;
		for(unsigned int i = 0; i < ERRMat.size(); i++){
			for(unsigned int j = 0; j < ERRMat[i].size(); j++){
				ERRMat[i][j] = ERRMat[i][j] - this->imgTileVec[l][i][j];
			}
		}
		this->ERRTileVec.push_back(ERRMat);
	}
};

void LossyCompression::setDifferenceImg(std::string filename){
	// set header
	for(unsigned int i = 0; i < 2; i++) this->differenceImg.push_back(this->img[i]);
	std::stringstream ss;
	ss << "\n#GRAY " << GRAYDIFF << '\n';
	std::string comment = ss.str();
	for(unsigned int i = 0; i < comment.size(); i++) this->differenceImg.push_back(comment.at(i));
	for(unsigned int i = 2; i < this->header.size; i++) this->differenceImg.push_back(this->img[i]);
	// set image
	for(unsigned int l = 0; l < this->imgTileVec.size();l++){
		for(unsigned int i = 0; i < this->imgTileVec[l].size();i++){
			for(unsigned int j = 0; j < this->imgTileVec[l][i].size();j++){
				this->differenceImg.push_back((unsigned char) (this->imgTileVec[l][i][j].r - (int) this->invDCTTileVec[l][i][j].r) + GRAYDIFF);
				this->differenceImg.push_back((unsigned char) (this->imgTileVec[l][i][j].g - (int) this->invDCTTileVec[l][i][j].g) + GRAYDIFF);
				this->differenceImg.push_back((unsigned char) (this->imgTileVec[l][i][j].b - (int) this->invDCTTileVec[l][i][j].b) + GRAYDIFF);
			}
		}
	}
	// set file name and write file
	filename.erase(filename.end() - filename.substr(filename.find_last_of(".") + 1).size() - 1, filename.end());
	std::string newFileName = filename + "DiffImage";
	if(this->header.magicNumber == "P6") newFileName += ".ppm";
	writeFileFromVec(newFileName, this->differenceImg);
};

void LossyCompression::calculateRMSE(){
	std::vector<RGB> tempVec;
	RGB tempColor;
	RGB temp;
	double RMSETile;
	for(unsigned int l = 0; l < this->ERRTileVec.size(); l++){
		for(unsigned int i = 0; i < this->ERRTileVec[l].size(); i++){
			tempColor.r = tempColor.g = tempColor.b = 0;
			for(unsigned int j = 0; j < this->ERRTileVec[l][i].size(); j++){
				tempColor.r += pow(this->ERRTileVec[l][j][i].r, 2);
				tempColor.g += pow(this->ERRTileVec[l][j][i].g, 2);
				tempColor.b += pow(this->ERRTileVec[l][j][i].b, 2);
			}
			tempVec.push_back(tempColor);
		}
		temp.r = temp.g = temp.b = 0;
		for(unsigned int i = 0; i < tempVec.size(); i++){
			temp.r += tempVec[i].r;
			temp.g += tempVec[i].g;
			temp.b += tempVec[i].b;
		}
		RMSETile = sqrt(((temp.r + temp.g + temp.b)/3.0)/(TMATWIDTH*TMATHEIGHT));
		this->RMSETileVec.push_back(RMSETile);
		tempVec.clear();
	}
	this->RMSEMean = 0;
	for(unsigned int i = 0; i < this->RMSETileVec.size(); i++){
		this->RMSEMean += this->RMSETileVec[i];
	}
	this->RMSEMean = (double) this->RMSEMean / this->RMSETileVec.size();
};

void LossyCompression::setCompressedImgValues(){
	for(unsigned int l = 0; l < this->quantizationTileVec.size(); l++){
		for(unsigned int i = 0; i < this->quantizationTileVec[l].size(); i++){
			for(unsigned int j = 0; j < this->quantizationTileVec[l][i].size(); j++){
				this->compressedImgValues.push_back((int) this->quantizationTileVec[l][i][j].r);
				this->compressedImgValues.push_back((int) this->quantizationTileVec[l][i][j].g);
				this->compressedImgValues.push_back((int) this->quantizationTileVec[l][i][j].b);
			}
		}
	}
	int min = 0;
	for(unsigned int i = 0; i < this->compressedImgValues.size();i++){
		if(this->compressedImgValues[i] < min) min = this->compressedImgValues[i];
	}
	this->addedValue = abs(min);
	for(unsigned int i = 0; i < this->compressedImgValues.size();i++){
		this->compressedImgValues[i] = this->compressedImgValues[i] + this->addedValue;
	}
};

void LossyCompression::setCompressedImg(std::string filename){
	for(unsigned int i = 0; i < this->header.size; i++){
		this->compressedImg.push_back(this->img[i]);
	}
	std::string scalarStr;
	std::stringstream ss;
	this->setCompressedImgValues();
	ss << this->scalar << ' ' << this->scalar2 << ' ' << this->addedValue << ' ';
	scalarStr = ss.str();
	for(unsigned int i = 0; i < scalarStr.size(); i++) this->compressedImg.push_back(scalarStr.at(i));
	for(unsigned int i = 0; i < this->compressedImgValues.size(); i++){
		this->compressedImg.push_back((unsigned char) this->compressedImgValues[i]);
	}
	filename.erase(filename.end() - filename.substr(filename.find_last_of(".") + 1).size() - 1, filename.end());
	std::string newFileName = filename + "Lossycomp" + ".lcompress";
	writeFileFromVec(newFileName, this->compressedImg);
	try{
		this->huffman = new Huffman<CharInf<unsigned char> >();
		this->huffman->compressFile(newFileName);
	} catch(...){
		throw;
	}
};

void LossyCompression::setDecompressedImg(std::string filename){
	this->setDecompressedHeader();
	for(unsigned int l = 0; l < this->invDCTTileVec.size(); l++){
		for(unsigned int i = 0; i < this->invDCTTileVec[l].size(); i++){
			for(unsigned int j = 0; j < this->invDCTTileVec[l][i].size(); j++){
				this->decompressedImg.push_back((unsigned char) invDCTTileVec[l][i][j].r);
				this->decompressedImg.push_back((unsigned char) invDCTTileVec[l][i][j].g);
				this->decompressedImg.push_back((unsigned char) invDCTTileVec[l][i][j].b);
			}
		}
	}
	
	filename.erase(filename.end() - filename.substr(filename.find_last_of(".") + 1).size() - 1, filename.end());
	if(this->header.magicNumber == "P6"){
		std::string newFileName = filename + "Decompressed" + ".ppm";
		writeFileFromVec(newFileName, this->decompressedImg);
	}
};

void LossyCompression::setDecompressedHeader(){
	std::string headerStr;
	std::stringstream ss;
	ss << this->header.magicNumber << ' ' << this->header.imgWidth << ' ' << this->header.imgHeight << ' ' << this->header.maxValue << ' ';
	headerStr = ss.str();
	for(unsigned int i = 0; i < headerStr.size(); i++) this->decompressedImg.push_back(headerStr.at(i));
};

std::string LossyCompression::compressedDataToString(){
	std::string printString;
	if(!compressed){
		std::stringstream ss;
		ss << "RMSE mean: " << this->RMSEMean << '\n' << this->huffman->encodingDataToString() << "\nQuantization Matrix:\n";
		for(unsigned int i = 0; i < this->quantizationMat.size(); i++){
			for(unsigned int j = 0; j < this->quantizationMat[i].size();j++){
				ss << this->quantizationMat[i][j].r << ' ';
				ss << this->quantizationMat[i][j].g << ' ';
				ss << this->quantizationMat[i][j].b << ' ';
			}
			ss << '\n';
		}
		printString = ss.str();
	} else {
		throw;
	}
	return printString;
};

std::string LossyCompression::decompressedDataToString(){
	std::string printString;
	if(compressed){
		std::stringstream ss;
		ss << "Your file has been decompressed successfully.\nQuantization Matrix of your compressed image was:\n";
		for(unsigned int i = 0; i < this->quantizationMat.size(); i++){
			for(unsigned int j = 0; j < this->quantizationMat[i].size();j++){
				ss << this->quantizationMat[i][j].r << ' ';
				ss << this->quantizationMat[i][j].g << ' ';
				ss << this->quantizationMat[i][j].b << ' ';
			}
			ss << '\n';
		}
		printString = ss.str();
	} else {
		throw;
	}
	return printString;
};

std::vector<unsigned char> LossyCompression::getCompressedImg(){
	return this->compressedImg;
};

double LossyCompression::getRMSEMean(){
	return this->RMSEMean;
};