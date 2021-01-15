/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 20/10/2020

	Final Project
	Main file
*/

#define ___DEBUG_APARTMENTS_LIST___

#define ___PRINTING_FILE_DEBUGGING___

#include "Structs_And_Consts.h"
#include "Functions.h"
void main() {


	FILE* tf_ptr = fopen("Printing_Tests.txt", "w");
	puts(" Please enter one of the following commands:\n"
		" add - apt, find - apt, buy - apt, delete - apt or exit\n"
		" For reconstruction commands, please enter :\n"
		" !!, !num, history, short_history or !num ^ str1 ^ str2"
		"\n_____________________________________________________\n");

	fputs(" Please enter one of the following commands:\n"
		" add - apt, find - apt, buy - apt, delete - apt or exit\n"
		" For reconstruction commands, please enter :\n"
		" !!, !num, history, short_history or !num ^ str1 ^ str2"
		"\n_____________________________________________________\n", tf_ptr);

	STOCK stock;
	makeEmptyStockList(&stock);
	APT_LIST aptList;
	makeEmptyAptList(&aptList);
	FILE* fT_ptr = NULL;
	FILE* fB_ptr = NULL;
	openAndFill(fT_ptr, fB_ptr, &stock, &aptList);

	getCommand(tf_ptr ,&aptList, &stock);

#ifndef ___DEBUG_APARTMENTS_LIST___
#define ___DEBUG_APARTMENTS_LIST___
	printList(&aptList);
#endif

#ifndef ___PRINTING_FILE_DEBUGGING___
#define ___PRINTING_FILE_DEBUGGING___
	printListToFile(tf_ptr, &aptList);
#endif

	writeCommands(&stock);
	writeApts(&aptList);

	puts("Good Bye!");
	fclose(tf_ptr);
}




