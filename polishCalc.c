#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "expTree.h"
#include "polishCalcTypes.h"
#include "polishCalc.h"

//ofcourse I can't go into ptree fields directly since they are in .c file ..
//using the ADT and changing its fields can be only done using the ADT functions. (treeaddroot etc)
#define RESULTELEM -1 //used to know if this element is made as usage to result element.

static pTree ptree = NULL; //if we don't have a tree then ptree is NULL , otherwise there is a tree.

/*static char* staticstr = NULL;
static char* CurrentToken = NULL; */
typedef enum { Left, Right } TypeOfSun;


/*
Function:CloneFunc
Inputs:pointer to element.
Action:Makes a copy of the element and returns the address to the pointer.
*/
pElement CloneFunc(pElement element) {
	CalcElement* CastedElem = (CalcElement*)element; //casting
	CalcElement* pCopyOfElement = malloc(sizeof(*pCopyOfElement));
	//printf("DEBUG:allocating copy of element. (1malloc) \n");
	if (pCopyOfElement == NULL) {
		//printf("CloneFunc: failed to allocate memory for the element exiting with FAILURE.");
		exit(FAILURE);
	}
	/*setting pCopyOfElement fields , only allocate key if the element is symbol.*/
	if (CastedElem->key != NULL) {
		int sizeofstring = strlen(CastedElem->key) + 1;
		pCopyOfElement->key = (pKey)malloc(sizeofstring);
		//	printf("DEBUG:allocating a key for the element (1malloc) , there shouldn't be a malloc if its not a symbol.");
		if (pCopyOfElement->key == NULL) {
			//printf("Error allocatting key for elementCopy in CloneFunction.");
			exit(FAILURE);
		}
	}
	if (CastedElem->key != NULL) {
		//if success, copy the content of key.
		strcpy(pCopyOfElement->key, CastedElem->key); /* char *strcpy(char *dest, const char *src) */
	}
	else { // if we don't have a key.
		pCopyOfElement->key = NULL;
	}
	pCopyOfElement->opType = CastedElem->opType;
	pCopyOfElement->type = CastedElem->type;
	pCopyOfElement->val = CastedElem->val;
	return pCopyOfElement;
}
/*
Function:DelFunc.
Inputs:Pointer to an element.
Action:Free all the memory allocated of the struct that the user made.
return: -- .
*/
void DelFunc(pElement pElem) {
	CalcElement* CastedElem = (CalcElement*)pElem; //casting
	/* We have 2 memory allocations. one for key and one for the whole struct. first we free key.*/
	/*allocted memory for key only in case there was a symbol*/
	//printf("DEBUG:CastedElem->key:%d, Containing:%s \n", CastedElem->key, CastedElem->key);
	if (CastedElem->key != NULL) {
		//printf("DEBUG, free the element's key. (1free) \n");
		free(CastedElem->key); 
		CastedElem->key = NULL;
	}
	//printf("DEBUG, free the element. (1free) \n");
	free(CastedElem);

}

/*
Function:OperateFunc.
Inputs:Operation, LeftElem, RightElem.
Action:Gets 3 pointers to elements, and does the operation
*/
pElement OperateFunc(pElement op, pElement LeftElem, pElement RightElem) {
	CalcElement* CastedOp = (CalcElement*)op;
	CalcElement* CastedLeftElem = (CalcElement*)LeftElem;
	CalcElement* CastedRightElem = (CalcElement*)RightElem;
	if (CastedOp == NULL || CastedLeftElem == NULL || CastedRightElem == NULL) { //in case any of these are NULL.
		return NULL;
	}
	CalcElement* ResultElem = (CalcElement*)malloc(sizeof(*ResultElem));
	ResultElem->key = NULL; //we don't need a key in ResultElem. (we always set key to NULL if element is not an operator).
	//printf("allocating ResultElem in OperateFunc (1malloc) without a key \n");
	if (ResultElem == NULL) {
		//printf("Error allocating ResultElem in OperateFunc, returning NULL");
		return NULL;
	}
	switch (CastedOp->opType) {
	case ADD:
		ResultElem->val = (CastedRightElem->val) + (CastedLeftElem->val);
		ResultElem->key = NULL;
		ResultElem->opType = OPERAND;
		ResultElem->type = RESULTELEM; //I don't know what to set.
		return (void*)ResultElem; //instead of break.
	case SUB:
		ResultElem->val = (CastedRightElem->val) - (CastedLeftElem->val);
		ResultElem->key = NULL;
		ResultElem->opType = OPERAND;
		ResultElem->type = RESULTELEM; //I don't know what to set.
		return (void*)ResultElem; //instead of break.
	case MUL:
		ResultElem->val = (CastedRightElem->val) * (CastedLeftElem->val);
		ResultElem->key = NULL;
		ResultElem->opType = OPERAND;
		ResultElem->type = RESULTELEM; //I don't know what to set.
		return (void*)ResultElem; //instead of break.
	case DIV:
		if (CastedLeftElem->val == 0) { //if dividing by 0 , free the allocated resultelem and return NULL;
		//	printf("DEBUG:divided by 0 , going to free what we allocated and return NULL (1free) \n");
			/*if the element->type ==RESULTELEM (a unique type for an element that we allocated) free it. (USER (us) responsible for free)*/
			if (CastedLeftElem->type == RESULTELEM) {
				DelFunc(CastedLeftElem);
			}
			if (CastedRightElem->type == RESULTELEM) {
				DelFunc(CastedRightElem);
			}
			DelFunc(ResultElem);
			return NULL;
		}
		ResultElem->val = (CastedRightElem->val) / (CastedLeftElem->val);
		ResultElem->key = NULL;
		ResultElem->opType = OPERAND;
		ResultElem->type = RESULTELEM; //I don't know what to set.
		return (void*)ResultElem; //instead of break.
	}
	//printf("DEBUG:Unknown opType, going to free the memory allocated for ResultElem (1free) \n ");
	DelFunc(ResultElem);
	return NULL; //FAIL.
}
/*
Function:GetKeyFunc.
Inputs:Pointer to Element.
Action:gets Pointer to an element, returns the key of it element.
*/
pKey GetKeyFunc(pElement elem) {


	CalcElement* CastedElem = (CalcElement*)elem; //casting
	char* CurrElemKey = CastedElem->key;

	return (pKey)CurrElemKey; //Casted to pKey.
}
/*
Function:CompKeyFunc.
Inputs : Pointer to key1, Pointer to key2.
Action : Compares the 2 keys, If the keys are Identical the function returns TRUE, else returns FALSE.
*/
Bool CompKeyFunc(const pKey key1, const pKey key2) {
	if (strcmp((char*)key1, (char*)key2) == 0) {
		return TRUE;
	}
	return FALSE;
}


/*
Function:InitElement.
Inputs : Pointer to char.
Action : allocates memory for CalcElement, and it initializes it accoridng to the token it just took.
*/
CalcElement* InitElement(char* token) {

	//Operator case:
	CalcElement* ptr = (CalcElement*)malloc(sizeof(*ptr));
	//printf("DEBUG: allocating Element (1malloc) \n");
	if (ptr == NULL) {
		//printf("Error allocating in InitElement method.");
		return NULL;
	}
	//printf("length of current initexpression:%d \n", strlen(token));
	if (strlen(token) == 1) { //OPERATOR must only be ONE char. (this if fixes the following bug:if we get +++ then its symbol.)
		if (token[0] == '+' || token[0] == '/' || token[0] == '-' || token[0] == '*')
		{
			ptr->type = OPERATOR;
			if (token[0] == '+') ptr->opType = ADD;
			if (token[0] == '-') ptr->opType = SUB; //they added to Homework that we cant have negative operands.
			if (token[0] == '*') ptr->opType = MUL;
			if (token[0] == '/') ptr->opType = DIV;
			ptr->key = NULL;
			ptr->val = 0;
			return ptr;
		}
	}
	// if we got to this line then we have 2 options , 1) Symbol , 2)Operand.
	//we assume that we have an Operand , if we prove to have a Symbol => make symbol element and return ptr. else make operand elem.
	int i = 0;
	Bool IsSymbol = FALSE; //Dealing with symbols.
	int DotCounter = 0;
	while (token[i]) { //run over the token , and if you find something that is not a number =>its a token is symbol.
		if (!( (token[i] >= '0' && token[i] <= '9') || token[i] == '.')) {
			//	printf("CurrToken:%c \n", token[i]);
			IsSymbol = TRUE;
			break;
		}
		if (token[i] == '.') { //for cases of symbols with 2 dots and more.
			DotCounter++;
		}
		i++;
	}
	if (DotCounter > 1) {
		IsSymbol = TRUE;
	}
	if (IsSymbol) { //Symbol case:
	//	printf("DEBUG:we are in symbol initializing \n");
		int sizeofstring = strlen(token) + 1;
		char * keyptr = (char*)malloc(sizeofstring); // +1 for the '\0'.
		if (keyptr == NULL) {
			printf("Error allocating key for symbol");
			return NULL;
		}
		//printf("DEBUG:Allocated a key for the current element. (1malloc) \n");
		ptr->type = SYMBOL;
		ptr->key = keyptr;
		strcpy(ptr->key, token);
		ptr->opType = 0;
		ptr->val = 0;
		//	printf("DEBUG:symbolKey:%s,SymbolVal:%f \n", ptr->key, ptr->val);
		return ptr;
	}
	else { //Operand case:
	//	printf("DEBUG:CheckIfSymbol is %f \n", CheckIfSymbol);
		ptr->type = OPERAND;
		ptr->key = NULL;
		ptr->opType = 0; //doesn't matter
		ptr->val = atof(token);
		return ptr;
	}

}
Result RecursiveBuilder(char* token, pNode CurrNode, pTree ptree) {

	char* LastToken = token;
	char* AfterLastToken = NULL;
	AfterLastToken = strtok(NULL, " \r");

	Result RecursiveRes = FAILURE;
	pNode NewRootNode;
	CalcElement* NewElement;

	if (AfterLastToken == NULL)
		return FAILURE;

	if ((LastToken[0] == '+' || LastToken[0] == '/' || LastToken[0] == '-' || LastToken[0] == '*')
		&& strlen(LastToken) == 1) //strlen added to make sure that this is an operator.
	{
		NewElement = InitElement(LastToken);
		if (NewElement == NULL) {
			return FAILURE;
		}


		NewRootNode = TreeAddRightChild(ptree, CurrNode, NewElement);
		DelFunc(NewElement);

		if (NewRootNode == NULL) {
			//printf("DEBUG:NewRootNode = NULL ,Deleting an element");
			return FAILURE;
		}


		RecursiveRes = RecursiveBuilder(AfterLastToken, NewRootNode, ptree);
		/*if (NewElement->key != NULL) {
			free(NewElement->key);
		}*/



		if (RecursiveRes == FAILURE) {
			return FAILURE;

		}

		AfterLastToken = strtok(NULL, " \r");
		if (AfterLastToken == NULL) {
			return FAILURE;
		}

		NewElement = InitElement(AfterLastToken);
		if (NewElement == NULL) {
			return FAILURE;
		}

		NewRootNode = TreeAddLeftChild(ptree, CurrNode, NewElement);
		DelFunc(NewElement);

		if (NewRootNode == NULL) {
			//printf("DEBUG:NewRootNode = NULL ,Deleting an element");
			return FAILURE;
		}
		if ((AfterLastToken[0] == '+' || AfterLastToken[0] == '/' || AfterLastToken[0] == '-' || AfterLastToken[0] == '*')
			&& strlen(AfterLastToken) == 1)
		{
			AfterLastToken = strtok(NULL, " \r");
			if (AfterLastToken == NULL) return FAILURE;

			return (RecursiveBuilder(AfterLastToken, NewRootNode, ptree));
		}

		else
			return SUCCESS;

		
	}

	else
	{
		NewElement = InitElement(LastToken);
		if (NewElement == NULL) {
			return FAILURE;
		}
		NewRootNode = TreeAddRightChild(ptree, CurrNode, NewElement);
		DelFunc(NewElement);

		if (NewRootNode == NULL) {
			//printf("DEBUG:NewRootNode = NULL ,Deleting an element");

			return FAILURE;
		}
		NewElement = InitElement(AfterLastToken);
		if (NewElement == NULL) {
			return FAILURE;
		}

		NewRootNode = TreeAddLeftChild(ptree, CurrNode, NewElement);
		DelFunc(NewElement);



		if (NewRootNode == NULL) {
			//printf("DEBUG:NewRootNode = NULL ,Deleting an element");
			return FAILURE;
		}
		if ((AfterLastToken[0] == '+' || AfterLastToken[0] == '/' || AfterLastToken[0] == '-' || AfterLastToken[0] == '*')
			&& strlen(AfterLastToken) == 1)
		{
			AfterLastToken = strtok(NULL, " \r");
			if (AfterLastToken == NULL) {
				return FAILURE;
			}

			return (RecursiveBuilder(AfterLastToken, NewRootNode, ptree));
		}

		else {
		

			return SUCCESS;

		}


	}


	return SUCCESS;



}



Result InitExpression(char* str) {
	if (ptree != NULL) { //means that we already have a tree, delete the whole expression and add the new one.
		DeleteExpression();
	}
	Result res1 = FAILURE;
	if (str == NULL) {
		return res1;
	}
	char* WhatsLeftFromStr = NULL;
	char* delimiters = " \r";
	char* token = strtok(str, delimiters);

	CalcElement* CurrElement = InitElement(token);
	if (CurrElement == NULL) {
		return FAILURE;
	}

	ptree = TreeCreate(&CloneFunc, &DelFunc, &OperateFunc, &GetKeyFunc, &CompKeyFunc); //build tree with element functions.
	pNode pTreeRoot = TreeAddRoot(ptree, (void*)CurrElement);
	if (pTreeRoot == NULL) {
		DelFunc(CurrElement);
		return FAILURE;

	}
	char* FirstAfterRoot = strtok(NULL, delimiters);
	if (CurrElement->type == SYMBOL && FirstAfterRoot == NULL) { //Case of having symbol "+++" or "98+89*-/*/898*"
		/*Don't go into recursive builder. just return SUCCESS because we only have root.*/
			//we allocated CurrElement , added to the root , we need to free it now.
		DelFunc(CurrElement);
		return SUCCESS;
	}

	if (token[0] != '+' &&  token[0] != '/' && token[0] != '-' && token[0] != '*' && FirstAfterRoot == NULL) { // this deals with CASES like "EXP 5" 
			//we allocated CurrElement , added to the root , we need to free it now.
		DelFunc(CurrElement);
		return SUCCESS;
	}

	if (token[0] != '+' &&  token[0] != '/' && token[0] != '-' && token[0] != '*' && FirstAfterRoot != NULL) { // this deals with CASES like "EXP 9 +" 
			//we allocated CurrElement , added to the root , we need to free it now.
		DelFunc(CurrElement);
		return FAILURE;
	}
	if (FirstAfterRoot == NULL && CurrElement->type != OPERATOR) { //case of symbol ++++ //// +/*/*/+
			//we allocated CurrElement , added to the root , we need to free it now.
		DelFunc(CurrElement);
		return SUCCESS;
	}
	//we alreadty added it to root , we don't need CurrElement anymore.
	DelFunc(CurrElement);
	res1 = RecursiveBuilder(FirstAfterRoot, pTreeRoot, ptree);
	

	WhatsLeftFromStr = strtok(NULL, delimiters);
	if (WhatsLeftFromStr != NULL) {
			//DelFunc(CurrElement); not needed
		    return FAILURE;
	}

	else {
		    
			//DelFunc(CurrElement); not needed
		    return res1;
	}

}

Result SetSymbolVal(char* symName, float val) {
	CalcElement* WantedElem = TreeFindElement(ptree, symName); //finds the element with the wanted symbol.
	if (WantedElem == NULL) {
		return FAILURE; //we didn't find element with the current symbol.
	}
	/*if we found the element then we do the following: Convert the element from symbol to operand*/
	WantedElem->opType = 0; //Default value for symbols is 0.
	WantedElem->val = val;
	WantedElem->type = OPERAND;
	return SUCCESS;
}

Result EvaluateExpression(float *res) {
	CalcElement* ResultElement = TreeEvaluate(ptree);
	//TreeEvaluate made and element with the results inside it. we need Result Field.
	if (ResultElement == NULL) {
		//	printf("DEBUG:ResultElement is null. \n");
		return FAILURE;
	}
	*res = ResultElement->val; //updated the answer in res.
	//printf("DEBUG:result is:%f", *res);
	/*after having the result we don't need the element that TreeEvaluate allocated, deleting it*/
	DelFunc(ResultElement);

	return SUCCESS;
}

void DeleteExpression() {
	if (ptree == NULL) {
		//	printf("there is no tree wtf are you trying to destroy??");
		return;
	}
	TreeDestroy(ptree);
}







