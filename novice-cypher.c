
//	
//	novice-cypher.c is a simple encryption program that
//	reads and replaces the content of a file with a
//	version shifted by a key. Caesar cypher mod 256.
//
//	Created by Nick Chapman, 9-16-2026
//
//	I'm going to create an instance of the files content
//	in memory before performing the cypher, then writing
//	the output back to the file. I think it will be
//	faster, however I wonder if its possible for someone
//	to overflow and look for my previous message in memory.
//
//	So, I wonder if I can make a secure variation that
//	reads and writes directly from/to the file.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFERSIZE 512

//	Function Prototypes
//
void EncryptFile(char* filePath, int dflag, int k);
//void EncryptFileSecure(char* filePath, bool dflag, int k);


//	Function Implementations
//
void EncryptFile(char* filePath, int dflag, int k) {

	int key = k * dflag;

	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		printf("could not open file.");
		return;
	}

	unsigned char content[BUFFERSIZE] = {0};
	int i = 0;

	int ch;
	while ((ch = fgetc(file)) != EOF && i < BUFFERSIZE) {
		if (ch != EOF) {
			content[i] = (unsigned char)((ch + key) % 256);
			i += 1;
		}
	}

	fclose(file);

	file = fopen(filePath, "wb");
	if (file == NULL) {
		printf("could not open file.");
		return;
	}

	fwrite(content, 1, i + 1, file);

	fclose(file);
}


//	Entry Point
//
int main (int argc, char** argv) {

	if (argc != 4) {
		printf("invalid. usage: novice-cypher.exe <path-to-file> <option> <key>\n\t-e\tencrypts by key\n\t-d\tdecrypts by key\n");
		return 1;
	}

	char* filePath = argv[1];
	int dflag = 1;		if (strcmp(argv[2], "-d") == 0) dflag = -1;
	int key = atoi(argv[3]);

	EncryptFile(filePath, dflag, key);
}
