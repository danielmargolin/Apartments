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

		addToStock(stock, command);
		interpretation(aptList, stock, command);
		gets(command);
	}
	writeCommands(stock);
	writeApts(aptList);
}

void addToStock(STOCK* stock, char* command) {


	uint i, pos;
	if (!short_term_history[N - 1]) {


		pos = nextPos();
		if (pos) {

			short_term_history[pos] = (char*)malloc(sizeof(char) * strlen(short_term_history[pos - 1]));
			for (i = pos; i > 0; i--) {

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
		stock->size++;
	}

}

void interpretation(APT_LIST* aptList, STOCK* stock, char* command) {

	switch (command[0]) {

	case 'f':
		findApt(aptList, command);
		break;
	case 'a':
		addApt(aptList, &command[strlen(ADD) + 1]);
		break;
	case 'b':
		buy(aptList, &command[strlen(BUY) + 1]);
		break;
	case 'd':
		deleteApt(aptList, &command[strlen(DELETE) + 1]);
		break;
	default:
		other(aptList, stock, command);
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
			removeNode(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}
	}
	apt.size = count;
	if (params.sortType)
		sortList(&apt, "Price", params.sortType);
	return apt;
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
			removeNode(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}

	}
	apt.size = count;
	if (params.sortType)
		sortList(&apt, "Price", params.sortType);

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
			removeNode(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}
	}

	apt.size = count;
	if (params.sortType)
		sortList(&apt, "Rooms", params.sortType);

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
			removeNode(&apt, temp);
		}
		else {

			cur = cur->next;
			count++;
		}
	}
	apt.size = count;

	if (params.sortType && apt.size > 1)
		sortList(&apt, "Rooms", params.sortType);

	return apt;
}

void sortList(APT_LIST* apt, char* type, char* order) {

	uint i;
	APT* cur = apt->head;
	APT** arr;
	arr = listToArr(apt);

	if (strcmp(type, "Price") == 0)
		qsort(arr, apt->size, sizeof(APT*), &sortByPrice);
	else if (strcmp(type, "Rooms") == 0)
		qsort(arr, apt->size, sizeof(APT*), &sortByPrice);

	if (strcmp(order, "sr") == 0)
		reverseArrToList(arr, apt);
	else
		arrToList(arr, apt);
}

APT_LIST findLastDays(APT_LIST apt, FIND_PARAMS params) {
	int numOfDays = params.data;

	return apt;
}

FIND_FUNCTION* getFindFunctions(char* command, int* size, FIND_PARAMS* params) {

	FIND_FUNCTION* findFunctions = malloc(10 * sizeof(FIND_FUNCTION));
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

	int size = 0;
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
	addToHead(aptList, makeApt(address, aptList->size, price, rooms, date, now));
}

void buy(APT_LIST* aptList, char* command) {

	APT* cur = aptList->head;
	uint code = (uint)atoi(command), i;

	for (i = 0; i < aptList->size && cur; i++) {

		if (cur->code == code)
			removeNode(aptList, cur);
	}
	aptList->size--;
}

void deleteApt(APT_LIST* aptList, char* command) {

	uint days = atoi(command), i;
	APT* cur = aptList->tail;
	APT_LIST lst_To_Delete;
	makeEmptyAptList(&lst_To_Delete);
	lst_To_Delete.head = aptList->head;
	time_t now;
	time(&now);
	int dif;
	uint count = 0;
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

void printShortHistory() {

	int i;
	for (i = N - 1; i >= 0; i--) {

		printf("%d -> %s\n", i + 1, short_term_history[i]);
	}
}

void printHistory(STOCK* stock) {

	int i;
	STOCK_NODE* cur = stock->tail;
	for (i = N + stock->size - 1; i >= 0; i--) {

		if (cur) {

			printf("%d -> %s\n", i + 1, cur->command);
			cur = cur->prev;
		}
		else
			printf("%d -> %s\n", i + 1, short_term_history[i]);
	}
}


void other(APT_LIST* aptList, STOCK* stock, char* command) {

	if (strcmp(command, "short_history") == 0)
		printShortHistory();
	else if (strcmp(command, "history") == 0)
		printHistory(stock);
	else if (strlen(command) <= 2)
		otherCommands(aptList, stock, command);
	else
		changePastCommands(aptList, stock, command);


}

void changePastCommands(APT_LIST* aptList, STOCK* stock, char* command) {


}

void otherCommands(APT_LIST* aptList, STOCK* stock, char* command) {

	if (strcmp(command, "!!") == 0) {

		short_term_history[0] = (char*)realloc(short_term_history[0], strlen(short_term_history[1]));
		strcpy(short_term_history[0], short_term_history[1]);
		interpretation(aptList, stock, short_term_history[0]);
		return;
	}
	uint place = command[1] - '0';
	if (place < N) {

		short_term_history[0] = (char*)realloc(short_term_history[0], strlen(short_term_history[place]));
		strcpy(short_term_history[0], short_term_history[place]);
	}	
	else {

		int i;
		STOCK_NODE* cur = stock->head;
		for (i = 0; i < place; i++) {

			cur = cur->next;
		}
		short_term_history[0] = (char*)realloc(short_term_history[0], strlen(cur->command));
		strcpy(short_term_history[0], cur->command);
		
	}
	interpretation(aptList, stock, short_term_history[0]);
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

int sortByPrice(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->price;
	const int b = (int)(*seg2)->price;
	return a - b;
}

int sortByRooms(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->rooms;
	const int b = (int)(*seg2)->rooms;
	return a - b;
}