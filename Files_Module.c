/*
	Name:	Guy Shaked
			Daniel Margolin

	ID:		205717952
			314899436

	Date: 20/10/2020

	Final Project
	Main file
*/
#include "Files_Module.h"

/*************** Static Functions Prototypes ****************/

static APT* readApt(FILE* fB_ptr);

static void castingBits(uchar roomsAndDate[3], unsigned short int* rooms, unsigned short int* day, unsigned short int* month,
	unsigned short int* year);

/*************** Public Functions ****************/

void openAndFill(FILE* fT_ptr, FILE* fB_ptr, STOCK* stock, APT_LIST* aptList) {

	fT_ptr = openTFile();
	fB_ptr = openBFile();
	(*aptList) = fillApts(fB_ptr);
	(*stock) = fillStocks(fT_ptr);
	fclose(fT_ptr);
	fclose(fB_ptr);
}

FILE* openTFile() {

	FILE* fT_ptr = fT_ptr = fopen(TEXT_FILE_NAME, "r");

	if (!fT_ptr) {

		fT_ptr = fopen(TEXT_FILE_NAME, "w");
		fprintf(fT_ptr, "%u", 0);
		fclose(fT_ptr);
		fT_ptr = fopen(TEXT_FILE_NAME, "r");
	}
	return fT_ptr;
}

FILE* openBFile() {

	FILE* fB_ptr = fB_ptr = fopen(APTS_FILE_NAME, "rb");

	if (!fB_ptr) {

		fB_ptr = fopen(APTS_FILE_NAME, "wb");
		unsigned int size = 0;
		fwrite(&size, sizeof(size), 1, fB_ptr);
		fclose(fB_ptr);
		fB_ptr = fopen(APTS_FILE_NAME, "rb");
	}
	return fB_ptr;
}

STOCK fillStocks(FILE* f_ptr) {

	STOCK stock;
	makeEmptyStockList(&stock);
	char command[COMMAND];
	int commandLength;
	uint j;
	fscanf(f_ptr, "%u", &stock.size);
	for (j = 0; (j < stock.size && !feof(f_ptr)); j++) {


		fscanf(f_ptr, "%d", &commandLength);
		fgets(command, commandLength + 1, f_ptr);
		if (j < N) {

			short_term_history[j] = (char*)calloc(commandLength, sizeof(char));
			strcpy(short_term_history[j], command);
		}
		else
			addToTailStockList(&stock, makeStockNode(command));
	}
	stock.size -= nextPos(0);
	return stock;
}

APT_LIST fillApts(FILE* fB_ptr) {

	APT_LIST lst;
	APT* apt;
	makeEmptyAptList(&lst);
	fread(&lst.size, 1, sizeof(lst.size), fB_ptr);
	uint i;
	for (i = 0; i < lst.size && !feof(fB_ptr); i++) {

		apt = readApt(fB_ptr);
		addToTail(&lst, apt);
	}
	return lst;
}

void writeCommands(STOCK* stock) {

	FILE* f_ptr = fopen(TEXT_FILE_NAME, "w");
	STOCK_NODE* cur = stock->head;
	uint num = nextPos(0);
	fprintf(f_ptr, "%u\n", num + stock->size);
	writeDataBaseA(f_ptr);
	writeDataBaseB(f_ptr,stock->head);

	fclose(f_ptr);
}

void writeDataBaseA(FILE *f_ptr) {

	uint i;
	for (i = 0; i < N; i++) {

		if (short_term_history[i])
			fprintf(f_ptr, "%d%s\n", strlen(short_term_history[i]), short_term_history[i]);
	}
}

void writeDataBaseB(FILE* f_ptr, STOCK_NODE* cur) {

	if (cur) {

		fprintf(f_ptr, "%d%s\n", strlen(cur->command), cur->command);
		writeDataBaseB(f_ptr, cur->next);
	}
}

void writeApts(APT_LIST* lst) {

	FILE* f_ptr = fopen(APTS_FILE_NAME, "wb");
	APT* cur = lst->head;
	fwrite(&lst->size, 1, sizeof(lst->size), f_ptr);
	uint i;
	sint addressLen;
	uchar roomsAndDate[3];
	for (i = 0; i < lst->size; i++) {

		cur->date.year = cur->date.year - 2000;
		roomsAndDate[0] = 0 | ((cur->rooms << 4) | (cur->date.day >> 1));
		roomsAndDate[1] = 0 | (((cur->date.day << 7) | (cur->date.month << 3) | (cur->date.year >> 4)));
		roomsAndDate[2] = 0 | (cur->date.year << 4);
		fwrite(&cur->code, 1, sizeof(cur->code), f_ptr);
		addressLen = strlen(cur->address);
		fwrite(&addressLen, 1, sizeof(addressLen), f_ptr);
		fwrite(cur->address, addressLen, sizeof(char), f_ptr);
		fwrite(&cur->price, 1, sizeof(cur->price), f_ptr);
		fwrite(&cur->database_Entry_Date, 1, sizeof(cur->database_Entry_Date), f_ptr);
		fwrite(roomsAndDate, 3, sizeof(uchar), f_ptr);
		cur = cur->next;
	}
	fclose(f_ptr);
}

void printListToFile(APT_LIST* lst) {

	FILE* f_ptr = fopen("Printing_Tests.txt", "w");
	if (!lst->size) {

		puts("EMPTY");
		return;
	}
	else {

		APT* cur = lst->head;
		uint i;
		for (i = 0; i < lst->size && cur; i++) {

			printAptToFile(f_ptr, cur);
			cur = cur->next;
		}
	}
	fprintf(f_ptr, "\nGood Bye!");
	fclose(f_ptr);
}

void printAptToFile(FILE* f_ptr, APT* apt) {

	
	struct tm* info;
	char buffer[80];
	info = localtime(&apt->database_Entry_Date);
	strftime(buffer, 80, "%x", info);
	uint day = atoi(strtok(buffer, "/"));
	uint month = atoi(strtok(NULL, "/"));
	uint year = 2000 + atoi(strtok(NULL, "/"));
	fprintf(f_ptr, "Apt details:\nCode: %d\nAddress: %s\nRooms: %d\nPrice: %d\nEntry date: %d.%d.%d\nDatabase entry date: %d.%d.%d\n",
		apt->code, apt->address, apt->rooms, apt->price, apt->date.day, apt->date.month, apt->date.year, day, month, year);
	//printf("\n*************\nAddress: %s\nPrice: %d\nCode: %d\nRooms: %d\nDate: %d.%d.%d\nDatabase entry date: %d.%d.%d",
	//	apt->address, apt->price, apt->code, apt->rooms, apt->date.day, apt->date.month, apt->date.year, day, month, year);
}


/*************** Static Functions ****************/

static APT* readApt(FILE* fB_ptr) {

	APT* apt;
	uint code;
	sint addressLen;
	char* address = NULL;
	uchar roomsAndDate[3];
	int price;
	unsigned short int rooms, day, month, year, mask;
	rooms = day = month = year = 0;
	time_t database_Entry_Date;
	DATE date;
	fread(&code, 1, sizeof(code), fB_ptr);
	fread(&addressLen, 1, sizeof(addressLen), fB_ptr);
	address = (char*)calloc(addressLen + 1, sizeof(char));
	fread(address, addressLen, sizeof(char), fB_ptr);
	address[addressLen] = '\0';
	fread(&price, 1, sizeof(price), fB_ptr);
	fread(&database_Entry_Date, 1, sizeof(database_Entry_Date), fB_ptr);
	fread(roomsAndDate, 1, sizeof(roomsAndDate), fB_ptr);
	castingBits(roomsAndDate, &rooms, &day, &month, &year);
	date.day = (sint)day;
	date.month = (sint)month;
	date.year = ((sint)year) + 2000;
	apt = makeApt(address, code, price, rooms, date, database_Entry_Date);
	free(address);
	return apt;
}

static void castingBits(uchar roomsAndDate[3], unsigned short int* rooms, unsigned short int* day, unsigned short int* month,
	unsigned short int* year) {

	unsigned short int mask = MASK_WITH_N_LSB_SET(sint, 4);
	(*rooms) = (0 | (roomsAndDate[0] >> 4));
	mask = MASK_WITH_N_LSB_SET(sint, 4);
	(*day) = (0 | (((mask & roomsAndDate[0]) << 1) | (1 & roomsAndDate[1])));
	(*month) = (0 | ((mask & (roomsAndDate[1] >> 3))));
	mask >>= 1;
	(*year) = (0 | (((mask & roomsAndDate[1]) << 4) | (roomsAndDate[2] >> 4)));
}






