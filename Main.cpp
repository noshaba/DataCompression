#include "DataCompression.hpp"

int main(){

	DataCompression *dc = new DataCompression();
	std::cout << dc->getWelcomeText() << std::endl;
	try{
		dc->menu();
	} catch (const eFileNotFoundException &e){
		std::cerr << "ERROR: File not found!" << std::endl;
	} catch (const eInvalidFileFormatException &e){
		std::cerr << "ERROR: Invalid file format!" << std::endl;
	} catch (const eHeaderNotFoundException &e){
		std::cerr << "ERROR: Header not found!" << std::endl;
	} catch (const eMustBeFileToBeCompressedException &e){
		std::cerr << "ERROR: Must be file to be compressed!" << std::endl;
	} catch (const eMustBeFileToBeDecompressedException &e){
		std::cerr << "ERROR: Must be file to be decompressed!" << std::endl;
	} catch (const eMagicNumberException &e){
		std::cerr << "ERROR: Magic number must be P6!" << std::endl;
	}
	
	delete dc;
	
	return 0;
}