#include <stdio.h>
#include "wav.h"
#include "file_lib.h"

struct wave_file* load_wave(const char* filePath) {
	size_t size;
	char* waveByteData = read_file(filePath, &size);
	if(waveByteData == NULL) {
		return NULL;
	}

	// Setup waveFile
	struct wave_file* waveFile = (struct wave_file*)malloc(sizeof(struct wave_file));
	waveFile->waveHeader = create_wave_header(waveByteData);
	waveFile->fileSize = size;
	waveFile->dataPointer = waveByteData + 44;
}

struct wave_header* create_wave_header(char* headerData) {
	struct wave_header* waveFileHeader = (struct wave_header*)malloc(sizeof(struct wave_header));
	waveFileHeader->header = headerData;
	waveFileHeader->channels = headerData[22];
	waveFileHeader->bytesPerSample = headerData[32];
	waveFileHeader->dataSize = headerData[40];

	return waveFileHeader;
}

int reverse_wave_file(struct wave_file* waveFile, const char* outputFilePath) {
	// Setup pointers to shift around samples
	char* swapByteDataLower = waveFile->dataPointer;
	char* swapByteDataUpper = waveFile->waveHeader->header + waveFile->fileSize;
	char* swapByteDataTemp = (char*)malloc(waveFile->waveHeader->bytesPerSample*sizeof(char)); 

	// Swap around samples to reverse their order
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
	write_file(outputFilePath, waveFile->waveHeader->header, waveFile->fileSize);
}

int validate_wave_file(struct wave_file* waveFile) {
	int isValid = 1;

	// String checking
	char* riff = "RIFF";
	char* wave = "WAVE";
	char* fmt  = "fmt ";
	char* data = "data";
	
	if(*((int*)(waveFile->waveHeader->header+0)) != *((int*)(riff))) {
		printf("Not a RIFF compliant file\n");
		isValid = 0;
	}
	if(*((int*)(waveFile->waveHeader->header+8)) != *((int*)(wave))) {
		printf("Not a WAVE compliant file\n");
		isValid = 0;
	}
	if(*((int*)(waveFile->waveHeader->header+12)) != *((int*)(fmt))) {
		printf("Wave file's format is not supported\n");
		isValid = 0;
	}
	if(*((int*)(waveFile->waveHeader->header+36)) != *((int*)(data))) {
		printf("Wave file's data header is not in assumed spot\n");
		isValid = 0;
	}

	// Number checking
	int correctFormat = 1;
	int correctChannels = 2;
	unsigned int correctSize = waveFile->fileSize - 8;
	if(*(short*)(waveFile->waveHeader->header+20) != correctFormat) {
		printf("Wave file's format (%u) is not supported\n", *(short*)(waveFile->waveHeader->header+20));
		isValid = 0;
	}
	if(*(short*)(waveFile->waveHeader->header+22) != correctChannels) {
		printf("Wave file's number of channels (%u) is not fully supported... continuing\n", *(short*)(waveFile->waveHeader->header+22));
		//isValid = 0;
	}
	if(*(int*)(waveFile->waveHeader->header+4) != correctSize) {
		printf("Wave file's size (%u) does not match assumed size (%u)\n", *(int*)(waveFile->waveHeader->header+4), correctSize);
		isValid = 0;
	}

	return isValid;
}

