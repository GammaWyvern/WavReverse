#include "wav.h"
#include "file_lib.h"

size_t size;

struct wave_file* load_wave(const char* filePath) {
	char* waveByteData = read_file(filePath, &size);
	struct wave_file* waveFile = (struct wave_file*)malloc(sizeof(struct wave_file));

	// Setup header
	struct wave_header* waveHeader = (struct wave_header*)malloc(sizeof(struct wave_header));
	for(int offset=0; offset<44; offset++) {
		(waveHeader->header)[offset] = waveByteData[offset];
	}
	waveFile->waveHeader = waveHeader;
	
	// Setup size
	waveFile->fileSize = size;
	
	// Setup dataPointer
	waveFile->dataPointer = waveByteData + 44;
}





/*

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("\nIncorrect number of arguments.\n");
		printf("Correct usage is:\n");
		printf("\treversewav [src .wav file] [dest .wav file]\n\n");

		return 1;
	}

	reverseWav(argv[1], argv[2]);
	// TODO do something with return error?
}

int reverseWav(const char* wavPath, const char* outputPath) {
	// Get byte data for wav
	char* wavByteData = read_file(wavPath, &size);

	// NULL check
	if(!wavByteData) {
		return 1;
	}
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
	
	// Output reversed wavByteData new file 
	write_file(outputPath, wavByteData, size);

	// Cleanup allocation
	free(wavByteData);

	return 0; // TODO change to error code later?
}

int validateWav(char* wavByteData) {
	int isValid = 1;

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
		isValid = 0;
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
		printf("Wave file's number of channels (%u) is not fully supported... continuing\n", *(short*)(wavByteData+22));
		//isValid = 0;
	}
	if(*(int*)(wavByteData+4) != correctSize) {
		printf("Wave file's size (%u) does not match assumed size (%u)\n", *(int*)(wavByteData+4), correctSize);
		isValid = 0;
	}

	return isValid;
}

*/
