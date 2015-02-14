#ifndef MATRIXOPERATIONS_HPP
	#define MATRIXOPERATIONS_HPP
	
	#include <vector>
	
	/**
		matrix multiplication
		@param: mat1, mat2
		@return: new mat
	*/
	
	template <typename T> std::vector<std::vector<T> > operator*(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2){
		std::vector<std::vector<T> > result;
		if(mat1[0].size() == mat2.size()){
			for(unsigned int i = 0; i < mat1.size(); i++){
				std::vector<T> row;
				for(unsigned int j = 0; j < mat2[i].size(); j++){
					T element = mat1[0][0] - mat1[0][0];
					for(unsigned int k = 0; k < mat1[j].size();k++){
						element = element + (mat1[i][k] * mat2[k][j]);
					}
					row.push_back(element);
				}
				result.push_back(row);
			}
		} else {
			std::cerr << "Matrix dimensions must agree!" << std::endl;
		}
		return result;
	};
	
	/**
		scales a matrix
		@param: mat1, double scalar
		@return: new mat
	*/
	
	template <typename T> std::vector<std::vector<T> > operator*(std::vector<std::vector<T> > mat, double scalar){
		std::vector<std::vector<T> > result;
		for(unsigned int i = 0; i < mat.size(); i++){
			std::vector<T> row;
			for(unsigned int j = 0; j < mat[i].size(); j++){
				row.push_back(mat[i][j] * scalar);
			}
			result.push_back(row);
		}
		return result;
	};
	
	/**
		matrix multiplication (component wise)
		@param: mat1, mat2
		@return: new mat
	*/
	
	template <typename T> std::vector<std::vector<T> > operator&(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2){
		std::vector<std::vector<T> > result;
		if(mat1.size() == mat2.size() && mat1[0].size() == mat2[0].size()){
			for(unsigned int i = 0; i < mat1.size(); i++){
				std::vector<T> row;
				for(unsigned int j = 0; j < mat1[i].size(); j++){
					row.push_back(mat1[i][j] * mat2[i][j] );
				}
				result.push_back(row);
			}
		} else {
			std::cerr << "Matrix dimensions must agree!" << std::endl;
		}
		return result;
	};
	
	/**
		matrix division (component wise)
		@param: mat1, mat2
		@return: new mat
	*/
	
	template <typename T> std::vector<std::vector<T> > operator/(std::vector<std::vector<T> > mat1, std::vector<std::vector<T> > mat2){
		std::vector<std::vector<T> > result;
		if(mat1.size() == mat2.size() && mat1[0].size() == mat2[0].size()){
			for(unsigned int i = 0; i < mat1.size(); i++){
				std::vector<T> row;
				for(unsigned int j = 0; j < mat1[i].size(); j++){
					row.push_back(mat1[i][j] / mat2[i][j] );
				}
				result.push_back(row);
			}
		} else {
			std::cerr << "Matrix dimensions must agree!" << std::endl;
		}
		return result;
	};
	
#endif