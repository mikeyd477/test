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
	else
	{
		return NULL;
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

static Bool Square_Range_Helper(pNode CheckedSquare ,pNode FatherSquare,double x,double y)
{
	PSQUARE pCurrentSquare;
	PSQUARE pFatherSquare;
	//Check if the point is in the range of current square
	pCurrentSquare = (PSQUARE)CheckedSquare;
	float x_left_son = (pCurrentSquare->x_left);
	float x_right_son = (pCurrentSquare->x_right);
	float y_left_son = (pCurrentSquare->y_left);
	float y_right_son = (pCurrentSquare->y_right);
	//basic range check without extreme conditions
	if (!(x_left_son <= x && x <= x_right_son && y >= y_left_son && y <= y_right_son))
	{
		return FALSE;
	}
	else if (x_left_son <= x && x < x_right_son && y >= y_left_son && y < y_right_son)
	{
		return TRUE;
	}
	else
	{
		//Not Root Square case
		if (FatherSquare != NULL)
		{
			pFatherSquare = (PSQUARE)FatherSquare;
			float x_left_father = (pFatherSquare->x_left);
			float x_right_father = (pFatherSquare->x_right);
			float y_left_father = (pFatherSquare->y_left);
			float y_right_father = (pFatherSquare->y_right);

			if (x_right_son == x_right_father && y_right_son == y_right_father)
			{
				return TRUE;
			}
			else if (x_right_son == x_right_father)
			{
				return TRUE;
			}
			else if (y_right_son == y_right_father)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		//Root Square case
		else
		{
			if (x < 0 || x>1 || y > 1 || y < 0)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
	}

}

static pNode RefineCell_Helper(double x, double y, pNode CurrentNode, pNode FatherNode)
{
	//array of pNode pointers
	pNode* children_nodes;
	//Check if the point is in the range of father square
	if (Square_Range_Helper(CurrentNode, FatherNode, x, y) == FALSE)
	{
		return NULL;
	}
	//Case we are in the square range
	else
	{
		children_nodes = TreeGetChildren(pTreeSquare, SquareGetKeyFunction(CurrentNode));
		//check if the square has children case like when the loop is finishing so it's a waste of code!!!
					/* Mike: This is the same proce
							int flag_children_empty = 0;
							for (int i = 0; i < 4; i++)
							{
								if (children_nodes != NULL)
								{
									flag_children_empty = 1;
								}
							}
							if (flag_children_empty == 1)
							{
								return CurrentNode;
							}
							else
							{
					*/	
		for (int i = 0; i < 4; i++)
		{
			if (children_nodes[i] != NULL)
			{
				if (Square_Range_Helper(children_nodes[i], CurrentNode, x, y) == TRUE)
				{
					RefineCell_Helper(x, y, children_nodes[i], CurrentNode);
					break;
				}
			}

		}
		return CurrentNode;
	}
}

static Bool New_Square_Check(double x, double y, float x_left_father, float x_right_father, float y_left_father, float y_right_father,
	float x_left_son, float x_right_son, float y_left_son, float y_right_son)
{
	//basic range check without extreme conditions
	if (!(x_left_son <= x && x <= x_right_son && y >= y_left_son && y <= y_right_son))
	{
		return FALSE;
	}
	else if (x_left_son <= x && x < x_right_son && y >= y_left_son && y < y_right_son)
	{
		return TRUE;
	}
	else
	{	
		if (x_right_son == x_right_father && y_right_son == y_right_father)
		{
			return TRUE;
		}
		else if (x_right_son == x_right_father)
		{
			return TRUE;
		}
		else if (y_right_son == y_right_father)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}
static void New_Square_Create(double x, double y, pNode Father)
{
	PSQUARE pSquare;
	pNode pNew_Node;
	PSQUARE Father_Square;
	Father_Square = (PSQUARE)Father;
	//Get Father Coordinates
	float x_left_father = Father_Square->x_left;
	float x_right_father = Father_Square->x_right;
	float y_left_father = Father_Square->y_left;
	float y_right_father = Father_Square->y_right;
	//Coordinates Calculation
	float x_res_ref = ((x_right_father - x_left_father) / 2) + x_left_father;
	float y_res_ref = ((y_right_father - y_left_father) / 2) + y_left_father;
	//Add new Square
	pSquare = (PSQUARE)malloc(sizeof(SQUARE));
	if (pSquare != NULL) {
		pSquare->key_node = TreeNodesCount(pTreeSquare) + 1;
		//cases of different squares:
		//left upper square:
		if (New_Square_Check(x, y, x_left_father, x_right_father, y_left_father, y_right_father,x_left_father,x_res_ref,y_res_ref,y_right_father))
		{
				pSquare->x_left = x_left_father ;
				pSquare->x_right = x_res_ref ;
				pSquare->y_left = y_res_ref;
				pSquare->y_right = y_right_father;
		}
		//left bottom square:
		else if (New_Square_Check(x, y, x_left_father, x_right_father, y_left_father, y_right_father,x_left_father,x_res_ref,y_left_father,y_res_ref))
		{
				pSquare->x_left = x_left_father;
				pSquare->x_right = x_res_ref;
				pSquare->y_left = y_left_father;
				pSquare->y_right = y_res_ref;
		}
		//right upper square:
		else if (New_Square_Check(x, y, x_left_father, x_right_father, y_left_father, y_right_father,x_res_ref,x_right_father,y_res_ref,y_right_father))
		{
			pSquare->x_left = x_res_ref;
			pSquare->x_right = x_right_father;
			pSquare->y_left = y_res_ref;
			pSquare->y_right = y_right_father;
		}
		//right bottom square:
		else if (New_Square_Check(x, y, x_left_father, x_right_father, y_left_father, y_right_father,x_res_ref,x_right_father,y_left_father,y_res_ref))
		{
			pSquare->x_left = x_res_ref;
			pSquare->x_right = x_right_father;
			pSquare->y_left = y_left_father;
			pSquare->y_right = y_res_ref;
		}
		//Casting the pSquare to void* to use the clonefunction
		pNew_Node = (void*)pSquare;
		//Add Leaf Function!!!
		TreeAddLeaf(pTreeSquare, SquareGetKeyFunction(Father), pNew_Node);
	}

}

void RefineCell(double x, double y)
{
	pNode pRoot;
	pNode pFather;
	//Check if the point is in our range or not what to do about it we need to check!
	if (x > 1 || x < 0 || y>1 || y < 0) {
		return;
	}
	else {
		pRoot = TreeGetRoot(pTreeSquare);
		pFather = RefineCell_Helper(x, y, pRoot, NULL);
		//Build the first node in the partition
		New_Square_Create(x, y, pFather);
	}
}
void PrintPartition()
{
	TreePrint(pTreeSquare);
}
