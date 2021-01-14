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
static void replaceLastCommand(STOCK* stock, char* command);
static char* replaceWord(const char* s, const char* oldW, const char* newW);
static void interpretation(APT_LIST* aptList, STOCK* stock, char* command);
static void changePastCommands(APT_LIST* aptList, STOCK* stock, char* command);
static void lastCommand(APT_LIST* aptList, STOCK* stock, char* command);
static void replaceFirstCommand(char* command);
static int sortByPrice(const void* element1, const void* element2);
static int sortByRooms(const void* element1, const void* element2);
static int sortByDate(const void* element1, const void* element2);
static int reverseSortByDate(const void* element1, const void* element2);
static int reverseSortByRooms(const void* element1, const void* element2);
static int reverseSortByPrice(const void* element1, const void* element2);
static uint checkDate(APT* cur, int day, int month, int year);
static int sortByDataBase(const void* element1, const void* element2);
static double secondsSinceMidnight(int days);
static void splitAndDelete(APT_LIST* lst, APT_LIST* lstToDelete, APT* cur);

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

	sortList(&apt, "Price", 'r');
	if (param >= apt.head->price)
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if (param < apt.tail->price) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint count = 0;
	while (cur && cur->price > param) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

APT_LIST findMinPrice(APT_LIST apt, int param) {

	sortList(&apt, "Price", 's');
	if (param <= apt.head->price)
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if (param > apt.tail->price) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint count = 0;
	while (cur && cur->price < param) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

APT_LIST findDate(APT_LIST apt, int param) {

	int year = param % 10000;
	param = param / 10000;
	int month = param % 100;
	param = param / 100;
	int day = param % 100;
	sortList(&apt, "Date", 'r');

	if (!checkDate(apt.head, day, month, year))
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if (checkDate(apt.tail, day, month, year)) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint i, count = 0;
	while (cur && checkDate(cur, day, month, year)) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

static uint checkDate(APT* cur, int day, int month, int year) {

	if (cur->date.year > year || (cur->date.year == year && cur->date.month > month)
		|| (cur->date.year == year && cur->date.month == month && cur->date.day > day))
		return TRUE;
	return FALSE;
}

APT_LIST findMaxRooms(APT_LIST apt, int param) {

	sortList(&apt, "Rooms", 'r');
	if (param >= apt.head->rooms)
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if (param < apt.tail->rooms) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint count = 0;
	while (cur && cur->rooms > param) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

APT_LIST findMinRooms(APT_LIST apt, int param) {

	sortList(&apt, "Rooms", 's');
	if (param <= apt.head->rooms)
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if (param > apt.tail->rooms) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint count = 0;
	while (cur && cur->rooms < param) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

void sortList(APT_LIST* apt, char* type, char order) {

	uint i;
	APT* cur = apt->head;
	APT** arr;
	arr = listToArr(apt);

	if (strcmp(type, "Price") == 0) {

		if (order == 'r')
			qsort(arr, apt->size, sizeof(APT*), &reverseSortByPrice);
		else
			qsort(arr, apt->size, sizeof(APT*), &sortByPrice);
	}
	else if (strcmp(type, "Date") == 0) {

		if (order == 'r')
			qsort(arr, apt->size, sizeof(APT*), &reverseSortByDate);
		else
			qsort(arr, apt->size, sizeof(APT*), &sortByDate);
	}
	else if (strcmp(type, "Rooms") == 0) {

		if (order == 'r')
			qsort(arr, apt->size, sizeof(APT*), &reverseSortByRooms);
		else
			qsort(arr, apt->size, sizeof(APT*), &sortByRooms);
	}
	else if (strcmp(type, "DataBaseDate") == 0)
			qsort(arr, apt->size, sizeof(APT*), &sortByDataBase);

	arrToList(arr, apt);
}

APT_LIST findLastDays(APT_LIST apt, int param) {

	if (param == -2)
		return apt;

	sortList(&apt, "DataBaseDate", 'r');
	time_t now;
	time(&now);
	double  daysInSeconds = secondsSinceMidnight(param);

	if (daysInSeconds >= difftime(now, apt.head->database_Entry_Date))
		return apt;
	apt.head->prev = apt.tail->next = NULL;
	if ((param == -1) || daysInSeconds < difftime(now, apt.tail->database_Entry_Date)) {

		freeList(&apt);
		makeEmptyAptList(&apt);
		return apt;
	}
	APT* cur = apt.head;
	APT_LIST aptToDelete;
	aptToDelete.head = apt.head;
	uint count = 0;
	while (cur && difftime(now, cur->database_Entry_Date) > daysInSeconds) {

		cur = cur->next;
		count++;
	}
	splitAndDelete(&apt, &aptToDelete, cur);
	apt.size = apt.size - count;
	return apt;
}

static void splitAndDelete(APT_LIST* lst, APT_LIST* lstToDelete, APT* cur) {

	lst->head = cur;
	lstToDelete->tail = cur->prev;
	lst->head->prev = NULL;
	lstToDelete->tail->next = NULL;
	freeList(lstToDelete);
}

FIND_FUNCTIONS_LIST getFindFunctions(char* command, char* sort) {

	FIND_FUNCTIONS_LIST functionsLst;
	makeEmptyFindFunctionList(&functionsLst);
	char* currentWord = strtok(command, DELIMETERS);

	while (currentWord) {

		if (strcmp(currentWord, "MaxPrice") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMaxPrice, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "MinPrice") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMinPrice, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "MinNumRooms") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMinRooms, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "MaxNumRooms") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findMaxRooms, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "Enter") == 0) {
			char* temp = strtok(NULL, DELIMETERS);
			int param;
			if (temp)
				param = atoi(temp) - 1;
			else
				param = -2;
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findLastDays, param);
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "Date") == 0) {
			FIND_FUNCTION_NODE* node = makeFindFunctionNode(&findDate, atoi(strtok(NULL, DELIMETERS)));
			addToFunctionList(&functionsLst, node);
		}
		else if (strcmp(currentWord, "sr") == 0)
			(*sort) = 'r';

		else if (strcmp(currentWord, "s") == 0)
			(*sort) = 's';

		currentWord = strtok(NULL, DELIMETERS);
	}
	return functionsLst;
}

APT_LIST executeFindFunctions(FIND_FUNCTIONS_LIST lst, APT_LIST* aptList) {

	APT_LIST filtered_List;
	makeEmptyAptList(&filtered_List);
	copyList(&filtered_List, aptList);

	FIND_FUNCTION_NODE* head = lst.head;

	while (head) {

		if (!filtered_List.head)
			return filtered_List;
		APT_LIST(*findFunction)(APT_LIST, int*);
		findFunction = head->function;
		filtered_List = (*findFunction)(filtered_List, head->param);
		head = head->next;
	}
	return filtered_List;
}

void findApt(APT_LIST* aptList, char* command) {

	char sort = '\0';
	FIND_FUNCTIONS_LIST functionsLst;
	functionsLst = getFindFunctions(command + 8, &sort);
	APT_LIST filtered_List;
	makeEmptyAptList(&filtered_List);
	filtered_List = executeFindFunctions(functionsLst, aptList);
	if (sort)
		sortList(&filtered_List, "Price", sort);

	filtered_List.head->prev = filtered_List.tail->next = NULL;
	if (functionsLst.size == 1 && functionsLst.head->function == &findLastDays && functionsLst.head->param != -2)
		printOnlyCodes(&filtered_List);
	else
		printList(&filtered_List);

	freeFunctionList(&functionsLst);
	freeList(&filtered_List);
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
			freeList(&lst_To_Delete);
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

static void replaceLastCommand(STOCK* stock, char* command) {

	int place = atoi(command);
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

	replaceLastCommand(stock, "0");
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

static int reverseSortByPrice(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->price;
	const int b = (int)(*seg2)->price;
	return b - a;
}

static int sortByRooms(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->rooms;
	const int b = (int)(*seg2)->rooms;
	return a - b;
}

static int reverseSortByRooms(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const int a = (int)(*seg1)->rooms;
	const int b = (int)(*seg2)->rooms;
	return b - a;
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

static int reverseSortByDate(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const DATE a = (*seg1)->date;
	const DATE b = (*seg2)->date;

	if (a.year == b.year) {
		if (a.month == b.month) {
			return b.day - a.day;
		}
		else {
			return b.month - a.month;
		}
	}
	else {
		return b.year - a.year;
	}
}

static int sortByDataBase(const void* element1, const void* element2) {

	APT** seg1 = element1;
	APT** seg2 = element2;
	const time_t a = (time_t)(*seg1)->database_Entry_Date;
	const time_t b = (time_t)(*seg2)->database_Entry_Date;
	double dif = difftime(a, b);
	if (dif > 0.0)
		return 1;
	if (dif < 0.0)
		return -1;
	return 0;
}

static double secondsSinceMidnight(int days) {


	int ret;
	struct tm info;
	time_t now;
	time(&now);
	struct tm* n;
	n = localtime(&now);
	info = (*n);
	info.tm_hour = info.tm_min = info.tm_sec = 0;
	if (info.tm_mday - days <= 0) {

		if (info.tm_mon < 0) {

			info.tm_year--;
			info.tm_mon = 11;
		}
		else {

			info.tm_mon--;
			info.tm_mday = 31 - (days - info.tm_mday);
		}
	}
	else
		info.tm_mday -= days;
	ret = mktime(&info);
	if (ret == -1) {

		puts("ERROR");
		exit(1);
	}
	time_t mid = mktime(&info);
	return difftime(now, mid);
}