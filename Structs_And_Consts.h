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

#ifndef ___STRUCTS_AND_CONSTANTS_H___
#define ___STRUCTS_AND_CONSTANTS_H___

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*************** Macros ****************/


/*************** Constants ****************/

#define N 7

char* short_term_history[N];

typedef unsigned int uint;

typedef short int sint;

typedef unsigned char uchar;

#define TRUE 1

#define FALSE 0

#define ADDRESS 100

#define COMMAND 100

#define DAYS_TO_SECONDS (24 * 60 * 60)

#define FIND "find-apt"

#define ADD "add-apt"

#define BUY "buy-apt"

#define DELETE "delete-apt –Enter"

#define DELIMETERS " -\n\t"

#define ADDRESS_DELIMETERS "\""

#define TEXT_FILE_NAME "Commands_Text_File.txt"

#define APTS_FILE_NAME "Apartments_Binary_File.bin"

#define MSB_SET(TYPE) ((TYPE)(1)<<(sizeof(TYPE)*8-1))

#define MASK_WITH_N_LSB_SET(TYPE,N) ((((TYPE)1) << N) - 1)

/*************** Structs ****************/

typedef struct _date {

	sint day;
	sint month;
	sint year;

} DATE;

typedef struct _apartment {

	struct _apartment* prev;
	uint code;
	char* address;
	int price;
	sint rooms;
	DATE date;
	time_t database_Entry_Date;
	struct _apartment* next;

} APT;

typedef struct _apartmentsList {

	APT* head;
	APT* tail;
	uint size;

} APT_LIST;

typedef struct _stockNode {

	struct _stockNode* prev;
	char* command;
	struct _stockNode* next;


} STOCK_NODE;

typedef struct _stock {

	STOCK_NODE* head;
	STOCK_NODE* tail;
	uint size;

} STOCK;

typedef APT_LIST(*FIND_FUNCTION)(APT_LIST, int);

typedef struct find_function_node {
	FIND_FUNCTION* function;
	int param;
	struct find_function_node* next;
} FIND_FUNCTION_NODE;

typedef struct find_functions_list {
	FIND_FUNCTION_NODE* head;
	uint size;
} FIND_FUNCTIONS_LIST;


#endif

