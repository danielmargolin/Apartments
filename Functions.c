/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 20/10/2020

	Final Project
	Main file
*/
#include "Functions.h"

void getCommand(APT_LIST* aptList, STOCK* stock) {

	char command[COMMAND];
	gets(command);
	while (strcoll("exit", command)) {

		addToStock(stock, command, stock->size);
		interpretation(aptList, command);
		gets(command);
		stock->size++;
	}
	writeCommands(stock);
	writeApts(aptList);
}

void addToStock(STOCK* stock, char* command, uint add) {

	uint i;

	if (add < N) {

		if (add) {

			short_term_history[add] = (char*)malloc(sizeof(char) * strlen(short_term_history[add - 1]));
			for (i = add; i > 0; i--) {

				short_term_history[i] = (char*)realloc(short_term_history[i], strlen(short_term_history[i - 1]));
				strcpy(short_term_history[i], short_term_history[i - 1]);
			}
			short_term_history[0] = (char*)realloc(short_term_history[0], strlen(command));
		}
		else
			short_term_history[0] = (char*)malloc(sizeof(char) * strlen(command));

		strcpy(short_term_history[0], command);
	}
	else {

		addToHeadStockList(stock, makeStockNode(short_term_history[N - 1]));
		for (i = N - 1; i > 0; i--) {

			short_term_history[i] = (char*)realloc(short_term_history[i], strlen(short_term_history[i - 1]));
			strcpy(short_term_history[i], short_term_history[i - 1]);
		}
		short_term_history[0] = (char*)realloc(short_term_history[0], strlen(command));
		strcpy(short_term_history[0], command);
	}
}

void interpretation(APT_LIST* aptList, char* command) {

	switch (command[0]) {

	case 'f':
		findApt(aptList, command);
		break;
	case 'a':
		addApt(aptList, &command[strlen(ADD) + 1]);
		break;
	case 'b':
		buy(&command[strlen(BUY) + 1]);
		break;
	case 'd':
		deleteApt(aptList, &command[strlen(DELETE) + 1]);
		break;
	default:
		other(command);
		break;
	}
}

APT_LIST findMaxPrice(APT_LIST apt, FIND_PARAMS params) {
	int price = params.data;

	APT* cur = apt.head;
	APT* temp;
	uint i, count = 0;
	for (i = 0; i < apt.size && cur; i++) {

		if (cur->price > price) {

			temp = cur;
			cur = cur->next;
			aptOut(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}

	}
	apt.size = count;
	return apt;
}

void aptOut(APT_LIST* apt, APT* node) {

	if (node->next && node->prev) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	else if (node->next) {

		apt->head = node->next;
		apt->head->prev = NULL;
	}
	else if (node->prev) {

		apt->tail = node->prev;
		apt->tail->next = NULL;
	}
	else
		apt->head = apt->tail = NULL;

	free(node);
}

APT_LIST findMinPrice(APT_LIST apt, FIND_PARAMS params) {
	int price = params.data;

	APT* cur = apt.head;
	APT* temp;
	uint i, count = 0;
	for (i = 0; i < apt.size && cur; i++) {

		if (cur->price < price) {

			temp = cur;
			cur = cur->next;
			aptOut(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}

	}
	apt.size = count;
	return apt;
}

APT_LIST findDate(APT_LIST apt, FIND_PARAMS params) {
	int date = params.data;
	return apt;
}

APT_LIST findMaxRooms(APT_LIST apt, FIND_PARAMS params) {
	int numOfRooms = params.data;

	APT* cur = apt.head;
	APT* temp;
	uint i, count = 0;
	for (i = 0; i < apt.size && cur; i++) {

		if (cur->rooms > numOfRooms) {

			temp = cur;
			cur = cur->next;
			aptOut(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}
	}

	apt.size = count;
	return apt;
}

APT_LIST findMinRooms(APT_LIST apt, FIND_PARAMS params) {
	int numOfRooms = params.data;

	APT* cur = apt.head;
	APT* temp;
	uint i, count = 0;
	for (i = 0; i < apt.size && cur; i++) {

		if (cur->rooms < numOfRooms) {

			temp = cur;
			cur = cur->next;
			aptOut(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}
	}
	apt.size = count;
	return apt;
}

APT_LIST findLastDays(APT_LIST apt, FIND_PARAMS params) {
	int numOfDays = params.data;

	return apt;
}

FIND_FUNCTION* getFindFunctions(char* command, int* size, FIND_PARAMS* params) {

	FIND_FUNCTION* findFunctions = malloc(10 * sizeof(FIND_FUNCTION));
	(*size) = 0;
	char* currentWord = strtok(command, DELIMETERS);

	while (currentWord) {

		if (strcmp(currentWord, "MaxPrice") == 0) {
			findFunctions[(*size)] = &findMaxPrice;
			params[(*size)].data = atoi(strtok(NULL, DELIMETERS));
			(*size)++;
		}
		if (strcmp(currentWord, "MinPrice") == 0) {
			findFunctions[(*size)] = &findMinPrice;
			params[(*size)].data = atoi(strtok(NULL, DELIMETERS));
			(*size)++;
		}
		if (strcmp(currentWord, "MinNumRooms") == 0) {
			findFunctions[(*size)] = &findMinRooms;
			params[(*size)].data = atoi(strtok(NULL, DELIMETERS));
			(*size)++;
		}
		if (strcmp(currentWord, "MaxNumRooms") == 0) {
			findFunctions[(*size)] = &findMaxRooms;
			params[(*size)].data = atoi(strtok(NULL, DELIMETERS));
			(*size)++;
		}
		currentWord = strtok(NULL, DELIMETERS);
		if (currentWord && (strcmp(currentWord, "s") == 0 || strcmp(currentWord, "sr") == 0)) {
			params[(*size) - 1].sortType = currentWord;
			currentWord = strtok(NULL, DELIMETERS);
		}
		else {
			params[(*size) - 1].sortType = NULL;
		}
	}
	return findFunctions;
}

void findApt(APT_LIST* aptList, char* command) {

	int size;
	FIND_PARAMS params[10];

	APT_LIST(**findFunctions)(APT_LIST, FIND_PARAMS);
	findFunctions = getFindFunctions(command + 8, &size, &params);
	uint i;
	APT_LIST filtered_List;
	makeEmptyAptList(&filtered_List);
	copyList(&filtered_List, aptList);
	for (int i = 0; i < size; i++) {

		filtered_List = (*findFunctions[i])(filtered_List, params[i]);
	}
	printList(&filtered_List);
}

void addApt(APT_LIST* aptList, char* command) {

	aptList->size++;
	char address[ADDRESS];
	int price;
	uint count = 0, indexCommand = 0, indexAddress = 0;
	while (count < 2) {

		if (command[indexCommand] != 34) {

			address[indexAddress] = command[indexCommand];
			indexAddress++;
		}
		else
			count++;
		indexCommand++;
	}
	address[indexAddress] = '\0';

	price = atoi(strtok(&command[indexCommand], DELIMETERS));
	int rooms = atoi(strtok(NULL, DELIMETERS));
	char* d = strtok(NULL, "\0");
	DATE date = makeDate(d);
	time_t now;
	time(&now);
	addToList(aptList, makeApt(address, aptList->size, price, rooms, date, now));
}

void buy() {



}

void deleteApt(APT_LIST* aptList, char* command) {

	uint days = atoi(command), i;
	APT* cur = aptList->tail;
	APT_LIST lst_To_Delete;
	lst_To_Delete.head = aptList->head;
	time_t now;
	time(&now);
	int dif;
	uint count = 0;
	// 
	for (i = 0; i < aptList->size && cur; i++) {

		dif = (difftime(now, cur->database_Entry_Date)) / 60 / 60 / 24;
		if (dif <= days) {

			lst_To_Delete.tail = cur;
			if (cur->next) {

				aptList->head = cur->next;
				cur->next->prev = NULL;
				cur->next = NULL;
			}
			deleteList(&lst_To_Delete);
			if (!count)
				makeEmptyAptList(aptList);
			else
				aptList->size = count;
		}
		count++;
		cur = cur->prev;
	}
}

void deleteList(APT_LIST* lst) {

	if (lst->head)
		deleteListRec(lst->head);
}

void deleteListRec(APT* node) {

	if (node->next)
		deleteListRec(node->next);
	free(node);
}



void other() {


}

DATE makeDate(char* d) {

	DATE date;
	date.day = atoi(strtok(d, DELIMETERS));
	date.month = atoi(strtok(NULL, DELIMETERS));
	date.year = atoi(strtok(NULL, DELIMETERS));
	return date;
}

void allocationCheck(void* x) {

	if (!x) {

		printf("ALLOCATION ERROR");
		exit(1);
	}
}

void binaryPrint(uchar n) {

	uint i;
	uchar mask = MSB_SET(uchar);
	mask >>= 8;
	for (i = 0; i < sizeof(uchar) * 8; i++) {

		if (mask & n)
			putchar('1');
		else
			putchar('0');
		mask >>= 1;
	}
	putchar('\n');
}