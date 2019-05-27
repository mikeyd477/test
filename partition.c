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

void DeletePartition()
{
	//Destroy the Partition we have so far
	TreeDestroy(pTreeSquare);
}

void InitPartition()  //The function need to first destroy the tree before making new one ?
{
	pNode pnode;
	PSQUARE pSquare;
	//First check if the tree pointer  is not NULL
	if (pTreeSquare != NULL)
	{
		DeletePartition(pTreeSquare);
	}
	//Generate the tree
	pTreeSquare = TreeCreate(SquareGetKeyFunction, SquareCloneFunction, SquarePrintFunction, SquareDelFunction, 4);
	//DBG
	/*
				if (pTreeSquare) {
					printf("Success in TreeCreate");
				}
				else
				{
					printf("failure in TreeCreate");
				}
	*/
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
	}
}

static void RefineCell_Helper(float x_left,float x_right,float y_left,float y_right,double x,double y,int nodes_number,int division_number,PELEMENT child)
{

	//Check Left Upper Square 
	if (x<(x_right/2) & (y>(y_right/2)))
	{

	}

	//check Left Bottom Square
	if (x<(x_right/2) & (y<(y_right/2)))
	{

	}
	//check Right Upper Square
	if ((x>(x_right/2)) & (y>(y_right/2)))
	{

	}
	//check Right Bottom Square
	if ((x>(x_right/2)) & (y<(y_right/2)))
	{

	}

}





//AR: Check about the extreme situation when the point is on the edge of one of the squares!!!
//AR: No check if there is a square created or not !!
void RefineCell(double x, double y)
{
	pNode pnode;
	int key_node;
	PELEMENT* children_array;
	PSQUARE pSquare;
	int nodes_number;
	//get the root node
	pnode = TreeGetRoot(pTreeSquare);
	//get node key
	key_node = SquareGetKeyFunction(pnode);
	//get children array
	children_array = TreeGetChildren(pTreeSquare, key_node);
	//get_nodes_number for node key for the new node!!
	nodes_number = TreeNodesCount(pTreeSquare);
	//Left Bottom Square
	if ((x < 0.5)& (y < 0.5))
	{
		//Check if this square exists
		if (children_array[1] == NULL)
		{

		}

	}
	//Left Upper Square
	if ((x < 0.5) & (y > 0.5))
	{
		//check if this square exists
		if (children_array[0] == NULL)
		{
			//No Square we create a new square
			//Build the node in the partition
			pSquare = (PSQUARE)malloc(sizeof(SQUARE));
			if (pSquare != NULL) {
				pSquare->key_node = nodes_number+1;
				pSquare->x_left = 0;
				pSquare->x_right = 0.5;
				pSquare->y_left = 0.5;
				pSquare->y_right = 1;
				//Casting the pSquare to void* to use the clonefunction
				pnode = (void*)pSquare;
				//Set parameters for the first Element in the Partition
				TreeAddLeaf(pTreeSquare, 1, pnode);
			}
		}
		//if the node exists!! we go to our recursion function !! :)
		else
		{
			RefineCell_Helper(0, 0.5, 0.5, 1,x,y,nodes_number,2, children_array[0]);
		}
	}
	//Right Upper Square
	if ((x > 0.5) & (y > 0.5))
	{

	}
	//Right Bottom Square
	if ((x > 0.5) & (y < 0.5))
	{

	}


}

