#include <stdio.h>
#include <stdlib.h>
#include <tx_api.h>
#include <wrap.hh>
#include "binary_tree.h"


Tree* create_tree()
{
	Tree* tree = (Tree*)malloc(sizeof(Tree));
//	Tree* tree = (Tree*)pemalloc(sizeof(Tree));
	TX_BEGIN{
		TX_ADD(tree, sizeof(Tree));
		((Tree*)TX_RW(tree))->root = NULL;
	}TX_END
	return tree;
}

Node** find(Node** pRoot, int data)
{
	if(*pRoot == NULL)
	{
		return pRoot;
	}
	
	Node* root = (Node*)TX_RO(*pRoot);
	if(root != NULL){
		if(root->data == data){
			return pRoot;
		}else if(data < root->data){
			return find(&root->left, data);
		}else {
			return find(&root->right, data);
		}
	}else{
//		tx_rollback();
		return NULL;
	}
	

}


Node** find_data(Tree* pt, int data)
{
	Node** n;
//	Tree* tree = (Tree*)TX_GET(pt);
//	printf("tree in the find is %lx\n ", tree);
	TX_BEGIN{
		Tree* tree = (Tree*)TX_RO(pt);
		if(tree == NULL){
//			tx_rollback();
			return 0;
		}else{
			n = find(&tree->root, data);
		}
	}TX_END

	return n;
}


Node* create_node(int data)
{
//	struct timeval begin1, end1;
//	float diff1;
//	gettimeofday(&begin1, NULL);
	Node* pn = (Node*)malloc(sizeof(Node));
//	gettimeofday(&end1, NULL);
//	diff1 = (end1.tv_sec - begin1.tv_sec)*1000000 + (end1.tv_usec - begin1.tv_usec);
//	printf(" times diff........................................................................: %f\n", diff1);
	TX_ADD(pn, sizeof(Node));
	Node* p = (Node*)TX_RW(pn);
	if(p != NULL){
		p->data = data;
		p->left = NULL;
		p->right = NULL;
		return pn;
	}else{
//		tx_rollback();
		return NULL;
	}
}

int insert(Node** pRoot, Node* prev_node, Node* pn, int flag)
{
	if(*pRoot == NULL){
		TX_ADD(prev_node, sizeof(Node));
		Node* nd = (Node*)TX_RW(prev_node);
		Node* p_node = (Node*)TX_READ(prev_node);
		if(nd != NULL && p_node != NULL){
			if(flag == 0){
				nd->left = pn;
				nd->right = p_node->right;
				nd->data = p_node->data;
			}else{
				nd->left = p_node->left;
				nd->right = pn;
				nd->data = p_node->data;
			}
			return 1;
		}else{
			printf("cuowu\n");
			return 0;
		}
	}
	Node* root = (Node*)TX_RO(*pRoot);
	Node* p = (Node*)TX_RW(pn);
	if(root != NULL && p != NULL){
		if(root->data == p->data){
			return 1;
		}else if(root->data > p->data){
			insert(&root->left, *pRoot, pn, 0);
		}else{
			insert(&root->right, *pRoot, pn, 1);
		}
	}else{
//		tx_rollback();
		return 0;
	}
	return 1;


}

int insert_data(Tree* pt, int data)
{
	int n;
//	Tree* tree = (Tree*)TX_GET(pt);
	TX_BEGIN{
		Tree* tree = (Tree*)TX_RO(pt);
		if(tree != NULL){
			Node* nd = create_node(data);
			if(tree->root == NULL)
			{
				TX_ADD(pt, sizeof(Tree));
				Tree* t = (Tree*)TX_RW(pt);
				t->root = nd;
				n = 1;
			}else{
				n = insert(&tree->root, NULL, nd, 0);
			}
			if(n == 0){
//				tx_rollback();
				return 0;
			}
		}else{
//			tx_rollback();
			return 0;
		}
	}TX_END
	return n;

}
