//实现对order序(阶)的B-TREE结构基本操作的封装。
//查找：search，插入：insert，删除：remove。
//创建：create，销毁：destory，打印：print。
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <tx_api.h>
#include "btree.h"
 
//#define max(a, b) (((a) > (b)) ? (a) : (b))
#define cmp(a, b) ( ( ((a)-(b)) >= (0) ) ? (1) : (0) ) //比较a，b大小
#define DEBUG_BTREE
 


 
//insert
/***************************************************************************************
   将分裂的结点中的一半元素给新建的结点，并且将分裂结点中的中间关键字元素上移至父节点中。
   parent 是一个非满的父节点
   node 是 tree 孩子表中下标为 index 的孩子节点，且是满的，需分裂。
*******************************************************************/
void BTree_split_child(BTNode* parent, int index, BTNode* node)
{
    assert(parent && node);
    int i;
    BTNode* newNode = (BTNode*)malloc(sizeof(BTNode));
    if (!newNode) {
        printf("Error! out of memory!\n");
        exit(0);
    }
    TX_BEGIN{
        TX_ADD(newNode, sizeof(BTNode));
        BTNode* n1 = (BTNode*)TX_RW(newNode);
        BTNode* n2 = (BTNode*)TX_READ(node);
        n1->isLeaf = n2->isLeaf;
        n1->keynum = BTree_D - 1;
        for(i = 0; i < n1->keynum; ++i){
            n1->key[i] = n2->key[BTree_D + i];
            n1->key[BTree_D + i] = 0;
        }
        TX_ADD(node, sizeof(BTNode));
        BTNode* n3 = (BTNode*)TX_RW(node);
        if (!n2->isLeaf){
            for (i = 0; i < BTree_D; i++){
                n1->child[i] = n2->child[BTree_D + i];              
                n3->child[i] = n2->child[i];
                n3->child[BTree_D + i] = NULL;
            }
        }
        n3->keynum = BTree_D - 1;
        for(int i=0; i<n3->keynum; i++){
            n3->key[i] = n2->key[i];
        }
        n3->isLeaf = n2->isLeaf;

        TX_ADD(parent, sizeof(BTNode));
        BTNode* n4 = (BTNode*)TX_RW(parent);
        BTNode* np = (BTNode*)TX_READ(parent);
        for (i = np->keynum; i > index; --i) {
            n4->child[i + 1] = np->child[i];
        }
        n4->child[index + 1] = newNode;
        for (i = np->keynum - 1; i >= index; --i) {
            n4->key[i + 1] = np->key[i];
        }
        n4->key[index] = n2->key[BTree_D - 1];
        n4->keynum = np->keynum + 1;
        n3->key[BTree_D - 1] = 0;
    }TX_END
}

void BTree_insert_nonfull(BTNode* node, KeyType key)
{
    assert(node);
    int i;
    BTNode* nd = (BTNode*)TX_READ(node);
    if (nd->isLeaf){
        TX_BEGIN{
            i = nd->keynum - 1;
            TX_ADD(node, sizeof(BTNode));
            BTNode* n1 = (BTNode*)TX_RW(node);
            while (i >= 0 && key < nd->key[i]){
                n1->key[i + 1] = nd->key[i];
                --i;
            }
            n1->key[i + 1] = key;
            n1->keynum = nd->keynum + 1;
            n1->isLeaf = nd->isLeaf;
        }TX_END   
    }else {
        i = nd->keynum - 1;
        while (i >= 0 && key < nd->key[i]) {
            --i;
        }
        ++i;
        if (nd->child[i]->keynum == (ORDER-1)){
            BTree_split_child(node, i, nd->child[i]);
            printf("node%lx\n", nd->child[i]);
			if (key > nd->key[i]) {
                ++i;
            }
        }
		printf("i is .................%d\n", i);
        BTree_insert_nonfull(nd->child[i], key);
    }

}


void btree_map_insert(BTree* tree, KeyType key){
    BTNode* node;
    TX_BEGIN{
        BTree* bt = (BTree*)TX_READ(tree);
        BTNode* root = bt->root;
        if (NULL == root){
            root = (BTNode*)malloc(sizeof(BTNode));
            if (!root) {
                printf("Error! out of memory!\n");
                exit(0);
            }
//            TX_BEGIN{
            TX_ADD(root, sizeof(BTNode));
            BTNode* r = (BTNode*)TX_RW(root);
            r->isLeaf = true;
            r->keynum = 1;
            r->key[0] = key;
			for(int i=1, i<ORDER-1; i++){
				r->key[i] = 0;
			}
			for(int i=0; i<ORDER; i++){
				r->child[i] = NULL;
			}
            TX_ADD(tree, sizeof(BTree));
            ((BTree*)TX_RW(tree))->root = r;
  //          }TX_END
//            ret = 1 ;
        }else{
            BTNode* rd = (BTNode*)TX_READ(root);
            if(rd->keynum == (ORDER-1)){
                node = (BTNode*)malloc(sizeof(BTNode));
                if (!node) {
                    printf("Error! out of memory!\n");
                    exit(0);
                }
            	TX_BEGIN{
                	TX_ADD(node, sizeof(BTNode));
                	BTNode* n = (BTNode*)TX_RW(node);
                	n->isLeaf = false;
                	n->keynum = 0;
					for(int i=0, i<ORDER-1; i++){
					   n->key[i] = 0;
					}

                	n->child[0] = root;
					for(int i=1; i<ORDER; i++){
						n->child[i] = NULL;
					}

                	TX_ADD(tree, sizeof(BTree));
                	BTree* T = (BTree*)TX_RW(tree);
                	T->root = n;
            	}TX_END
                	BTree_split_child(node, 0, root);
                	BTree_insert_nonfull(node, key);
            }else {
                BTree_insert_nonfull(root, key);
//        	ret = 1;
			}
		}

    }TX_END
//    return ret;
}

//=====================================search====================================
BTNode* BTree_recursive_search(BTNode* node, KeyType key, int* pos)
{
    int i = 0;
    BTNode* N = (BTNode*)TX_RO(node);
    if(N == NULL){
 //       printf("BTree is NULL!\n");
        return NULL;
    }
    while (i < N->keynum && key > N->key[i]) {
        ++i;
    }
    if (i < N->keynum && N->key[i] == key) {
        *pos = i;
        return N;
    }
    if (N->isLeaf) {
        return NULL;
    }
    return BTree_recursive_search(N->child[i], key, pos);
}


BTNode* btree_map_get(BTree* tree, KeyType key, int* pos){
    BTNode* ret;
    TX_BEGIN{
        BTree* T = (BTree*)TX_READ(tree);
        if(T == NULL){
            printf("BTree is NULL!\n");
            return NULL;
        }
        *pos = -1;
        ret = BTree_recursive_search(T->root, key, pos);
    }TX_END
    return ret;
}

BTree* btree_map_create()
{
    BTree *map = (BTree*)malloc(sizeof(BTree));
    TX_BEGIN{
        TX_ADD(map, sizeof(BTree));
        ((BTree*)TX_RW(map))->root = NULL;     
    }TX_END
    return map;
}

