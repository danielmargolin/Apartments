/*
	Name:	Guy Shaked
			Yuval Hazan

	ID:		205717952
			205487515

	Date: 20/10/2020

	Final Project
	Main file
*/
#include "Structs_And_Consts.h"
#include "Functions.h"


void main() {


/*	
		puts("Please enter one of the following commands:\n"
		"add - apt, find - apt, buy - apt, delete - apt or exit\n"
		"For reconstruction commands, please enter :\n"
		"!!, !num, history, short_history or !num ^ str1 ^ str2\n");

	*/

	STOCK stock;
	makeEmptyStockList(&stock);
	APT_LIST aptList;
	makeEmptyAptList(&aptList);
	FILE* fT_ptr = fT_ptr = fopen(TEXT_FILE_NAME, "r");
	FILE *fB_ptr = fB_ptr = fopen(APTS_FILE_NAME, "rb");
	openAndFill(fT_ptr, fB_ptr, &stock, &aptList);

	getCommand(&aptList, &stock);

	printList(&aptList);

}










