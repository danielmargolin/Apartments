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

/*************** Functions Prototypes ****************/

void getCommand(APT_LIST* aptList, STOCK* stock);

void findApt(APT_LIST* aptList, char* command);

APT_LIST findMaxPrice(APT_LIST apt, int param);

APT_LIST findMinPrice(APT_LIST apt, int param);

APT_LIST findDate(APT_LIST apt, int param);

APT_LIST findMaxRooms(APT_LIST apt, int param);

APT_LIST findMinRooms(APT_LIST apt, int param);

APT_LIST findLastDays(APT_LIST apt, int param);

FIND_FUNCTION* getFindFunctions(char* command, int* size, int* params, char *sort);

void addApt(APT_LIST* aptList, char* command);

void buy(APT_LIST* aptList, char* command);

void deleteApt(APT_LIST* aptList, char* command);

void other(APT_LIST* aptList, STOCK* stock, char* command);

void allocationCheck(void* x);

DATE makeDate(char* d);

void addToStock(STOCK* stock, char* command);

void sortList(APT_LIST* apt, char* type, char order);

#endif

