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
#include "Lists_Module.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




#ifndef ___FILES_MODULE_H___
#define ___FILES_MODULE_H___


/*************** Functions Prototypes ****************/

FILE* openFile();

STOCK fillStocks(FILE* f_ptr);

STOCK_NODE* makeStockNode(char* command);

void writeCommands(STOCK* stock);

void writeApts(APT_LIST* lst);

APT_LIST fillApts(FILE* fB_ptr);

void openAndFill(FILE* fT_ptr, FILE* fB_ptr, STOCK* stock, APT_LIST* aptList);


#endif

