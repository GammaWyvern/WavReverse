#include <stdio.h>
#include "file_lib.h"

int reverseWav(const char* wavPath);
int validateWav(char* wavByteData);

size_t size;
const char* sampleWavPath = "/home/keag/Github/WavReverse/honeyhive.wav";
char* sampleWavByteData; 

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("\nIncorrect number of arguments.\n");
		printf("Correct usage is:\n");
		printf("\treversewav [.wav file]\n\n");

		return 1;
	}

	reverseWav(argv[1]);
	// TODO do something with return error
}

int reverseWav(const char* wavPath) {
	// Get byte data for wav
	char* wavByteData = read_file(wavPath, &size);

	// Validate byte data for wav (for our specifications)
	if(!validateWav(wavByteData)) {
		return 1; // TODO add error code later?
	}

	// Setup variables needed for manipulation
	const short bytesPerSample = wavByteData[32];

	// Setup pointers to shift around samples
	char* swapByteDataLower = wavByteData + 44;
	char* swapByteDataUpper = wavByteData + size - bytesPerSample; 
	char* swapByteDataTemp = (char*)malloc(bytesPerSample*sizeof(char)); 

	// Swap around samples to reverse their order
	while(swapByteDataLower < swapByteDataUpper) {
		for(int sampleByte=0; sampleByte<bytesPerSample; sampleByte++) {
			swapByteDataTemp[sampleByte] = swapByteDataLower[sampleByte]; 
			swapByteDataLower[sampleByte] = swapByteDataUpper[sampleByte]; 
			swapByteDataUpper[sampleByte] = swapByteDataTemp[sampleByte]; 
		}

		swapByteDataLower += bytesPerSample;
		swapByteDataUpper -= bytesPerSample;
	}
	free(swapByteDataTemp);
	
	// Output reversed wavByteData to same file 
	write_file(wavPath, wavByteData, size);

	// Cleanup allocation
	free(wavByteData);

	return 0; // TODO change to error code later?
}

int validateWav(char* wavByteData) {
	int isValid = 1;

	// Initial NULL check
	if(wavByteData == NULL) {
		printf("Wave file does not exist");
		return 1;
	}	

	// String checking
	char* riff = "RIFF";
	char* wave = "WAVE";
	char* fmt  = "fmt ";
	char* data = "data";
	if(*((int*)(wavByteData+0)) != *((int*)(riff))) {
		printf("Not a RIFF compliant file\n");
		isValid = 0;
	}
	if(*((int*)(wavByteData+8)) != *((int*)(wave))) {
		printf("Not a WAVE compliant file\n");
		isValid = 0;
	}
	if(*((int*)(wavByteData+12)) != *((int*)(fmt))) {
		printf("Wave file's format is not supported\n");
		isValid = 0;
	}
	if(*((int*)(wavByteData+36)) != *((int*)(data))) {
		printf("Wave file's data header is not in assumed spot\n");
		// TODO my wav file has extra metadata, so this error occurs
		//isValid = 0;
	}

	// Number checking
	int correctFormat = 1;
	int correctChannels = 2;
	unsigned int correctSize = size - 8;
	if(*(short*)(wavByteData+20) != correctFormat) {
		printf("Wave file's format (%u) is not supported\n", *(short*)(wavByteData+20));
		isValid = 0;
	}
	if(*(short*)(wavByteData+22) != correctChannels) {
		printf("Wave file's number of channels (%u) is not supported\n", *(short*)(wavByteData+22));
		isValid = 0;
	}
	if(*(int*)(wavByteData+4) != correctSize) {
		printf("Wave file's size (%u) does not match assumed size (%u)\n", *(int*)(wavByteData+4), correctSize);
		isValid = 0;
	}

	return isValid;
}

