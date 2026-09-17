
//	A program that will provide the average value
//	for each row of a .csv file.
//
//	Created by Nick Chapman, 9-16-2026

#include <stdio.h>
#include <stdlib.h>

//	A LinkedList node going rightwards on .csv table
//	Contains double type value and points to next in row.
//
struct ColumnNode {
	struct ColumnNode* next;
	double value;
};

//	A LinkedList node going downwards on .csv table
//	Contains RowNode* head node for row and poitns
//	to next down.
//
struct RowNode {
	struct RowNode* next;
	struct ColumnNode* head;
	struct ColumnNode* tail;
	int width;
};

//	Table struct contains a head and tail
//	row pointer and current width and
//	height.
//
struct Table {
	struct RowNode* head;
	struct RowNode* tail;
	int depth;
};


//	Function Prototypes
//
struct Table* ParseCSVTable(const char* directory);
void NewRow(struct Table* table);
void AppendColumn(struct Table* table, double value);
void FreeTable(struct Table* table);
double LookupValue(struct Table* table, int x, int y);
struct Table* AverageTable(struct Table* table);
int WriteToCSV(struct Table* table, char* fileDir);

//	Funciton Implementations
//
//
//	Parse .csv file into table object
//
struct Table* ParseCSVTable(const char* directory) {

	//	NOTE: To see values of table upon output,
	//		  uncomment lines that are denoted with: [*]

	//	Set filePtr to the .csv file
	//
	FILE *filePtr = fopen(directory, "r");

	//	Check to see if open was successful
	//
	if (filePtr == NULL) {
		printf("failed to open file.\n");
		return NULL;
	}
	
	//	Dynamically allocate memory for newTable
	//
	struct Table* newTable = malloc(sizeof(struct Table));
	newTable->head = NULL;
	newTable->tail = NULL;
	newTable->depth = 0;
	
	//	Initialize buffer to read lines to
	//
	char buffer[512];

	//	Read line by line
	//
	while (fgets(buffer, sizeof(buffer), filePtr) != NULL) {

		//	Create new row in table
		//
		NewRow(newTable);

		//	Initialize cursor and checkpoint pointers
		//
		char* cursor = buffer;
		char* checkpoint = cursor;

		//	Set endFlag to 1...
		//
		int endFlag = 1;
		//	
		//	...then continue to run as
		//	long as it's not 0.
		//
		while (endFlag) {

			//	Read number into list when cursor reaches ',' or '\0'
			//
			if (*cursor == ',' || *cursor == '\0') {

				//	If cursor is null-terminator set the endFlag to 0
				//
				if (*cursor == '\0') {
					endFlag = 0;
				}

				//	Always set cursor to point to null-terminator
				//	to denote end of string for atof()
				//
				*cursor = '\0';
				//
				//	Perform atof() on our current number string,
				//	which starts at checkpoint and is now
				//  terminated with cursor.
				//
				double v = atof(checkpoint);
				//	
				//	Add new column to the end of our row thus far
				//	supply it with new parsed value
				//
				AppendColumn(newTable, v);
				//
				// 
				//printf("%lf ", v);	//	[*]

				if (endFlag)
					checkpoint = &cursor[1];
			}
			cursor++;
		}
		//printf("\n");		//	[*]
	}

	//	Close the file
	//
	fclose(filePtr);
	//
	//	Return the table
	//
	return newTable;
}
//
//	Write table object into .csv file
//
int WriteToCSV(struct Table* table, char* fileDir) {

	if (table == NULL) {
		printf("invalid. table is null pointer.\n");
		return 1;
	}
	
	if (table->head == NULL) {
		printf("table is uninitialized. try running NewRow() first.\n");
		return 1;
	}

	FILE* csvout = fopen(fileDir, "w");

	struct RowNode* rowCursor = table->head;
	struct ColumnNode* columnCursor = NULL;

	while (rowCursor != NULL) {

		columnCursor = rowCursor->head;

		while (columnCursor != NULL) {

			fprintf(csvout, "%lf", columnCursor->value);

			columnCursor = columnCursor->next;

			if (columnCursor != NULL)
				fprintf(csvout, ",");
			else
				fprintf(csvout, "\n");
		}

		rowCursor = rowCursor->next;
	}

	fclose(csvout);

	printf("successfully wrote to %s\n", fileDir);
	
	return 0;
}
//
//	Lookup a value at a certain coordinate
//
double LookupValue(struct Table* table, int x, int y) {

	//	Temporary index values for row and column
	//
	int columnIndex = 0;
	int rowIndex 	= 0;

	//	RowNode pointer
	//
	struct RowNode* rowCursor = table->head;

	//	Loop through until reached correct row
	//
	while (rowIndex < y) {
		rowCursor = rowCursor->next;
		rowIndex += 1;
		
		if (rowCursor == NULL) return 0;
	}

	//	ColumnNode pointer
	//
	struct ColumnNode* columnCursor = rowCursor->head;

	//	Loop through until reached correct column
	//
	while (columnIndex < x) {
		columnCursor = columnCursor->next;
		columnIndex += 1;
		
		if (columnCursor == NULL) return 0;
	}

	//	Return final destination cursor value
	//
	return columnCursor->value;
}
//
//	Add a new empty row to the table
//
void NewRow(struct Table* table) {

	//	Dynamically allocate memory for newRow
	//
	struct RowNode* newRow = malloc(sizeof(struct RowNode));
	newRow->head = NULL;
	newRow->tail = NULL;
	newRow->next = NULL;
	newRow->width = 0;

	//	Update table->tail
	//
	if (table->tail != NULL) {
		table->tail->next = newRow;
		newRow->width = table->tail->width + 1;
	}
	else {
		table->head = newRow;
	}
	table->tail = newRow;
}
//
//	Append a new column to the end of latest row
//
void AppendColumn(struct Table* table, double value) {

	//	Check if row exists
	//
	if (table->tail == NULL) {
		printf("table uninitialized. try tunning NewRow() beforehand.\n");
		return;
	}

	//	Dynamically allocate memory for newColumn
	//
	struct ColumnNode* newColumn = malloc(sizeof(struct ColumnNode));
	newColumn->next = NULL;
	newColumn->value = value;

	if (table->tail->tail != NULL) {
		table->tail->tail->next = newColumn;
	}
	else {
		table->tail->head = newColumn;
	}
	table->tail->tail = newColumn;
}
//
//	Free entire table, rows and columns included
//
void FreeTable(struct Table* table) {

	//	Check if table is uninitialized
	//
	if (table == NULL) return;
	if (table->head == NULL) {
		free(table);
		return;
	}

	//	Initialize RowNode and ColumnNode pointers
	//
	struct RowNode* rowCursor = table->head;
	struct ColumnNode* columnCursor = rowCursor->head;

	//	Loop until there are no more rows
	//
	while (rowCursor != NULL) {
	
		//	Loop unitil there are no more columns
		//
		while (columnCursor != NULL) {
		
			//	Free using tmp and move columnCursor to next
			//
			struct ColumnNode* tmp = columnCursor;
			columnCursor = columnCursor->next;
			free(tmp);
		}
		
		//	Free using tmp and move rowCursor to next
		//
		struct RowNode* tmp = rowCursor;
		rowCursor = rowCursor->next;
		
		//	Only set columnCursor to head if row exists
		//
		if (rowCursor != NULL)
			columnCursor = rowCursor->head;
			
		free(tmp);
	}

	//	Free the table
	//
	free(table);
}
//
//	Returns new table with averages of each row
//
struct Table* AverageTable(struct Table* table) {

	if (table == NULL) {
		printf("invalid. table is null pointer\n");
		return NULL;
	}

	struct Table* averageTable = malloc(sizeof(struct Table));
	averageTable->head = NULL;
	averageTable->tail = NULL;
	averageTable->depth = 0;

	struct RowNode* rowCursor = table->head;
	struct ColumnNode* columnCursor = NULL;

	while (rowCursor != NULL) {
	
		double total = 0.0;
		double count = 0.0;

		columnCursor = rowCursor->head;
		
		while (columnCursor != NULL) {
			total += columnCursor->value;
			count += 1.0;
			columnCursor = columnCursor->next;
		}

		double cellAverage = total / count;

		// printf("%lf\n", cellAverage);	//	[*]

		NewRow(averageTable);
		AppendColumn(averageTable, cellAverage);

		rowCursor = rowCursor->next;
	}

	return averageTable;
}


//	Entry Point
//
int main(int argc, char** argv) {

	//	Check to make sure argc matches
	//
	if (argc != 2) {
		printf("invalid. provide path to valid .csv file.\n");
		return 1;
	}

	//	Set filepath to .csv file
	//
	char* filePath = argv[1];

	//	Parse .csv file into table object
	//
	struct Table* table = ParseCSVTable(filePath);

	struct Table* averageTable = AverageTable(table);

	int flags = WriteToCSV(averageTable, "./average-table.csv");

	if (flags == 1) {
		printf("could not write to file.\n");
	}
	
	//	Free the table object
	//
	FreeTable(table);
	FreeTable(averageTable);

	return 0;
}
