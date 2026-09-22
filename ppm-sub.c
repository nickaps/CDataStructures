
//	ppm-sub.c is the first of many parts of a series of ppm image
//	processing in the CDataStructures project. ppm-sub.c will take
//	two images and subtracts the values of the second from the first.
//	Should give warning if dimensions don't match but still subtract
//	given dimensions from larger or smaller image.
//	
//	Created by Nick Chapman, 9-17-2026
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//	We assume that all Images will be
//	8 bit size. 255 max color value.
//
struct Image {
	int width;
	int height;

	int* colorValues;
};


//	Function Prototypes
//
struct Image* ParsePPM(char* filePath);
void FreeImage(struct Image* image);
struct Image* GetSubtraction(struct Image* base, struct Image* subtraction, uint8_t flag);
void WritePPM(struct Image* image, char* fileDir);

//	Function Implementations
//
struct Image* ParsePPM(char* filePath) {

	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("failed to open file.\n");
		return NULL;
	}
	
	struct Image* newImage = malloc(sizeof(struct Image));
	
	char buffer[256];
	int lineIndex = 0;
	
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
	
		if (*buffer != '#') {
		
			if (lineIndex == 1) {
				char* cursor = buffer;
				while(*cursor != ' ') {
					cursor = &cursor[1];
				}
				*cursor = '\0';
				cursor = &cursor[1];

				newImage->width = atoi(buffer);
				newImage->height = atoi(cursor);

				newImage->colorValues = malloc(sizeof(int) * newImage->width * newImage->height * 3);
			}
			else if (lineIndex > 2 && (lineIndex - 3) < (newImage->width * newImage->height * 3)) {
				newImage->colorValues[lineIndex - 3] = atoi(buffer);
			}

			lineIndex += 1;
		}
	}

	fclose(file);

	return newImage;
}

void WritePPM(struct Image* image, char* fileDir) {

	if (image == NULL || image->colorValues == NULL) return;

	FILE* file = fopen(fileDir, "w");
	if (file == NULL) {
		printf("failed to create file at %s\n", fileDir);
		return;
	}

	fprintf(file, "P3\n#Created by ppm-sub by Nick\n%d %d\n255\n", image->width, image->height);

	int area = image->width * image->height;
	for (int i = 0; i < area * 3; i++) {
		fprintf(file, "%d\n", image->colorValues[i]);
	}

	fclose(file);
}

void FreeImage(struct Image* image) {
	struct Image* tmp = image;
	
	free(image->colorValues);
	image->colorValues = NULL;

	image = NULL;
	free(tmp);
}

struct Image* GetSubtraction(struct Image* base, struct Image* subtraction, uint8_t flag) {

	if (base == NULL || subtraction == NULL) {
		printf("missing parameters; null pointers.\n");
		return NULL;
	}

	if (base->width != subtraction->width || base->height != subtraction->height) {
		printf("warning: images uneven. will still attempt to subtract.\n");
	}

	int scanWidth = MIN(base->width, subtraction->width);
	int scanHeight = MIN(base->height, subtraction->height);

	int scanArea = scanWidth * scanHeight;

	struct Image* newImage = malloc(sizeof(struct Image));
	newImage->width = scanWidth;
	newImage->height = scanHeight;
	newImage->colorValues = malloc(sizeof(int) * scanArea * 3);

	for (int i = 0; i < scanArea * 3; i++) {
		int diff = 0;

		if (flag != (uint8_t)1)
			diff = base->colorValues[i] - subtraction->colorValues[i];
		else
			diff = base->colorValues[i] + subtraction->colorValues[i];

		diff = MIN(255, MAX(0, diff));
		
		newImage->colorValues[i] = diff;
	}

	return newImage;
}

int main(int argc, char** argv) {

	uint8_t flag = (uint8_t)0;

	if (argc != 4 && argc != 5) {
		printf("invalid. usage: ppm-sub <base-file> <subtraction-file> <output-file> [-subtraction flag]\n");
		return 1;
	}

	if (argc == 5 && strcmp(argv[4], "-a") == 0) {
		flag = (uint8_t)1;
	}

	struct Image* base = ParsePPM(argv[1]);
	struct Image* subt = ParsePPM(argv[2]);

	struct Image* output = GetSubtraction(base, subt, flag);

	WritePPM(output, argv[3]);
	
	FreeImage(base);
	FreeImage(subt);
	FreeImage(output);

	return 0;	
}
