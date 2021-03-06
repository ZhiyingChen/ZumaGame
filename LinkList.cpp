#include "LinkList.h"


/*
Args: 
    * None.
Return: 
    * Pointer of the head node.
Description:
    * Create an empty linked list with a head node.
    * The "next" field of the head node should point to NULL.
*/
Node* CreateEmptyList()
{
	Node* head;
	
	head =(Node*) malloc(sizeof(Node));
	head->prior = head;
	head->next = head;

	return head;
}


/*
Args:
    * addr: pointer of an array.
    * n: length of the array.
Return:
    * Pointer of the head node.
Description:
    * Initialize a linked list with an array.
*/
Node* CreateList(DataType *addr, unsigned int n)
{
	Node *head;

	head = (Node*) malloc(sizeof(Node));
	head->next = head;
	head->prior = head;

	for (unsigned int i = 0; i < n; i++)
	{
		ListInsert(head, i, addr[i]);
	}

	return head;
}


/*
Args:
    * head: pointer of the head node.
Returns:
    * None
Description:
    * Destroy the linked list.
    * Release all allocated memory.
*/
void DestroyList(Node *head)
{
	Node *p;

	
	while (head->next != head)
	{
		p = head->next;
		free(head);
		head = p;
	}
	free(head);

}


/*
Args:
    * head: pointer of the head node.
    * index: index of the inserted data.
    * data: the inserted data.
Returns:
    * None.
Description:
    *   If the linked list is "head->3->5->2",
    * when you call (head, 0, 6), the linked 
    * list will be "head->6->3->5->2".
*/
void ListInsert(Node *head, unsigned int index, DataType data)
{
	unsigned int j = 0;
	Node* p = head;
	
	while (j <= index)
	{
		p = p->next;
		++j;
	}
	

	Node *s = (Node*)malloc(sizeof(Node));
	s->data = data;
	s->prior = p->prior;
	p->prior->next = s;
	s->next = p;
	p->prior = s;

}


/*
Args:
    * head: pointer of the head node.
    * index: index of the deleted data.
Returns:
    * The deleted data.
Description:
    *   If the linked list is "head->3->5->2",
    * when you call (head, 2), the linked 
    * list will be "head->3->5".
*/
DataType ListDelete(Node *head, unsigned int index)
{
	unsigned int j = 0;
	Node *p = head;
	Node* q;
	DataType data;

	while (p->next && j < index)
	{
		p = p->next;
		++j;
	}

	q = p->next;
	data = q->data;
	q->prior->next = q->next;
	q->next->prior = q->prior;
	free(q);

	return data;
}

void ListDeleteLast(Node* head)
{
	Node* p = head;
	Node* q;

	q = p->prior;
	
	q->prior->next = q->next;
	q->next->prior = q->prior;
	free(q);

	
}




