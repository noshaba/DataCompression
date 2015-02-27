#include "DataCompression.hpp"

int main(){

	DataCompression *dc = new DataCompression();
	std::cout << dc->getWelcomeText() << std::endl;
	
	while(1){
		try{
			dc->menu();
		} catch (std::exception &e){
			std::cout << e.what() << std::endl;
		}
	}
	
	delete dc;
	
	return 0;
}