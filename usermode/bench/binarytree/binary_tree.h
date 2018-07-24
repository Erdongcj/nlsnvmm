#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node{
	uint64_t data;
	struct Node* left;
	struct Node* right;
}Node;

typedef struct {
	Node* root;
}Tree;

Tree* create_tree();
int insert_data(Tree* pt, int data);
int insert(Node** pRoot, Node* prev_node, Node* pn, int flag);
Node* create_node(int data);

Node** find_data(Tree* pt, int data);
Node** find(Node** pRoot, int data);
