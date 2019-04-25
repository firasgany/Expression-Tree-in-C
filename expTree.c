#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "expTree.h"

/* definition of a node in the tree */
typedef struct _node
{
	pElement	elem;

	pNode		leftChild;
	pNode		rightChild;
} Node;

/* definition of the tree structure */
typedef struct _tree
{
	pNode root;


	/*****   Interface Functions pointers    *****/
	CloneFunction      CloneFunction;
	DelFunction        DelFunction;
	OperateFunction    OperateFunction;
	GetKeyFunction     GetKeyFunction;
	CompareKeyFunction CompareKeyFunction;


	/* *** complete the definition of the tree structure *** */
} Tree;

/* *** complete the interface functions implementation *** */
/*Implemention of private functions (only for the use of ADT makers)*/

/*
Function:makenode(function name).
Inputs:string to the function name that we called this function from(in order to print the function name in printf).
Action:allocates a node structure, checks malloc, initialize all fields of the struct created to NULL.
returns:if SUCCESS:pointer to the new empty node struct allocated. else exists with returning FAILURE.
*/
pNode makenode() {
	pNode pNewNode = (pNode)malloc(sizeof(*pNewNode));
	/*Checking malloc success*/
	if (pNewNode == NULL) {
		printf("Error allocating memory, returning NULL");
		return NULL;
	}
	/*initializing the 2 sons to NULL (also element to null) and returning pointer*/
	pNewNode->elem = NULL;
	pNewNode->leftChild = NULL;
	pNewNode->rightChild = NULL;
	return pNewNode;
}




/*end of Implemention of private functions*/

/*
Function:TreeCreate.
Inputs:Functions implemented by user (used to work with his element).
Action:Makes an initialized Tree structure database that can work with the user's element.
*/

pTree TreeCreate(CloneFunction CloneFunc, DelFunction DelFunc,
	OperateFunction OperateFunc, GetKeyFunction GetKeyFunc, CompareKeyFunction CompKeyFunc) {
	pTree PointerToTree = (pTree)malloc(sizeof(Tree)); // PointerToTree points to the struct of type Tree we just built.
	//printf("DEBUG:allocated pTree (1malloc) \n");
	/* Malloc Success Check*/
	if (PointerToTree == NULL) {
		printf("Error Allocating Tree struct in memory, Exiting and returning FAILURE");
		exit(FAILURE);
	}
	/* Placing the functions pointers inside the struct They are implemented by the user of the ADT*/
	PointerToTree->CloneFunction = CloneFunc; 
	PointerToTree->DelFunction = DelFunc;
	PointerToTree->OperateFunction = OperateFunc;
	PointerToTree->GetKeyFunction = GetKeyFunc;
	PointerToTree->CompareKeyFunction = CompKeyFunc;
	/*Initializing the struct of root*/
	PointerToTree->root = NULL;
	/* I think there are not needed , TODO:check if they are needed.
	PointerToTree->root->elem       = NULL;
	PointerToTree->root->leftChild  = NULL;
	PointerToTree->root->rightChild = NULL;
	*/
	return PointerToTree;
}
void DestroyChildren(pTree ptree) {
	pNode RootOfCurrDepth = ptree->root; //always updating the RootOfCurrent depth when using recursion.
/*algorithm: stack all pNode adresses on stack with recursion and then free element with DelFunc and free the node.*/
	if (ptree->root->rightChild != NULL) {
		ptree->root = ptree->root->rightChild;
		DestroyChildren(ptree);
		ptree->root = RootOfCurrDepth;
	}
	if (ptree->root->leftChild != NULL) {
		ptree->root = ptree->root->leftChild;
		DestroyChildren(ptree);
		ptree->root = RootOfCurrDepth;
	}
	/*this code will be implemented recursively for each node*/
//	printf("DEBUG:deleting an element \n");
	ptree->DelFunction(ptree->root->elem);//free all the memory that the element is using. TODO:must check that elem is not already freed.
	free(ptree->root);
}
/*
Function:TreeDestroy.
Inputs:pointer the head of the tree.
Action:Free all the allocated memory that the tree had.(including the memory of every copy or elements we had inside each node)
*/
void TreeDestroy(pTree ptree) {
	DestroyChildren(ptree);
		//if we have no more roots , free the empty tree struct that is left.
		//printf("DEBUG:free the empty tree struct , 1free");
		free(ptree);
		return;
	}
/*
Function:TreeAddLeftChild.
Inputs:Pointer to Tree, Pointer to node in the tree, Pointer to new element.
Action:Finds the the pNode of the correct tree that we want to add the new leftchild to and adds it.
return:In case of SUCCESS returns the pointer to the new node added (pNode) , else returns NULL.
*/
pNode TreeAddLeftChild(pTree ptree, pNode pnode, pElement pNewElem) {
	if (pnode->leftChild != NULL) {
	//	printf("DEBUG:this pNode already have a leftchild *retuning null*, pTree: %d , pNode: %d", &ptree, &pnode);
		return NULL;
	}
	/*we should get a Pointer to an EXISTING Node.*/
	if (pnode == NULL) {
		printf("ERROR:Trying to add leftchild to null, returning NULL");
		return NULL;
	}
	/* -----not needed after adding function makenode.
	Node* tmpPNode = (Node*)malloc(sizeof(Node));
	if ( tmpPNode == NULL) {
		printf("DEBUG:Failed to allocate memory in tmpPNode in AddLeftChild, returning null");
		return NULL;
	}
	*/
	Node* tmpPNode = makenode();
	pnode->leftChild = tmpPNode;
	/*making copy of the struct that pNewElem points to, pCpyNewElem points to it.*/
	pElement pCpyNewElem = ptree->CloneFunction(pNewElem);
	/* initializing the children of the leftchild node that we recently added*/
	pnode->leftChild->elem = pCpyNewElem;
	pnode->leftChild->leftChild = NULL;
	pnode->leftChild->rightChild = NULL;
	return (pnode->leftChild);
}
/*
Function:TreeAddRightChild.
Inputs:Pointer to Tree, Pointer to node, Pointer to new element.
Action:Finds the the pNode of the correct tree that we want to add the new RightChild to and adds it.
return:In case of SUCCESS returns the pointer to the new node added (pNode) , else returns NULL.
*/
pNode TreeAddRightChild(pTree ptree, pNode pnode, pElement pNewElem) {
	if (pnode->rightChild != NULL) {
	//	printf("DEBUG:this pNode already have a rightchild *retuning null*, pTree: %d , pNode: %d", &ptree, &pnode);
		return NULL;
	}
	if (pnode == NULL) {
		printf("ERROR:Trying to add rightchild to null, returning NULL");
		return NULL;
	}
	/* ---- Not needed after adding makenode function.
	Node* tmpPNode = (Node*)malloc(sizeof(Node));
	if (tmpPNode == NULL) {
		printf("DEBUG:Failed to allocate memory in tmpPNode in AddRightChild, returning with NULL");
		return NULL;
	}
	*/
	Node* tmpPNode = makenode();
	pnode->rightChild = tmpPNode;
	/*making copy of the struct that pNewElem points to, pCpyNewElem points to it.*/
	pElement pCpyNewElem = ptree->CloneFunction(pNewElem);
	/* initializing the children of the leftchild node that we recently added*/

	pnode->rightChild->elem = pCpyNewElem;
	pnode->rightChild->leftChild = NULL;
	pnode->rightChild->rightChild = NULL;
	return (pnode->rightChild);
}


/*
Function:TreeFindElement.
Inputs:Pointer to Tree, Pointer to node, Pointer to new element.
Action:Finds the element with the pKey that we are looking for .
The algorithm is based on recursive search in binary tree nodes.
return:In case of SUCCESS returns the pointer to the element we found , else returns NULL.
*/
pElement TreeFindElement(pTree ptree, const pKey Key) {
	if (ptree == NULL) { //there is no tree, don't search
		return NULL;
	}
	/* Local Variables*/
	pNode RootOfCurrDepth = ptree->root;
	pElement LeftResult = NULL;
	pElement RightResult = NULL;
	/* Stop Condition*/
	if (ptree->root == NULL) { //means that we already got to the leafs (longest depth in tree and didn't find anything)
		return NULL;
	}
	/*comparing the key of the current node is identical to they key given in funciton*/
	if (ptree->GetKeyFunction(ptree->root->elem) != NULL) {
		const pKey CurrElemKey = ptree->GetKeyFunction(ptree->root->elem);

		Bool result = ptree->CompareKeyFunction(CurrElemKey, Key);
		if (result == TRUE) {
			return RootOfCurrDepth->elem;
		}
	}
	else { // we enter else code if the Current node's key is not identical to the key we are comparing 
		   // updating the root and calling recursively.
		ptree->root = ptree->root->leftChild; //updating the root.
		LeftResult = TreeFindElement(ptree, Key);
		ptree->root = RootOfCurrDepth; //updating the root back to how it was when we enter the function first time.
		if (LeftResult != NULL) {      //means that we already found something.
			return LeftResult;
		}
		ptree->root = ptree->root->rightChild;//updating the root.
		RightResult = TreeFindElement(ptree, Key);
		ptree->root = RootOfCurrDepth; //updating the root back to how it was when we enter the function first time.
		if (RightResult != NULL) {
			return RightResult;
		}
	}
	//if we got to here means that we didn't find anything. returning NULL
	return NULL;
}
/*
Function:TreeAddRoot.
Inputs:Pointer to Tree,Pointer to new element.
Action:Adds the new element to the root field of the struct TREE.
return:In case of SUCCESS returns the pointer to the new node added (pNode) , else returns NULL.
*/
pNode TreeAddRoot(pTree ptree, pElement pElem) {
	/*Checking if element already exists in root*/
	if (ptree->root != NULL) {
	//	printf("DEBUG:There is already an element in ROOT . CANT ADD new element, returning NULL");
		return NULL;
	}
	if (pElem == NULL) { //check if needed.
	//	printf("adding a NULL element to the tree's root, Can't add. returning NULL");
		return NULL;
	}
	/*Making a new node and returning pointer to it to root field in ptree*/
	ptree->root = makenode(); //makenode deals with malloc checks and initialize sons and element.
	pElement pcpyElem = ptree->CloneFunction(pElem); //allocated memory with size of element and copied the struct pElem points to , to it.
	ptree->root->elem = pcpyElem;
	return (ptree->root);
}
pElement TreeEvaluateAux(pTree ptree, pNode CurrNode) {
	if (CurrNode->leftChild == NULL || CurrNode->rightChild == NULL) {
		return CurrNode->elem;
	}
	pElement right = TreeEvaluateAux(ptree, CurrNode->rightChild);
	if (right == NULL) {
	//	printf("DEBUG:right returned NULL \n");
		return NULL;
	}
	pElement left = TreeEvaluateAux(ptree, CurrNode->leftChild);
	if (left == NULL) {
	//	printf("DEBUG:left returned NULL \n");
		return NULL;
	}

	pElement ResultElem = ptree->OperateFunction(CurrNode->elem, left, right);
	if (ResultElem == NULL) {
		/* only if there is depth in the root , means that we allocated result in right and then divided by 0 => free what allocated.*/
		//printf("DEBUG:OPATEFUNC returned NULL \n");
		return NULL;
	}
	if (CurrNode->leftChild->leftChild != NULL) {
		ptree->DelFunction(left);
	}
	if (CurrNode->rightChild->rightChild != NULL) {
		ptree->DelFunction(right);
	}
	return ResultElem;
}

/*
Function:TreeEvaluate.
Inputs:Pointer to Tree.
Action:Recursively calculate the tree value by calculating operation between the elements.
return:In case of SUCCESS returns the answer in new element copy (using Clone) , else returns NULL.
*/
pElement TreeEvaluate(pTree ptree) { //check if root is null before calling aux
	/*Stop Condition - don't go to leafs.*/
		//its a strict binary tree database so either 2 of the sons exist or none so OR operation is enough for stop condition
	if (ptree == NULL) {
	//	printf("DEBUG:the tree is empty , returning null \n");
		return NULL;
	}
	//MKRE KATSI: if we only have one element inside the root, we make a clone of it and return it.
	if (ptree->root->leftChild == NULL || ptree->root->leftChild == NULL) {
		return ptree->CloneFunction(ptree->root->elem);
	}
	return TreeEvaluateAux(ptree, ptree->root);
}
