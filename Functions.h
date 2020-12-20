/*
	Name:	Guy Shaked
			Yuval Hazan

	ID:		205717952
			205487515

	Date: 20/10/2020

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

void interpretation(APT_LIST* aptList, char* command);

void findApt(char* command);

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

