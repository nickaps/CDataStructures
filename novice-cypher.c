
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
//

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

	//	Set the key equal to the value, k, by the mag, dflag
	//
	int key = k * dflag;
	//
	//	(rb) Open a file at filePath
	//
	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		printf("could not open file.");
		return;
	}
	//
	//	Set content buffer
	//
	unsigned char content[BUFFERSIZE] = {0};
	//
	//	Set increment value; used as length
	//
	int i = 0;
	//
	//	Reserve space for character value
	//
	int ch;
	while ((ch = fgetc(file)) != EOF && i < BUFFERSIZE) {
		//	While not EOF continue to alter bytes
		//	and add them to memory
		//
		if (ch != EOF) {
			content[i] = (unsigned char)((ch + key) % 256);
			i += 1;
		}
	}
	//
	//	Close file
	//
	fclose(file);
	//
	//	(wb) Open the file again at filePath
	//
	file = fopen(filePath, "wb");
	if (file == NULL) {
		printf("could not open file.");
		return;
	}
	//
	//	Overwrite all bytes in file with content
	//
	fwrite(content, 1, i, file);
	//
	//	Close file
	//
	fclose(file);
}


//	Entry Point
//
int main (int argc, char** argv) {

	if (argc != 4) {
		printf("invalid. usage: novice-cypher.exe <path-to-file> <option> <key>\n\t-e\tencrypts by key\n\t-d\tdecrypts by key\n");
		return 1;
	}

	//	filePath - Which file to encrypt
	//
	char* filePath = argv[1];
	//
	//	dflag - +/- magnitude for key value
	//
	int dflag = 1;
	//
	//	if second argument matches "-d" then
	//	set dflag to decrypt mode (-1).
	//
	if (strcmp(argv[2], "-d") == 0)
		dflag = -1;
	//
	//	key - number of bytes to shift
	//
	int key = atoi(argv[3]);

	//
	//	EncryptFile() at filePath using the dflag and key
	//
	EncryptFile(filePath, dflag, key);

	//	Happy ending :)
	//
	return 0;
}
