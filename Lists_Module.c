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


void addToHeadStockList(STOCK *stock, STOCK_NODE *node) {

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

void addToList(APT_LIST* lst, APT* node) {

	if ((!lst->head) && (!lst->tail)) {
		lst->head = lst->tail = node;
	}
	else {

		node->next = lst->head;
		lst->head->prev = node;
		lst->head = node;
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


	printf("Address: %s\nPrice: %d\nRooms: %d\nDate: %d/%d/%d\nEntry time: %s", apt->address, apt->price,
		apt->rooms, apt->date.day, apt->date.month, apt->date.year, ctime(&apt->database_Entry_Date));
}






