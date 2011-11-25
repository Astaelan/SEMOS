extern "C" {
#include <stdlib.h>
}
#include <Utility/List.h>

Node * List_Find(List * pList,
                 PVOID pData)
{
	if (!pData) return NULL;
	for (Node* node = pList->Head; node; node = node->Next) if (node->Data == pData) return node;
	return NULL;
}

void List_Add(List * pList,
              PVOID pData)
{
	Node * node = (Node*)malloc(sizeof(Node));
	node->Prev = NULL;
	node->Next = NULL;
	node->Data = pData;
	if (!pList->Head) pList->Head = node;
	else { pList->Tail->Next = node; node->Prev = pList->Tail; }
	pList->Tail = node;
	++pList->Count;
}

void List_RemoveNode(List * pList,
                     Node * pNode)
{
	if (pNode == pList->Head) pList->Head = pList->Head->Next;
	if (pNode == pList->Tail) pList->Tail = pList->Tail->Prev;
	if (pNode->Prev) pNode->Prev->Next = pNode->Next;
	if (pNode->Next) pNode->Next->Prev = pNode->Prev;
	free(pNode);
}

BOOL List_Remove(List * pList,
                 PVOID pData)
{
	Node * node = List_Find(pList, pData);
	BOOL removed = FALSE;
	if (node) { List_RemoveNode(pList, node); removed = TRUE; }
	return removed;
}