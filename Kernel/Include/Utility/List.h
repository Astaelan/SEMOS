#ifndef _LIST_H
#define _LIST_H

#include <types.h>

typedef struct _Node {
	struct _Node * Next;
	struct _Node * Prev;
	PVOID Data;
} Node;

typedef struct {
	Node * Head;
	Node * Tail;
	UINT32 Count;
} List;

Node * List_Find(List * pList, PVOID pData);
void List_Add(List * pList, PVOID pData);
void List_RemoveNode(List * pList, Node * pNode);
BOOL List_Remove(List * pList, PVOID pData);

#endif