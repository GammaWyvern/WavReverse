#include <stdio.h>
#include <stdlib.h>
#include "wav.h"
#include "file_lib.h"

struct wave_file* load_wave(const char* filePath) {
	size_t size;
	char* waveByteData = read_file(filePath, &size);
	if(waveByteData == NULL) {
		return NULL;
	}

	// Setup waveFile
	struct wave_file* waveFile = malloc(sizeof(*waveFile));
	waveFile->waveHeader = create_wave_header(waveByteData);
	waveFile->fileSize = size;
	waveFile->dataPointer = waveByteData + 44;

	return waveFile;
}

struct wave_header* create_wave_header(char* headerData) {
	struct wave_header* waveFileHeader = malloc(sizeof(*waveFileHeader));
	waveFileHeader->header = headerData;
	waveFileHeader->fileContainer = (headerData+0);
	waveFileHeader->assumedSize = *(unsigned int*)(headerData+4);
	waveFileHeader->fileType = (headerData+8);
	waveFileHeader->formatChunk = (headerData+12);
	waveFileHeader->formatLength = *(unsigned int*)(headerData+16);
	waveFileHeader->formatType = *(unsigned short*)(headerData+20);
	waveFileHeader->channels = *(unsigned short*)(headerData+22);
	waveFileHeader->sampleRate = *(unsigned int*)(headerData+24);
	waveFileHeader->bytesPerSecond = *(unsigned int*)(headerData+28);
	waveFileHeader->bytesPerSample = *(unsigned short*)(headerData+32);
	waveFileHeader->bitsPerSample = *(unsigned short*)(headerData+34);
	waveFileHeader->dataLabel = (headerData+36);
	waveFileHeader->dataSize = *(unsigned int*)(headerData+40);

	return waveFileHeader;
}

int reverse_wave_file(struct wave_file* waveFile, const char* outputFilePath) {
	// Setup pointers to shift around samples
	char* swapByteDataLower = waveFile->dataPointer;
	char* swapByteDataUpper = waveFile->dataPointer + waveFile->waveHeader->dataSize - waveFile->waveHeader->bytesPerSample;
	char* swapByteDataTemp = malloc(waveFile->waveHeader->bytesPerSample*sizeof(*swapByteDataTemp)); 

	// Swap around samples byte by byte to reverse their order
	while(swapByteDataLower < swapByteDataUpper) {
		for(int sampleByte=0; sampleByte<waveFile->waveHeader->bytesPerSample; sampleByte++) {
			swapByteDataTemp[sampleByte] = swapByteDataLower[sampleByte]; 
			swapByteDataLower[sampleByte] = swapByteDataUpper[sampleByte]; 
			swapByteDataUpper[sampleByte] = swapByteDataTemp[sampleByte]; 
		}

		swapByteDataLower += waveFile->waveHeader->bytesPerSample;
		swapByteDataUpper -= waveFile->waveHeader->bytesPerSample;
	}
	free(swapByteDataTemp);
	
	// Output reversed wavByteData new file 
	return write_file(outputFilePath, waveFile->waveHeader->header, waveFile->fileSize);
}

int validate_wave_file(struct wave_file* waveFile) {
	if(waveFile == NULL) {
		return 0;
	}

	int isValid = 1;

	// String checking
	char* riff = "RIFF";
	char* wave = "WAVE";
	char* fmt  = "fmt ";
	char* data = "data";
	
	// You're gonna love this shit, it's beautiful!
	// Just cast all the pointers to ints to read 4 bytes! Boom!
	// No library or complex code needed!
	if(*(int*)waveFile->waveHeader->fileContainer != *(int*)riff) {
		printf("Not a RIFF compliant file\n");
		// Doesn't make sense to continue checks if not RIFF file
		return 0;
	}
	if(*(int*)waveFile->waveHeader->fileType != *(int*)wave) {
		printf("Not a WAVE compliant file\n");
		// Doesn't make sense to continue checks if not WAVE file
		return 0;
	}
	if(*(int*)waveFile->waveHeader->formatChunk != *(int*)fmt) {
		printf("Wave file's format is not supported\n");
		isValid = 0;
	}
	if(*(int*)waveFile->waveHeader->dataLabel != *(int*)data) {
		printf("Wave file's data header is not in assumed spot\n");
		isValid = 0;
	}

	// Number checking
	short correctFormat = 1;
	short correctChannels = 2;
	int correctSize = waveFile->fileSize - 8;
	if(waveFile->waveHeader->formatType != correctFormat) {
		printf("Wave file's format (%hi) is not supported\n", waveFile->waveHeader->formatType); 
		isValid = 0;
	}
	if(waveFile->waveHeader->channels != correctChannels) {
		printf("Wave file's number of channels (%hi) is not supported\n", waveFile->waveHeader->channels);
		isValid = 0;
	}
	if(waveFile->waveHeader->assumedSize != correctSize) {
		printf("Wave file's size (%d) does not match assumed size (%d)\n", waveFile->waveHeader->assumedSize, correctSize);
		isValid = 0;
	}

	return isValid;
}

void free_wave_file(struct wave_file* waveFile) {
	if(waveFile != NULL) {
		free(waveFile->waveHeader->header);
		free(waveFile->waveHeader);
		free(waveFile);
	}
}

