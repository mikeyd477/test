#ifndef TREE_H
#define TREE_H

#include "defs.h"
/* complete the definition of the pointer to Tree */
typedef struct _ELEMENT* PELEMENT; //definition of the pointer to ELEMENT
typedef struct _tree* pTree; //definition of the pointer to Tree
typedef void* pNode;
/* Global Variable of the Tree Pointer !!!*/
extern pTree pTreeSquare;
/* Function types that the user will supply to the ADT*/
typedef int (*GetKeyFunction)(pNode e);
typedef pNode (*CloneFunction)(pNode e);
typedef void (*PrintFunction)(pNode e);
typedef void (*DelFunction)(pNode e);

/* complete the definition of the interface functions */
pTree TreeCreate(GetKeyFunction, CloneFunction, PrintFunction, DelFunction, int); //Function to Create the Tree structure
int TreeNodesCount(pTree); //Function that return the number of nodes in the tree 
void TreePrint(pTree); //Function that will print all the nodes in the Tree
void TreeDestroy(pTree); //Function that will free all the tree structure a bit complex need to verify.
Result TreeAddLeaf(pTree, int, pNode);
Result TreeNodeIsActive(pTree,int,Bool*);
Result TreeNodeIsLeaf(pTree, int, Bool*);
Result TreeDelLeaf(pTree ptree, int nodekey);
pNode TreeGetRoot(pTree);
pNode TreeGetNode(pTree,int);
pNode* TreeGetChildren(pTree,int);
#endif
