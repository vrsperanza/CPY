#include "rntree.h"

int main()
	//scanf("%*[^\n]")
	
	int o, x;
	node ** root = tree_init()
	while scanf("%d", &o) != EOF
		if o == 1
			scanf("%d", &x)
			//printf("INSERT %d\n", x)
			tree_insert(root, x)
			(*root)->red = 0
		else if (o == 2)
			scanf("%d", &x)
			//printf("tree_sucessor %d\n", x)
			int sucessor = tree_sucessor(root, *root, x);
			if sucessor != -1
				printf("%d\n", sucessor)
			else
				printf("erro\n")
		else if o == 3
			scanf("%d", &x)
			//printf("tree_predecessor %d\n", x)
			int predecessor = tree_predecessor(root, *root, x);
			if predecessor != -1
				printf("%d\n", predecessor)
			else
				printf("erro\n")
		else if o == 4
			//printf("tree_max %d\n", x)
			printf("%d\n", tree_max(*root))
		else if o == 5
			//printf("tree_min %d\n", x)
			printf("%d\n", tree_min(*root))
		else if o == 6
			//printf("tree_preOrder %d\n", x)
			tree_preOrder(*root)
			printf("\n")
		else if o == 7
			//printf("tree_inOrder %d\n", x)
			tree_inOrder(*root)
			printf("\n")
		else if o == 8
			//printf("tree_postOrder %d\n", x)
			tree_postOrder(*root)
			printf("\n")
		else if o == 9
			//printf("treeCostlyPrint %d\n", x)
			treeCostlyPrint(*root)
			printf("\n")
		else
			printf("%d is not a valid option\n", o)
	tree_free(root)