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

/*************** Static Functions Prototypes ****************/

static void queuePush(uint position, char* command);
static char* inputCommand();
static void printHistory(STOCK* stock);
static void printShortHistory();
static void printOnlyCodes(APT_LIST* lst);
static void replaceLastCommand(STOCK* stock, char* command);
static char* replaceWord(const char* s, const char* oldW, const char* newW);
static void interpretation(APT_LIST* aptList, STOCK* stock, char* command);
static void changePastCommands(APT_LIST* aptList, STOCK* stock, char* command);
static void lastCommand(APT_LIST* aptList, STOCK* stock, char* command);
static void replaceFirstCommand(char* command);
static int sortByPrice(const void* element1, const void* element2);
static int sortByRooms(const void* element1, const void* element2);
static int sortByDate(const void* element1, const void* element2);
static void reallocationFindFuncs(FIND_FUNCTION** findFunctions, int* params, int size);

/*************** Public Functions ****************/

void getCommand(APT_LIST* aptList, STOCK* stock) {

	char* command = inputCommand();
	while (strcoll("exit", command)) {

		addToStock(stock, command);
		interpretation(aptList, stock, command);
		command = inputCommand(command);
	}
	free(command);
}

void addToStock(STOCK* stock, char* command) {


	uint i, pos;
	if (!short_term_history[N - 1]) {

		pos = nextPos(0);
		if (pos)
			queuePush(pos, command);
		else
			short_term_history[0] = (char*)calloc(strlen(command), sizeof(char));

		strcpy(short_term_history[0], command);
	}
	else {

		addToHeadStockList(stock, makeStockNode(short_term_history[N - 1]));
		queuePush(N - 1, command);
		strcpy(short_term_history[0], command);
		stock->size++;
	}
}

APT_LIST findMaxPrice(APT_LIST apt, int param) {

	int price = param;

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
	return apt;
}

APT_LIST findMinPrice(APT_LIST apt, int param) {

	int price = param;
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
	return apt;
}

APT_LIST findDate(APT_LIST apt, int param) {

	int dateAsNumber = param;
	int year = dateAsNumber % 10000;
	dateAsNumber = dateAsNumber / 10000;
	int month = dateAsNumber % 100;
	dateAsNumber = dateAsNumber / 100;
	int day = dateAsNumber % 100;
	
	APT* cur = apt.head;
	APT* temp;
	uint i, count = 0;
	for (i = 0; i < apt.size && cur; i++) {

		if (cur->date.year > year || (cur->date.year == year && cur->date.month > month) 
			|| (cur->date.year == year && cur->date.month == month && cur->date.day > day)) {

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
	return apt;
}

APT_LIST findMaxRooms(APT_LIST apt, int param) {

	int numOfRooms = param;
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
	return apt;
}

APT_LIST findMinRooms(APT_LIST apt, int param) {

	int numOfRooms = param;
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

	return apt;
}

void sortList(APT_LIST* apt, char* type, char order) {

	uint i;
	APT* cur = apt->head;
	APT** arr;
	arr = listToArr(apt);

	if (strcmp(type, "Price") == 0)
		qsort(arr, apt->size, sizeof(APT*), &sortByPrice);
	else if (strcmp(type, "Date") == 0)
		qsort(arr, apt->size, sizeof(APT*), &sortByDate);
	else if (strcmp(type, "Rooms") == 0)
		qsort(arr, apt->size, sizeof(APT*), &sortByRooms);

	if (order == 'r')
		reverseArrToList(arr, apt);
	else
		arrToList(arr, apt);
}

APT_LIST findLastDays(APT_LIST apt, int param) {

	if (!param)
		return apt;
	int numOfDays = param;
	time_t now;
	time(&now);
	uint count = 0, i;
	APT* cur = apt.head;

	while (cur && difftime(now, cur->database_Entry_Date) <= param) {

		count++;
		cur = cur->next;
	}

	if (!cur)
return apt;

apt.tail = cur;
apt.size = count;
if (apt.tail->next) {

	APT_LIST lstToDelete;
	lstToDelete.head = apt.tail->next;
	lstToDelete.tail = apt.tail;
	deleteList(&lstToDelete);
}
return apt;
}

FIND_FUNCTIONS_LIST* getFindFunctions(char* command, char* sort) {
	FIND_FUNCTIONS_LIST* functionsLst = (FIND_FUNCTIONS_LIST*)malloc(sizeof(FIND_FUNCTIONS_LIST));
	makeEmptyFindFunctionList(functionsLst);
	char* currentWord = strtok(command, DELIMETERS);

	while (currentWord) {

		if (strcmp(currentWord, "MaxPrice") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMaxPrice, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "MinPrice") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMinPrice, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "MinNumRooms") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMinRooms, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "MaxNumRooms") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMaxRooms, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "Enter") == 0) {
			char* temp = strtok(NULL, DELIMETERS);
			int param;
			if (temp)
				param = DAYS_TO_SECONDS * atoi(temp);
			else
				param = NULL;
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findLastDays, param);
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "Date") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findDate, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(functionsLst, node);
		}
		else if (strcmp(currentWord, "sr") == 0)
			(*sort) = 'r';

		else if (strcmp(currentWord, "s") == 0)
			(*sort) = 's';

		currentWord = strtok(NULL, DELIMETERS);
	}
	return functionsLst;
}

static void reallocationFindFuncs(FIND_FUNCTION** findFunctions, int* params, int size) {

	puts("start");
	findFunctions = (FIND_FUNCTION**)realloc(findFunctions, size * 2 + 1);
	allocationCheck(findFunctions);
	params = (int*)realloc(params, size * 2 + 1);
	allocationCheck(params);
	puts("end");

}

APT_LIST executeFindFunctions(FIND_FUNCTIONS_LIST* lst, APT_LIST* aptList) {
	APT_LIST filtered_List;
	makeEmptyAptList(&filtered_List);
	copyList(&filtered_List, aptList);
	FIND_FUNCTION_NODE* head = lst->head;
	while (head != NULL) {
		APT_LIST(*findFunction)(APT_LIST, int*);
		findFunction = head->function;
		filtered_List = (*findFunction)(filtered_List, head->param);
		head = head->next;
	}
	return filtered_List;
}

void findApt(APT_LIST* aptList, char* command) {

	char sort = '\0';
	APT_LIST(**findFunctions)(APT_LIST, int*);

	FIND_FUNCTIONS_LIST* functionsLst;
	functionsLst = getFindFunctions(command + 8, &sort);
	APT_LIST filtered_List;
	filtered_List = executeFindFunctions(functionsLst, aptList);
	if (sort)
		sortList(&filtered_List, "Price", sort);

	if (functionsLst->size == 1) {
		APT_LIST(*findFunction)(APT_LIST, int*);
		findFunction = functionsLst->head->function;
		if (*findFunction == &findLastDays) {
			printOnlyCodes(&filtered_List);
		}
		else {
			printList(&filtered_List);
		}
	}
	else
		printList(&filtered_List);
	freeFunctionList(&functionsLst);
}

void addApt(APT_LIST* aptList, char* command) {

	aptList->size++;
	char* temp = (char*)calloc(strlen(command), sizeof(char));
	strcpy(temp, command);
	strtok(temp, ADDRESS_DELIMETERS);
	char* address = strtok(NULL, ADDRESS_DELIMETERS);
	int price;
	price = atoi(strtok(NULL, DELIMETERS));
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

		if (cur->code == code) {

			removeNode(aptList, cur);
			aptList->size--;
			return;
		}
		cur = cur->next;
	}
	if (!cur)
		printf("The apartment was not found\n");
}

void deleteApt(APT_LIST* aptList, char* command) {

	uint days = (DAYS_TO_SECONDS * atoi(command)), i;
	APT* cur = aptList->tail;
	APT_LIST lst_To_Delete;
	makeEmptyAptList(&lst_To_Delete);
	lst_To_Delete.head = aptList->head;
	time_t now;
	time(&now);
	uint count = 0;
	for (i = 0; i < aptList->size && cur; i++) {

		if (difftime(now, cur->database_Entry_Date) <= days) {

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

void other(APT_LIST* aptList, STOCK* stock, char* command) {

	if (strcmp(command, "short_history") == 0)
		printShortHistory();
	else if (strcmp(command, "history") == 0)
		printHistory(stock);
	else if (strcmp(command, "!!") == 0)
		lastCommand(aptList, stock, command);
	else
		changePastCommands(aptList, stock, command);
}

DATE makeDate(char* d) {

	DATE date;
	date.day = atoi(strtok(d, DELIMETERS));
	date.month = atoi(strtok(NULL, DELIMETERS));
	date.year = 2000 + atoi(strtok(NULL, DELIMETERS));
	return date;
}

void allocationCheck(void* x) {

	if (!x) {

		printf("ALLOCATION ERROR");
		exit(1);
	}
}

/*************** Static Functions ****************/

static void interpretation(APT_LIST* aptList, STOCK* stock, char* command) {

	switch (command[0]) {

	case 'f':
		findApt(aptList, command);
		break;
	case 'a':
		addApt(aptList, command);
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

static char* inputCommand() {

	char* command = (char*)malloc(sizeof(char));
	int i = 0;
	do {

		if (!command[i]);
		command = (char*)realloc(command, i * 2 + 1);
		command[i] = getchar();

	} while (command[i++] != '\n');
	command = (char*)realloc(command, i);
	command[i - 1] = '\0';
	return command;
}

static void queuePush(uint position, char* command) {

	uint i;
	short_term_history[position] = (char*)malloc(0);
	for (i = position; i > 0; i--) {

		short_term_history[i] = (char*)realloc(short_term_history[i], strlen(short_term_history[i - 1]));
		short_term_history[i][strlen(short_term_history[i - 1])] = '\0';
		strcpy(short_term_history[i], short_term_history[i - 1]);
	}
	short_term_history[0] = (char*)realloc(short_term_history[0], strlen(command));
	short_term_history[0][strlen(command)] = '\0';
}

static void printShortHistory() {

	int i;
	for (i = N - 1; i >= 0; i--)
		printf("%d -> %s\n", i, short_term_history[i]);

}

static void printHistory(STOCK* stock) {

	int i;
	STOCK_NODE* cur = stock->tail;
	for (i = N + stock->size - 1; i >= 0; i--) {

		if (cur) {

			printf("%d -> %s\n", i, cur->command);
			cur = cur->prev;
		}
		else
			printf("%d -> %s\n", i, short_term_history[i]);
	}
}

static void printOnlyCodes(APT_LIST* lst) {

	if (!lst->size) {

		puts("EMPTY");
		return;
	}
	else {

		APT* cur = lst->head;
		uint i;
		for (i = 0; i < lst->size && cur; i++) {

			printf("%d\n", cur->code);
			cur = cur->next;
		}
	}
}

static void replaceLastCommand(STOCK* stock, char* command) {

	int place = atoi(command + 1);
	if (place < N - 1) {
		replaceFirstCommand(short_term_history[place + 1]);
		return;
	}

	int i;
	STOCK_NODE* cur = stock->head;
	for (i = 0; (i < place - N) && cur; i++)
		cur = cur->next;
	replaceFirstCommand(cur->command);
}

static char* replaceWord(const char* s, const char* oldW, const char* newW) {

	char* result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	for (i = 0; i < strlen(s) && s[i]; i++) {

		if (strstr(&s[i], oldW) == &s[i]) {

			cnt++;
			i += oldWlen - 1;
		}
	}
	result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s) {
		if (strstr(s, oldW) == s) {

			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}
	result[i] = '\0';
	return result;
}

static void replaceFirstCommand(char* command) {

	short_term_history[0] = (char*)realloc(short_term_history[0], strlen(command));
	allocationCheck(short_term_history[0]);
	short_term_history[0][strlen(command)] = '\0';
	strcpy(short_term_history[0], command);
}

static void lastCommand(APT_LIST* aptList, STOCK* stock, char* command) {

	short_term_history[0] = (char*)realloc(short_term_history[0], strlen(short_term_history[1]));
	strcpy(short_term_history[0], short_term_history[1]);
	interpretation(aptList, stock, short_term_history[0]);
}

static void changePastCommands(APT_LIST* aptList, STOCK* stock, char* command) {

	replaceLastCommand(stock, command);
	strtok(command, "^");
	char* strToReplace = strtok(NULL, "^");
	if (strToReplace) {

		char* strToReplaceWith = strtok(NULL, "^");
		char* replicationOfCommand = replaceWord(short_term_history[0], strToReplace, strToReplaceWith);
		short_term_history[0] = (char*)realloc(short_term_history[0], strlen(replicationOfCommand));
		strcpy(short_term_history[0], replicationOfCommand);
		interpretation(aptList, stock, short_term_history[0]);
	}
	else
		interpretation(aptList, stock, short_term_history[0]);
}

static int sortByPrice(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->price;
	const int b = (int)(*seg2)->price;
	return a - b;
}

static int sortByRooms(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->rooms;
	const int b = (int)(*seg2)->rooms;
	return a - b;
}

static int sortByDate(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const DATE a = (*seg1)->date;
	const DATE b = (*seg2)->date;

	if (a.year == b.year) {
		if (a.month == b.month) {
			return a.day - b.day;
		}
		else {
			return a.month - b.month;
		}
	}
	else {
		return a.year - b.year;
	}
}