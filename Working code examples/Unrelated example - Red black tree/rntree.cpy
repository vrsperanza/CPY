#include <stdio.h>
#include <stdlib.h>

typedef struct node
	int data
	char red
	struct node *left, *right

node* node_init (int data)
	node * n = (node *)malloc(sizeof(node))
	n->red = 1
	n->left = NULL
	n->right = NULL
	n->data = data
	return n

node** tree_init()
	node ** tree = (node **)malloc(sizeof(node*))
	(*tree) = NULL
	return tree

char isRed(node * n)
	return n != NULL && n->red
	
void costlyPrint (node * n, int depth, ideal)
	if n == NULL 
		return
	if depth < ideal
		costlyPrint(n->left, depth+1, ideal)
		costlyPrint(n->right, depth+1, ideal)
	else if depth == ideal
		char c = n->red == 1 ? 'r' : 'b'
		printf("%d%c ", n->data, c)

void treeCostlyPrint (node * n)
	for int i = 0; i < 15; i++
		printf("%d - ", i)
		costlyPrint(n, 0, i)
		printf("\n")

void rotateRight (node ** r)
	node * n = *r
	node * l = n->left
	n->left = l->right
	l->right = n	
	*r = l
	//printf("rotateRight %d %d\n", n->data, l->data)
	
void rotateLeft (node ** r)
	node * n = *r
	node * l = n->right	
	n->right = l->left
	l->left = n	
	*r = l
	//printf("rotateLeft %d %d\n", n->data, l->data)

void colorSwap (node ** n)
	if (*n)->left != NULL
		(*n)->left->red = 0
	if (*n)->right != NULL
		(*n)->right->red = 0
	(*n)->red = 1
	//printf("colorSwap %d %d %d\n", (*n)->data, (*n)->left->data, (*n)->right->data)
	
void tree_balance (node** n)
	if isRed((*n)->left) && isRed((*n)->right) && (isRed((*n)->right->right) || isRed((*n)->right->left) || isRed((*n)->left->right) || isRed((*n)->left->left))
		colorSwap(n)
	else if isRed((*n)->left)
		if (isRed((*n)->left->right))
			rotateLeft(&(*n)->left)
			rotateRight(n)
			(*n)->red = 0
			(*n)->right->red = 1
		else if isRed((*n)->left->left)
			rotateRight(n)
			(*n)->red = 0
			(*n)->right->red = 1
	else if isRed((*n)->right)
		if isRed((*n)->right->left)
			rotateRight(&(*n)->right)
			rotateLeft(n)
			(*n)->red = 0
			(*n)->left->red = 1
		else if isRed((*n)->right->right)
			rotateLeft(n)
			(*n)->red = 0
			(*n)->left->red = 1
	//printf("bal %d\n", (*n)->data)
		
void tree_insert (node** n, int data)
	if *n == NULL
		*n = node_init(data)
		return;

	if data < (*n)->data
		tree_insert(&(*n)->left, data)
	else
		tree_insert(&(*n)->right, data)
		
	tree_balance(n)
	return;
	
void tree_preOrder (node* n)
	if n == NULL
		return
	printf("%d ", n->data)
	tree_preOrder(n->left)
	tree_preOrder(n->right)

void tree_inOrder (node* n)
	if n == NULL
		return
	tree_inOrder(n->left)
	printf("%d ", n->data)
	tree_inOrder(n->right)
	
void tree_postOrder (node* n)
	if n == NULL
		return
	tree_postOrder(n->left)
	tree_postOrder(n->right)
	printf("%d ", n->data)


node* tree_find (node * n, int key)
	if n == NULL
		return NULL

	if key == n->data
		return n
	else if key < n->data
		return tree_find(n->left, key)
	else if key > n->data
		return tree_find(n->right, key)

int tree_sucessor (node** root, node* n, int key)
	n = tree_find(n, key)	
	if n != NULL && n->right != NULL
        return tree_min(n->right);
	
	struct node* successor = NULL;
	n = *root;
    while root != NULL
        if key < n->data
            successor = n
            n = n->left
        else if key > n->data
            n = n->right
        else
           break
	
	if successor == NULL
		return -1
    return successor->data
	
int tree_predecessor (node** root, node* n, int key)
	n = tree_find(n, key)	
	if n != NULL && n->left != NULL
        return tree_max(n->left);
	
	struct node* predecessor = NULL;
	n = *root;
    while root != NULL
        if key > n->data
            predecessor = n
            n = n->right
        else if key < n->data
            n = n->left
        else
           break
	
	if predecessor == NULL
		return -1
    return predecessor->data
	
int tree_max (node* n)
	if n->right != NULL
		return tree_max(n->right)
	else
		return n->data
	
int tree_min (node* n)
	if n->left != NULL
		return tree_min(n->left)
	else
		return n->data
	
void tree_free2 (node* n)
	if n == NULL
		return
	tree_free2(n->left)
	tree_free2(n->right)
	free(n)
	
void tree_free (node** t)
	tree_free2(*t)
	free(t)