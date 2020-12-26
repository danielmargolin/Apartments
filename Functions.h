/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 15/01/2020

	Final Project
	Structs and Constants h file
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Structs_And_Consts.h"
#include "Lists_Module.h"
#include "Files_Module.h"



#ifndef ___FUNCTIONS_H___
#define ___FUNCTIONS_H___

typedef APT_LIST* (*FIND_FUNCTION)(APT_LIST*, int);

/*************** Functions Prototypes ****************/

void getCommand(APT_LIST* aptList, STOCK* stock);

void interpretation(APT_LIST* aptList, char* command);

void findApt(APT_LIST* aptList, char* command);

APT_LIST* findMaxPrice(APT_LIST* apt, int price);

APT_LIST* findMinPrice(APT_LIST* apt, int price);

APT_LIST* findDate(APT_LIST* apt, int date);

APT_LIST* findMaxRooms(APT_LIST* apt, int numOfRooms);

APT_LIST* findMinRooms(APT_LIST* apt, int* numOfRooms);

APT_LIST* findLastDays(APT_LIST* apt, int* numOfDays);

FIND_FUNCTION* getFindFunctions(char* command, int* size, int** params);

APT_LIST* findAllApt(APT_LIST* apt, int* param);

void addApt(APT_LIST* aptList, char* command);

void buy();

void deleteApt(APT_LIST* aptList, char* command);

void other();

void allocationCheck(void* x);

DATE makeDate(char* d);

void addToStock(STOCK* stock, char* command, uint add);

void deleteList(APT_LIST* lst);

void deleteListRec(APT* node);

void binaryPrint(uchar n);

#endif

