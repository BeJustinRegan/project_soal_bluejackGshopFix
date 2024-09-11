#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Game{
	char title[255];
	char genre[255];
	int stock;
	int height;
	Game *left, *right;
}*root;

Game *addGame(char *title, char *genre, int stock){
	Game *temp = (Game*)malloc(sizeof(Game));
	strcpy(temp->title,title);
	strcpy(temp->genre,genre);
	temp->stock =stock;
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}

int getMax(int a, int b){
	return a > b ? a : b;
}

int getHeight(Game *curr){
	if(!curr) return 0;
	return 1 + getMax(getHeight(curr->left), getHeight(curr->right));
}

int getBalance(Game *curr){
	if(!curr) return 0;
	return getHeight(curr->left) - getHeight(curr->right);
}

Game *leftRotate(Game *curr){
	Game *rightChild = curr->right;
	Game *rightChildsLeft = rightChild->left;
	
	rightChild->left = curr;
	curr->right = rightChildsLeft;
	
	rightChild->height = getHeight(rightChild);
	curr->height = getHeight(curr);
	
	return rightChild;
}

Game *rightRotate(Game *curr){
	Game *leftChild = curr->left;
	Game *leftChildsRight = leftChild->right;
	
	leftChild->right = curr;
	curr->left = leftChildsRight;
	
	leftChild->height = getHeight(leftChild);
	curr->height = getHeight(curr);
	
	return leftChild;
}

Game *rebalancing(Game *curr){
	curr->height = getHeight(curr);
	int bal = getBalance(curr);
	
	if(bal > 1 && getBalance(curr->left) >= 0){
		return rightRotate(curr);
	}
	else if(bal > 1 && getBalance(curr->left) < 0){
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	else if(bal < -1 && getBalance(curr->right) <= 0){
		return leftRotate(curr);
	}
	else if(bal < -1 && getBalance(curr->right) > 0){
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	
	return curr;
}

Game *insert(Game *root, char *title, char *genre, int stock){
	if(!root) return addGame(title, genre, stock);
	
	else if(strcmp(title, root->title) < 0){
		root->left = insert(root->left, title, genre, stock);
	}
	else if(strcmp(title, root->title) > 0){
		root->right = insert(root->right, title, genre, stock);
	}
	
	return rebalancing(root);
}

Game *search(Game *root, char *title){
	if(!root) return NULL;
	else if(strcmp(title, root->title) < 0){
		return search(root->left, title);
	}
	else if(strcmp(title,root->title) > 0){
		return search(root->right, title);
	}
	else if(strcmp(title,root->title)==0){
		return root;
	}
}

Game *del(Game *root, char *title){
	if(!root) return root;
	else if(strcmp(title,root->title) < 0){
		root->left = del(root->left, title);
	}
	else if(strcmp(title,root->title) > 0){
		root->right = del(root->right,title);
	}
	else{
		if(!root->left || !root->right){
			Game *temp = root->left ? root->left : root->right;
			
			if(!temp){
				temp = root;
				root = NULL;
			}
			
			else{
				*root = *temp;
			}
			free(temp);
		}
		
		else{
			Game *temp = root->left;
			while(temp->right) temp= temp->right;
			strcpy(root->title,temp->title);
			strcpy(root->genre,temp->genre);
			root->stock = temp->stock;
			root->left = del(root->left, temp->title);
		}
	}
	if(!root) return NULL;
	return rebalancing(root);
}

Game *update(Game *root, char *findname, char *key, int stock){
	if(strcmp(findname,root->title)==0 && strcmpi(key,"add")==0){
		root->stock += stock;
		printf("Data update successfully\n");
		return root;
	}
	else if(strcmp(findname,root->title)==0 && strcmpi(key,"remove")==0){
		root->stock -= stock;
		if(root->stock == 0){
			root = del(root,findname);
			printf("Data update successfully\n");
			printf("%s is removed from the warehouse\n",findname);
		}
		return root;
	}
	else if(strcmp(findname,root->title) < 0){
		root->left = update(root->left,findname,key,stock);
	}
	else if(strcmp(findname,root->title) > 0){
		root->right = update(root->right,findname,key,stock);
	}
	else if(!root){
		return NULL;
	}
}

void inorder(Game *root){
	if(root){
		inorder(root->left);
		printf("Game Title: %s\n",root->title);
		printf("Game Genre: %s\n",root->genre);
		printf("Game Stock: %d\n",root->stock);
		puts("=============================================================");
		inorder(root->right);
	}
}

void clear(){
	for (int i = 0; i < 30; i++){
		puts("");
	}
}

int main(){
	
	int choose;
	
	while(choose != 4){
		puts("Bluejack Gshop");
		puts("==============");
		puts("1. Insert Game");
		puts("2. View Game");
		puts("3. Update Game");
		puts("4. Exit");
		printf(">>");
		scanf("%d", &choose); getchar();
		
		switch(choose){
			case 1:{
				clear();
				char newGame[255];
				char newGenre[255];
				int stock;
				
				while(1){
					printf("Input game title[5-25][unique]: ");
					scanf("%[^\n]",newGame); getchar();
					
					if(strlen(newGame) > 4 && strlen(newGame) < 26){
						 break;
					}
				}
				
				while(1){
					printf("Input game type [Action|RPG|Adventure|Card Game]:");
					scanf("%[^\n]",newGenre); getchar();
					
					if(strcmp(newGenre,"Action")==0 || strcmp(newGenre,"RPG") == 0 || strcmp(newGenre,"Adventure")==0 || strcmp(newGenre, "Card Game")==0){
						break;
					}
					else{
						continue;
					}
				}
				
				while(1){
					printf("Input game stock [>=1]: ");
					scanf("%d",&stock); getchar();
					
					if(stock > 0){
						break;
					}
				}
				
				root = insert(root,newGame,newGenre,stock);
				printf("Insert Success!\n");
				getchar();
				clear();
				
				break;
			}
			
			case 2:{
				clear();
				inorder(root);
				getchar();
				clear();
				break;
			}
			
			case 3:{
				clear();
				int stock;
				char title[255];
				char key[255];
				
				printf("Input game title: ");
				scanf("%[^\n]",title); getchar();
				
				Game *searchItem = search(root,title);
				if(searchItem){
					printf("Game stock = %d\n",searchItem->stock);
					
					while(1){
						printf("Input update type [add|remove][case insentive]: ");
						scanf("%s",key); getchar();	
						if(strcmpi(key,"add")==0 || strcmpi(key,"remove")==0){
							break;
						}
					}
						
					if(strcmpi(key,"add")==0){
						while(1){
							printf("Input stock to add[>=1]: ");
							scanf("%d",&stock); getchar();
								
							if(stock > 0){
								break;
							}
						}
						root = update(root,title,key,stock);
					}
					
					else if(strcmpi(key,"remove")==0){
						while(1){
							printf("Input stock to remove[1-%d]: ",searchItem->stock);
							scanf("%d",&stock); getchar();
							
							if(stock > 0 && stock <=searchItem->stock){
								break;
							}
						}
						root = update(root,title,key,stock);
					}
					getchar();
					clear();			
				}
				
				else if(!searchItem){
					printf("Data not found!\n");
					getchar();
					
					clear();
				}
				
				break;
			}
		}
	}
	
}
