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

void makeEmptyStockList(STOCK* lst) {

	lst->head = lst->tail = NULL;
	lst->size = 0;
}

void makeEmptyAptList(APT_LIST* lst) {

	lst->head = lst->tail = NULL;
	lst->size = 0;
}

STOCK_NODE* makeStockNode(char* command) {

	STOCK_NODE* node = (STOCK_NODE*)malloc(sizeof(STOCK_NODE));
	node->command = (char*)malloc(strlen(command) * sizeof(char));
	strcpy(node->command, command);
	node->next = node->prev = NULL;
	return node;
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

	apt->address = (char*)malloc(strlen(ad) * sizeof(char));
	allocationCheck(apt->address);
	strcpy(apt->address, ad);
	apt->code = code;
	apt->price = price;
	apt->date = date;
	apt->rooms = rooms;
	apt->database_Entry_Date = database_Entry_Date;

	return apt;
}

void printList(APT_LIST* lst) {

	if (!lst->size) {

		puts("EMPTY");
		return;
	}
	else {

		APT* cur = lst->head;
		uint i;
		for (i = 0; i < lst->size && cur; i++) {

			printApt(cur);
			puts("");
			cur = cur->next;
		}

	}

}

void printApt(APT* apt) {


	printf("Address: %s\nPrice: %d\nCode: %d\nRooms: %d\nDate: %d/%d/%d\nEntry time: %s", apt->address, apt->price, apt->code,
		apt->rooms, apt->date.day, apt->date.month, apt->date.year, ctime(&apt->database_Entry_Date));
}

void copyList(APT_LIST* target, APT_LIST* source) {

	uint i;
	APT* cur = source->head;
	for (i = 0; (i < source->size && cur); i++) {

		(target->size)++;
		addToTail(target, makeApt(cur->address, cur->code, cur->price, cur->rooms, cur->date, cur->database_Entry_Date));
		cur = cur->next;

	}
}

void removeInnerNode(APT* node) {

	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
}

void removeHead(APT_LIST* apt) {

	apt->head = apt->head->next;
	free(apt->head->prev);
	apt->head->prev = NULL;
}

void removeTail(APT_LIST* apt) {

	apt->tail = apt->tail->prev;
	free(apt->tail->next);
	apt->tail->next = NULL;
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

void reverseArrToList(APT** arr, APT_LIST* lst) {

	uint i;
	lst->head = arr[lst->size - 1];
	lst->tail = arr[0];
	for (i = 0; i < lst->size; i++) {

		if (!i) {

			arr[i]->prev = arr[i + 1];
			arr[i]->next = NULL;
		}
		else if (i == (lst->size - 1)) {

			arr[i]->prev = NULL;
			arr[i]->next = arr[i - 1];
		}
		else {

			arr[i]->prev = arr[i + 1];
			arr[i]->next = arr[i - 1];
		}
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

void deleteList(APT_LIST* lst) {

	if (lst->head)
		deleteListRec(lst->head);
}

void deleteListRec(APT* node) {

	if (node->next)
		deleteListRec(node->next);
	free(node);
}

uint nextPos() {

	uint i = 0;
	while (short_term_history[i]) {

		i++;
	}
	return i;
}