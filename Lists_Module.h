/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 20/10/2020

	Final Project
	Structs and Constants h file
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Structs_And_Consts.h"



#ifndef ___LISTS_MODULE_H___
#define ___LISTS_MODULE_H___


/*************** Functions Prototypes ****************/

void makeEmptyStockList(STOCK* lst);

void makeEmptyFindFunctionList(FIND_FUNCTIONS_LIST* lst);

FIND_FUNCTION_NODE* makeFindFunctionNode(FIND_FUNCTION* findFunction, int param);

void addToFunctionList(FIND_FUNCTIONS_LIST* lst, FIND_FUNCTION_NODE* node);

void freeFunctionList(FIND_FUNCTIONS_LIST* lst);

void deletehead(STOCK* lst);

void makeEmptyAptList(APT_LIST* lst);

APT* makeApt(char* ad, unsigned int code, int price, int rooms, DATE date, time_t database_Entry_Date);

void printList(FILE* f_ptr, APT_LIST* lst);

void printOnlyCodes(APT_LIST* lst);

void copyList(APT_LIST* target, APT_LIST* source);

void removeNode(APT_LIST* apt, APT* node);

APT** listToArr(APT_LIST* lst);

void arrToList(APT** arr, APT_LIST* lst);

void freeList(APT_LIST* lst);

void freeListRec(APT* node);

void addToHead(APT_LIST* lst, APT* node);

void addToTail(APT_LIST* lst, APT* node);

uint nextPos(uint i);

void deleteNode(APT* node);

void addToHeadStockList(STOCK* stock, STOCK_NODE* node);

void addToTailStockList(STOCK* stock, STOCK_NODE* node);

#endif