#ifndef EXP_TREE_H
#define EXP_TREE_H

#include "defs.h"

/* complete the definition of the pointers to Tree and Node */
		  	/*      Definitions of pointers     */
typedef void* pElement;
typedef void* pKey;
typedef struct _node* pNode; //pointer to a node
typedef struct _tree* pTree; // pointer to tree

			/* Definition of Interface functions*/
typedef pElement	(*CloneFunction)(pElement e);
typedef void		(*DelFunction)(pElement e);
typedef pElement	(*OperateFunction)(pElement op, 
									   pElement left, 
									   pElement right);
typedef pKey		(*GetKeyFunction)(pElement elem);
typedef Bool		(*CompareKeyFunction)(const pKey key1, 
										  const pKey key2);
		    /* Definition of Interface Functions of ADT (used by user) */
pTree    TreeCreate(CloneFunction CloneFunc, DelFunction DelFunc,
			        OperateFunction OperateFunc, GetKeyFunction GetKeyFunc, CompareKeyFunction CompKeyFunc);
void     TreeDestroy(pTree ptree);
pNode    TreeAddLeftChild(pTree ptree, pNode pnode, pElement pNewElem);
pNode    TreeAddRightChild(pTree ptree, pNode pnode, pElement pNewElem);
pNode    TreeAddRoot(pTree ptree, pElement pElem);
pElement TreeFindElement(pTree ptree, const pKey Key);
pElement TreeEvaluate(pTree ptree);
/* I think this is not what we had to add. TODO:check.
typedef pTree		(*TreeCreate)(CloneFunction, DelFunction,OperateFunction, GetKeyFunction, CompareKeyFunction);
typedef void        (*TreeDestroy)(pTree e);
typedef pNode       (*TreeAddRoot)(pTree ptree, pElement pElem);
typedef pNode		(*TreeAddLeftChild)( pTree ptree, pNode pNode, pElement pNewElem);
typedef pNode		(*TreeAddRightChild)(pTree ptree, pNode pNode, pElement pNewElem);
typedef pElement	(*TreeFindElement)(pTree ptree, const pKey key);
typedef pElement	(*TreeEvaluate)(pTree ptree);
*/


/* complete the definition of the interface functions */


#endif	// EXP_TREE_H