#ifndef TREENODE_HPP
	#define TREENODE_HPP

	template <typename T> class TreeNode {

	private:
		T data;
		TreeNode* left;
		TreeNode* right;

	public:
	
		TreeNode(){};
	
		TreeNode(T data){
			this->data = data;
			this->left = this->right = NULL;
		};

		~TreeNode(){
			if(this->left != NULL) delete this->left;
			if(this->right != NULL) delete this->right;
		};

		/**
			returns the data of a tree node
		*/
		T getData(){
			return this->data;
		};
		
		/**
			sets the data of a tree node
		*/
		
		void setData(T data){
			this->data = data;
		};
		
		/**
			gets the left node of the current node
		*/

		TreeNode* getLeft(){
			return this->left;
		};
		
		/**
			gets the right node of the current node
		*/

		TreeNode* getRight(){
			return this->right;
		};
		
		/**
			sets the left node of the current node
			@param: left node
		*/

		void setLeft(TreeNode* left){
			this->left = left;
		};
		
		/**
			sets the right node of the current node
			@param: right node
		*/

		void setRight(TreeNode* right){
			this->right = right;
		};
		
		/**
			iterates through the tree and returns the data of the last nodes
			@return: data
		*/
		
		T iterThroughTree(){
			if(this != NULL){
				if(this->left == NULL && this->right == NULL){
					return this->data;
				}
				
				this->left->iterThroughTree();
				this->right->iterThroughTree();
			}
		};
	};

#endif
