#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"


/* definition of the element in the tree */
typedef struct _ELEMENT
{
  pNode obj;
  PELEMENT* children;
  PELEMENT parent;
  int childrenCount;
} ELEMENT;

/* definition of the tree structure */    
typedef struct _tree{
  PELEMENT head;
  int k; //number of maximum children in the tree.
  int nodes_num; //number of current nodes in the tree.
  /*function pointers*/
  GetKeyFunction GET_KEY;
  CloneFunction CLONE_NODE;
  PrintFunction PRINT_NODE;
  DelFunction DEL_NODE;
  /* *** complete the definition of the tree structure *** */

} Tree;

/* Global Variable of the Tree Pointer !!!*/
pTree pTreeSquare = NULL;

/* *** complete the interface functions implementation *** */
pTree TreeCreate(GetKeyFunction GET_KEY, CloneFunction CLONE_NODE, PrintFunction PRINT_NODE, DelFunction DEL_NODE, int k)
{
	pTree ptree;
	ptree = (pTree)malloc(sizeof(Tree));
	if (ptree)   //if malloc succeded
	{
		ptree->head = (PELEMENT)malloc(sizeof(ELEMENT)); //allocate memory for the first element
		ptree->k = k; //the number of maximum children
		ptree->nodes_num = 0;
		ptree->GET_KEY = GET_KEY;
		ptree->CLONE_NODE = CLONE_NODE;
		ptree->PRINT_NODE = PRINT_NODE;
		ptree->DEL_NODE = DEL_NODE;
	}
	return ptree;
}
static void Internal_Del_Helper(PELEMENT pElement,pTree ptree)
{
	if (pElement == NULL) return;
	for (int j = 0;j < (ptree->k);j++)
	{
		
		Internal_Del_Helper(pElement->children[j], ptree);
	}
	if (pElement->childrenCount == 0)
	{
		ptree->DEL_NODE(pElement->obj);
		free(pElement);
		return;
	}
	
}

void TreeDestroy(pTree ptree)
{
	if (ptree->nodes_num == 0)
	{
		free(ptree);
		return;
	}
	for (int i = 0;i < (ptree->k);i++)
	{
		Internal_Del_Helper(ptree->head->children[i],ptree);
	}
	ptree->DEL_NODE(ptree->head->obj);
	free(ptree->head);
	free(ptree);
}

int TreeNodesCount(pTree ptree)   //Not sure if it's better to implement it this way...!
{
	return (ptree->nodes_num);
}
static void Internal_Print_Helper(PELEMENT pElement,pTree ptree)
{
	if (pElement == NULL) return;
	ptree->PRINT_NODE(pElement->obj);
	for (int j = 0;j < (ptree->k);j++)
	{
		Internal_Print_Helper(pElement->children[j], ptree);
	}
	
}
void TreePrint(pTree ptree)
{
	if ((ptree->nodes_num) == 0) return;
	ptree->PRINT_NODE(ptree->head->obj); //print the first node!
	for (int i = 0;i < (ptree->k);i++)
	{
		Internal_Print_Helper(ptree->head->children[i],ptree);
	}
}
static void Internal_element_finder_helper(pTree ptree, int nodekey, PELEMENT pElement_KEY,PELEMENT pElement)
{
	if (pElement == NULL) return;
	if (ptree->GET_KEY((pElement->obj)) == nodekey)
	{
		pElement_KEY = pElement;
	}
	else
	{
		for (int j = 0; j < (ptree->k); j++)
		{
			Internal_element_finder_helper(ptree, nodekey, pElement_KEY, (pElement->children)[j]);
		}
	}
}
static void Internal_element_finder(pTree ptree, int nodekey,PELEMENT pElement_KEY)
{
	if ((ptree->nodes_num) == 0)
	{
		pElement_KEY = NULL;
	}
	else if ((ptree->GET_KEY((ptree->head->obj))) == nodekey)
	{
		pElement_KEY = ptree->head;
	}
	else
	{
		for (int i = 0; i < (ptree->k); i++)
		{
			Internal_element_finder_helper(ptree,nodekey,pElement_KEY,ptree->head->children[i]);
		}
	}
}
Result TreeAddLeaf(pTree ptree, int nodekey, pNode pnode)      //What about the key of the node for now we assume it's coming with the node pointer 
{
	PELEMENT pElement_new = NULL;
	PELEMENT pElement_KEY=NULL;
	// Case the tree is empty
	if (ptree->nodes_num == 0)
	{
		//the pElement_KEY will get the adress of the head Element which we allocated in the "CreateTree" Function!!!
		pElement_KEY = (ptree->head);
		//Need to allocate memory for the array of the pointers because it's a dynamic array!!! !! how we can know 
		pElement_KEY->children = (PELEMENT*)malloc((ptree->k)*sizeof(PELEMENT));
		if (pElement_KEY) //Check if malloc succeded!!!
		{
			ptree->nodes_num = 1; //change the number of nodes to 1!
			(pElement_KEY->obj) = (ptree->CLONE_NODE(pnode));
			pElement_KEY->parent = NULL;
			pElement_KEY->childrenCount = 0;
			//checking the malloc of the children was a success!
				if (pElement_KEY->children)
				{
					//Reset the children array of pElements
					for (int i = 0; i < (ptree->k); i++)
					{
						(pElement_KEY->children)[i] = NULL; //There is a warning i'm not sure about for now!
					}
			}
		}
	}
	else
	//Case the tree is not empty
	{
		//First lets find the pointer to the Element with the desired key!
		Internal_element_finder(ptree, nodekey, pElement_KEY);
		if (pElement_KEY == NULL)
		{
			return FAILURE;
		}
		else
		{
			//No place for another "child"
			if (pElement_KEY->childrenCount == (ptree->k))
			{
				return FAILURE;
			}
			for (int j=0;j<(ptree->k);j++)
			{
				if (pElement_KEY->children[j] != NULL)
				{
					pElement_new = (PELEMENT)malloc(sizeof(ELEMENT));
					pElement_new->obj = (ptree->CLONE_NODE(pnode));
					pElement_new->parent = pElement_KEY;
					pElement_new->childrenCount = 0;
					pElement_KEY->childrenCount = pElement_KEY->childrenCount + 1;
					pElement_KEY->children[j] = pElement_new;
					//First Allocat memory for the children of the new Element
					pElement_new->children = (PELEMENT*)malloc((ptree->k) * sizeof(PELEMENT));
					//reset children array
					for (int m = 0; m < (ptree->k); m++)
					{
						(pElement_new->children)[m] = NULL;
					}
					return SUCCESS;
				}
			}
		}
	}

}
Result TreeNodeIsActive(pTree ptree, int nodekey, Bool* pResult )
{
	PELEMENT pElement_KEY=NULL;
	
	Internal_element_finder(ptree, nodekey, pElement_KEY);
	if (pElement_KEY == NULL)
	{
		return FAILURE;
	}
	else
	{
		if ((pElement_KEY->childrenCount) < (ptree->k))
		{
			*pResult = TRUE;
		}
		else
		{
			*pResult = FALSE;
		}
		return SUCCESS;
	}

}
Result TreeNodeIsLeaf(pTree ptree, int nodekey, Bool* pResult)
{
	PELEMENT pElement_KEY=NULL;
	Internal_element_finder(ptree, nodekey, pElement_KEY);
	if (pElement_KEY == NULL)
	{
		return FAILURE;
	}
	else
	{
		if ((pElement_KEY->childrenCount) == 0)
		{
			*pResult = TRUE;
		}
		else
		{
			*pResult = FALSE;
		}
		return SUCCESS;
	}

}
Result TreeDelLeaf(pTree ptree, int nodekey) //Not clear instruction if delete the element struct as well or not !!! 
{
	PELEMENT pElement_KEY=NULL;
	Internal_element_finder(ptree, nodekey, pElement_KEY);
	if (pElement_KEY == NULL)
	{
		return FAILURE;
	}
	else
	{
		//case it's a leaf
		if ((pElement_KEY->childrenCount) == 0)
		{
			ptree->DEL_NODE(pElement_KEY->obj); //Delete node obj
			pElement_KEY->parent->childrenCount = pElement_KEY->parent->childrenCount - 1;
			/*Set the childrent array of the element to NULL in the parent junction */
			for (int i = 0; i < (ptree->k); i++)
			{
				if (pElement_KEY->parent->children[i] == pElement_KEY)
				{
					pElement_KEY->parent->children[i] = NULL;
					break;
				}
			}
			/*         */
			free(pElement_KEY);

		}
	}
} 

pNode TreeGetRoot(pTree ptree)
{
	//if tree is empty
	if (ptree->head == NULL)
	{
		return NULL;
	}
	else
		return (ptree->CLONE_NODE(ptree->head->obj));
}

pNode TreeGetNode(pTree ptree, int nodekey)
{
	pNode temp_pNode;
	PELEMENT pElement_KEY=NULL;
	Internal_element_finder(ptree, nodekey, pElement_KEY);
	if (pElement_KEY == NULL)
	{
		return NULL;
	}
	else
	{
		temp_pNode = pElement_KEY->obj;
		return (ptree->CLONE_NODE(temp_pNode));
	}
}

pNode* TreeGetChildren(pTree ptree, int nodekey)   //!!!!!No consideration in the case that there is no junction with that key !!!!!
{
	pNode* nodes_array;
	PELEMENT pElement_KEY = NULL;
	Internal_element_finder(ptree, nodekey, pElement_KEY);
	if (pElement_KEY)
	{
		nodes_array = (pNode*)malloc((ptree->k) * sizeof(pNode));
		if (nodes_array)
		{
			for (int i = 0; i < (ptree->k); i++)
			{
				if (((pElement_KEY->children)[i]) == NULL)
				{
					nodes_array[i] = NULL;
				}
				else
				{
					nodes_array[i] = (((pElement_KEY->children)[i])->obj);
				}
			}
			return (nodes_array);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}




