#include "gentree.h"
#include "partition.h"
#include <stdio.h>
#include <stdlib.h>

/*Structure we will use for a node! */

typedef struct SQUARE {
	int key_node;
	float x_left;
	float x_right;
	float y_left;
	float y_right;
} SQUARE, *PSQUARE;

/*Supplied Functions to Tree ADT !!!*/

int SquareGetKeyFunction(pNode pnode)
{
	PSQUARE pSquare = (PSQUARE)pnode;
	return (pSquare->key_node);
}
void SquareDelFunction(pNode pnode)
{
	PSQUARE pSquare = (PSQUARE)pnode;
	free(pSquare);
}
void SquarePrintFunction(pNode pnode)
{
	PSQUARE pSquare = (PSQUARE)pnode;
	printf("([%f, %f], [%f, %f])", pSquare->x_left, pSquare->x_right, pSquare->y_left, pSquare->y_right);
}
//I don't understand well enough the usage of the clone function!!!
pNode SquareCloneFunction(pNode pnode)  
{
	PSQUARE pSquare = (PSQUARE)malloc(sizeof(SQUARE));
	PSQUARE pSquare_Orig = (PSQUARE)pnode;
	if (pSquare)
	{
		*pSquare = *pSquare_Orig;
		void* CloneResult = (void*)pSquare;
		return CloneResult;
	}
} 

/*---------------------------------------------*/

/*Partition Functions!!!!  */
void InitPartition()  //The function need to first destroy the tree before making new one ?
{
	pNode pnode;
	PSQUARE pSquare;
	//Generate the tree
	pTreeSquare = TreeCreate(SquareGetKeyFunction, SquareCloneFunction, SquarePrintFunction, SquareDelFunction, 4);
	//DBG
	if (pTreeSquare) {
		printf("Success in TreeCreate");
	}
	else
	{
		printf("failure in TreeCreate");
	}
	//Build the first node in the partition
	pSquare = (PSQUARE)malloc(sizeof(SQUARE));
	if (pSquare != NULL) {
		pSquare->key_node = 1;
		pSquare->x_left = 0;
		pSquare->x_right = 1;
		pSquare->y_left = 0;
		pSquare->y_right = 1;
		//Casting the pSquare to void* to use the clonefunction
		pnode = (void*)pSquare;
		//Set parameters for the first Element in the Partition
		TreeAddLeaf(pTreeSquare, 0, pnode);
		//Test
		TreePrint(pTreeSquare);
	}
}

