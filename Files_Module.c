/*
	Name:	Guy Shaked
			Daniel Mar

	ID:		205717952
			205487515

	Date: 20/10/2020

	Final Project
	Main file
*/
#include "Files_Module.h"


void binaryPrint1(unsigned short int n);

void binaryPrint2(uchar n);

void openAndFill(FILE* fT_ptr, FILE* fB_ptr, STOCK* stock, APT_LIST* aptList) {

	if (!fT_ptr) {

		fT_ptr = fopen(TEXT_FILE_NAME, "w");
		fprintf(fT_ptr, "%u", 0);
		fclose(fT_ptr);
		fT_ptr = fopen(TEXT_FILE_NAME, "r");
	}

	if (!fB_ptr) {

		fB_ptr = fopen(APTS_FILE_NAME, "wb");
		unsigned int size = 0;
		fwrite(&size, sizeof(size), 1, fB_ptr);
		fclose(fB_ptr);
		fB_ptr = fopen(APTS_FILE_NAME, "rb");
	}

	(*aptList) = fillApts(fB_ptr);
	(*stock) = fillStocks(fT_ptr);
	fclose(fT_ptr);
	fclose(fB_ptr);
}
FILE* openFile() {

	FILE* f_ptr = fopen(TEXT_FILE_NAME, "r");
	if (f_ptr)
		return f_ptr;
	f_ptr = fopen(TEXT_FILE_NAME, "w");
	fclose(f_ptr);
	f_ptr = fopen(TEXT_FILE_NAME, "r");
	return f_ptr;
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

			short_term_history[j] = (char*)malloc(commandLength * sizeof(char));
			strcpy(short_term_history[j], command);
		}
		else
			addToTailStockList(&stock, makeStockNode(command));
	}
	return stock;
}

APT_LIST fillApts(FILE* fB_ptr) {

	APT_LIST lst;
	makeEmptyAptList(&lst);
	uint code;
	sint addressLen;
	char address[ADDRESS];
	uchar roomsAndDate[3];
	int price;
	unsigned short int rooms;
	unsigned short int day, month, year;
	time_t database_Entry_Date;
	DATE date;
	unsigned short int mask;

	fread(&lst.size, 1, sizeof(lst.size), fB_ptr);
	uint i;
	for (i = 0; i < lst.size && !feof(fB_ptr); i++) {

		fread(&code, 1, sizeof(code), fB_ptr);
		fread(&addressLen, 1, sizeof(addressLen), fB_ptr);
		fread(address, addressLen, sizeof(char), fB_ptr);
		address[addressLen] = '\0';
		fread(&price, 1, sizeof(price), fB_ptr);
		fread(&database_Entry_Date, 1, sizeof(database_Entry_Date), fB_ptr);
		fread(roomsAndDate, 1, sizeof(roomsAndDate), fB_ptr);
		rooms = (0 | (roomsAndDate[0] >> 4));
		mask = MASK_WITH_N_LSB_SET(sint, 4);
		day = (0 | (((mask & roomsAndDate[0]) << 1) | (1 & roomsAndDate[1])));
		month = (0 | ((mask & (roomsAndDate[1] >> 3))));
		mask >>= 1;
		year = (0 | (((mask & roomsAndDate[1]) << 4) | (roomsAndDate[2] >> 4)));
		date.day = (sint)day;
		date.month = (sint)month;
		date.year = (sint)year;
		addToList(&lst, makeApt(address, code, price, rooms, date, database_Entry_Date));
	}
	return lst;
}

void writeCommands(STOCK* stock) {

	FILE* f_ptr = fopen(TEXT_FILE_NAME, "w");
	STOCK_NODE* cur = stock->head;
	fprintf(f_ptr, "%u\n", stock->size);
	unsigned int i;
	for (i = 0; i < N; i++) {

		if (short_term_history[i])
			fprintf(f_ptr, "%d%s\n", strlen(short_term_history[i]), short_term_history[i]);
	}
	while (cur) {

		fprintf(f_ptr, "%d%s\n", strlen(cur->command),cur->command);
		cur = cur->next;
	}
	fclose(f_ptr);
}

void writeApts(APT_LIST* lst) {

	FILE* f_ptr = fopen(APTS_FILE_NAME, "wb");
	APT* cur = lst->head;
	fwrite(&lst->size, 1, sizeof(lst->size), f_ptr);
	uint i;
	sint addressLen;
	uchar roomsAndDate[3];
	for (i = 0; i < lst->size; i++) {

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

void binaryPrint1(unsigned short int n) {

	uint i;
	unsigned short int mask = MSB_SET(unsigned short int);

	for (i = 0; i < sizeof(unsigned short int) * 8; i++) {

		if (mask & n)
			putchar('1');
		else
			putchar('0');
		mask >>= 1;
	}
	putchar('\n');
}

void binaryPrint2(uchar n) {

	uint i;
	uchar mask = MSB_SET(uchar);

	for (i = 0; i < sizeof(uchar) * 8; i++) {

		if (mask & n)
			putchar('1');
		else
			putchar('0');
		mask >>= 1;
	}
	putchar('\n');
}









