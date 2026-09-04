
#include <stdio.h>
#include <stdlib.h>

//	Structs
//
struct ListNode {
	int value;
	struct ListNode* next;
	struct ListNode* prev;
};

struct LinkedList {
	int count;
	struct ListNode* head;
	struct ListNode* tail;
};


//	Function Prototypes
//
struct LinkedList* ListInit();
void FreeList(struct LinkedList* list);
void ListAdd(struct LinkedList* list, int value);
int ListRemove(struct LinkedList* list, int value);
int ListRemoveAt(struct LinkedList* list, int index);
int ListSnip(struct LinkedList* list);
int ListSnipCount(struct LinkedList* list, int snipCount);
void ListDelete(struct LinkedList* list);
void ListPrint(struct LinkedList* list);


//	Function Implementations
//
struct LinkedList* ListInit() {
	struct LinkedList* list = malloc(sizeof(struct LinkedList));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

//	Free all nodes in LinkedList and free the list itself
//
void FreeList(struct LinkedList* list) {
	ListSnipCount(list, list->count);

	struct LinkedList* tmp = list;
	list = NULL;
	free(tmp);
}

//	Append new value to the end of the LinkedList
//
void ListAdd(struct LinkedList* list, int value) {
	struct ListNode* node = malloc(sizeof(struct ListNode));
	node->next = NULL;
	node->prev = NULL;
	node->value = value;

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
		list->count += 1;
		ListPrint(list);
		return;
	}
	
	list->tail->next = node;
	node->prev = list->tail;
	node->next = NULL;
	
	list->tail = node;

	list->count += 1;

	ListPrint(list);
}

//	Removes value from LinkedList
//
int ListRemove(struct LinkedList* list, int value) {

	if (list->head == NULL) return 1;

	struct ListNode* current = list->head;
	struct ListNode* prev = NULL;

	while (current != NULL && current->value != value) {
		prev = current;
		current = current->next;
	}

	if (current == NULL) {
		printf("invalid. list does not contain value");
		return 1;
	}

	if (current == list->head) {
		struct ListNode* tmp = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		if (current == list->tail) list->tail = NULL;

		free(tmp);
	}
	else {
		prev->next = current->next;
		current->next->prev = prev;

		if (current == list->tail) {
			list->tail = prev;
		}

		free(current);

	}

	list->count -= 1;

	return 0;
}

//	Snips tail node off of LinkedList
//
int ListSnip(struct LinkedList* list) {
	if (list->head == NULL) return 1;

	if (list->head == list->tail) {
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		struct ListNode* tmp = list->tail;
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		free(tmp);
	}
	
	list->count -= 1;

	ListPrint(list);	
}

//	Snips tail node off of Linked List
//	multiple times. Functionally similar
//	to popping a stack N number of times.
//
int ListSnipCount(struct LinkedList* list, int snipCount) {
	if (snipCount > list->count) {
		printf("invalid. snipCount greater than list size");
		return 1;
	}
	else if (snipCount == 0) {
		return 0;
	}

	for (int i = 0; i < snipCount; i++) {
		if (ListSnip(list) == 1) return 1;
	}
}

//	Print the list to console using printf
//
void ListPrint(struct LinkedList* list) {
	struct ListNode* current = list->head;
	int i = 0;
	
	printf("List: ");
	while (current != NULL && i < list->count) {
		printf("%d ", current->value);
		current = current->next;
		i++;
	}
	printf("\n");
}

//	Entrypoint
//
int main() {

	struct LinkedList* list = ListInit();
	
	ListAdd(list, 20);
	ListAdd(list, 4040);
	ListAdd(list, 781);
	ListAdd(list, 200);

	ListSnipCount(list, list->count);

	FreeList(list);

}
