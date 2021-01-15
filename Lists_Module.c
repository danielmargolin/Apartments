/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 20/10/2020

	Final Project
	Main file
*/
#include "Lists_Module.h"

/*************** Static Functions Prototypes ****************/

static void printOnlyCodesRec(APT* apt);
static void printApt(FILE* f_ptr, APT* apt);
static void removeInnerNode(APT* node);
static void removeHead(APT_LIST* apt);
static void removeTail(APT_LIST* apt);
static void freeFunctionListRec(FIND_FUNCTION_NODE* head);

/*************** Public Functions ****************/

void makeEmptyStockList(STOCK* lst) {

	lst->head = lst->tail = NULL;
	lst->size = 0;
}

void makeEmptyAptList(APT_LIST* lst) {

	lst->head = lst->tail = NULL;
	lst->size = 0;
}

void makeEmptyFindFunctionList(FIND_FUNCTIONS_LIST* lst) {

	lst->head = NULL;
	lst->size = 0;
}

STOCK_NODE* makeStockNode(char* command) {

	STOCK_NODE* node = (STOCK_NODE*)malloc(sizeof(STOCK_NODE));
	node->command = (char*)malloc(strlen(command) * sizeof(char));
	strcpy(node->command, command);
	node->next = node->prev = NULL;
	return node;
}

FIND_FUNCTION_NODE* makeFindFunctionNode(FIND_FUNCTION* findFunction, int param) {

	FIND_FUNCTION_NODE* node = (FIND_FUNCTION_NODE*)malloc(sizeof(FIND_FUNCTION_NODE));
	node->function = findFunction;
	node->param = param;
	node->next = NULL;
	return node;
}

void freeFunctionList(FIND_FUNCTIONS_LIST* lst) {

	if (lst->head)
		freeFunctionListRec(lst->head);
}

void deletehead(STOCK* lst) {

	if (lst->head->next) {

		lst->head = lst->head->next;
		free(lst->head->prev);
		lst->head->prev = NULL;
	}
	else {

		free(lst->head);
		lst->head = lst->tail = NULL;
	}
}

void addToHead(APT_LIST* lst, APT* node) {

	if ((!lst->head) && (!lst->tail)) {
		lst->head = lst->tail = node;
	}
	else {

		node->next = lst->head;
		lst->head->prev = node;
		lst->head = node;
	}
}

void addToTail(APT_LIST* lst, APT* node) {

	if ((!lst->head) && (!lst->tail)) {
		lst->head = lst->tail = node;
	}
	else {

		node->prev = lst->tail;
		lst->tail->next = node;
		lst->tail = node;
	}
}

APT* makeApt(char* ad, unsigned int code, int price, int rooms, DATE date, time_t database_Entry_Date) {

	APT* apt = (APT*)malloc(sizeof(APT));
	allocationCheck(apt);
	apt->prev = apt->next = NULL;
	apt->address = (char*)calloc(strlen(ad)+1, sizeof(char));
	apt->address[strlen(ad)] = '\0';
	allocationCheck(apt->address);
	strcpy(apt->address, ad);
	apt->code = code;
	apt->price = price;
	apt->date = date;
	apt->rooms = rooms;
	apt->database_Entry_Date = database_Entry_Date;
	return apt;
}

void printList(FILE* f_ptr, APT_LIST* lst) {

	if (lst->head)
		printApt(f_ptr, lst->head);
	else
		puts("There are no such apartments in the database");
}

void printOnlyCodes(APT_LIST* lst) {

	if (lst->head)
		printOnlyCodesRec(lst->head);
	else
		puts("There are no such apartments in the database");
}

void copyList(APT_LIST* target, APT_LIST* source) {

	uint i;
	APT* cur = source->head;
	for (i = 0; (i < source->size && cur); i++) {

		(target->size)++;
		addToTail(target, makeApt(cur->address, cur->code, cur->price, cur->rooms, cur->date, cur->database_Entry_Date));
		cur = cur->next;

	}
	target->head->prev = target->tail->next = NULL;
}

void removeNode(APT_LIST* apt, APT* node) {

	if (node->next && node->prev)
		removeInnerNode(node);
	else if (node->next)
		removeHead(apt);
	else if (node->prev)
		removeTail(apt);
	else {

		apt->head = apt->tail = NULL;
		free(node);
	}
}

void addToFunctionList(FIND_FUNCTIONS_LIST* lst, FIND_FUNCTION_NODE* node) {
	FIND_FUNCTION_NODE* head = lst->head;
	if (head == NULL) {
		lst->head = node;
		lst->size = 1;
	}
	else {
		while (head->next != NULL) {
			head = head->next;
		}
		head->next = node;
		lst->size = lst->size + 1;
	}
}

void arrToList(APT** arr, APT_LIST* lst) {

	lst->head = arr[0];
	lst->tail = arr[lst->size - 1];

	uint i;

	for (i = 0; i < lst->size; i++) {

		if (!i) {

			arr[i]->prev = NULL;
			arr[i]->next = arr[i + 1];
		}
		else if (!arr[i + 1]) {

			arr[i]->prev = arr[i - 1];
			arr[i]->next = NULL;
		}
		else {

			arr[i]->prev = arr[i - 1];
			arr[i]->next = arr[i + 1];
		}
	}
}

APT** listToArr(APT_LIST* lst) {

	APT* cur = lst->head;
	APT** arr = (APT**)malloc(lst->size * sizeof(APT*));
	allocationCheck(arr);
	uint i;
	for (i = 0; i < lst->size && cur; i++) {

		arr[i] = cur;
		cur = cur->next;
		arr[i]->next = arr[i]->prev = NULL;
	}
	return arr;
}

void freeList(APT_LIST* lst) {

	if (lst->head)
		freeListRec(lst->head);
}

void freeListRec(APT* node) {

	if (node->next)
		freeListRec(node->next);
	deleteNode(node);
}

void deleteNode(APT* node) {

	free(node->address);
	free(node);
}

uint nextPos(uint i) {

	if (short_term_history[i])
		return nextPos(++i);
	return i;
}

void addToHeadStockList(STOCK* stock, STOCK_NODE* node) {

	if (!stock->head)
		stock->head = stock->tail = node;
	else {

		node->next = stock->head;
		stock->head->prev = node;
		stock->head = node;
	}
}

void addToTailStockList(STOCK* stock, STOCK_NODE* node) {

	if (!stock->head)
		stock->head = stock->tail = node;
	else {
		stock->tail->next = node;
		node->prev = stock->tail;
		stock->tail = node;
	}
}

/*************** Static Functions ****************/

static void printApt(FILE* f_ptr, APT* apt) {

	struct tm* info;
	char buffer[80];
	info = localtime(&apt->database_Entry_Date);
	strftime(buffer, 80, "%x", info);
	uint month = atoi(strtok(buffer, "/"));
	uint day = atoi(strtok(NULL, "/"));
	uint year = 2000 + atoi(strtok(NULL, "/"));
	printf("Apt details:\nCode: %d\nAddress: %s\nNumber of rooms: %d\nPrice: %d\nEntry date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n",
		apt->code, apt->address, apt->rooms, apt->price, apt->date.day, apt->date.month, apt->date.year, day, month, year);
	fprintf(f_ptr, "Apt details:\nCode: %d\nAddress: %s\nNumber of rooms: %d\nPrice: %d\nEntry date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n",
		apt->code, apt->address, apt->rooms, apt->price, apt->date.day, apt->date.month, apt->date.year, day, month, year);
	if (apt->next)
		printApt(f_ptr, apt->next);
}

static void printOnlyCodesRec(APT* apt) {

	if (apt)
		printf("%d\n", apt->code);
	if (apt->next)
		printOnlyCodesRec(apt->next);
}

static void removeInnerNode(APT* node) {

	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
}

static void removeHead(APT_LIST* apt) {

	apt->head = apt->head->next;
	free(apt->head->prev);
	apt->head->prev = NULL;
}

static void removeTail(APT_LIST* apt) {

	apt->tail = apt->tail->prev;
	free(apt->tail->next);
	apt->tail->next = NULL;
}

static void freeFunctionListRec(FIND_FUNCTION_NODE* head) {

	if (head->next)
		freeFunctionListRec(head->next);
	free(head);
}