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


void addToHeadStockList(STOCK* stock, STOCK_NODE* node);

void addToTailStockList(STOCK* stock, STOCK_NODE* node);

void makeEmptyStockList(STOCK* lst);

void deletehead(STOCK* lst);

void makeEmptyAptList(APT_LIST* lst);

void addToList(APT_LIST* lst, APT* node);

APT* makeApt(char* ad, unsigned int code, int price, int rooms, DATE date, time_t database_Entry_Date);

void printApt(APT* apt);

void printList(APT_LIST* lst);

#endif

