#ifndef __H_LIB__
#define __H_LIB__

struct wav_header {
	char* header;
}

struct wav_file {
	struct wav_header* wavHeader;
	int fileSize;
	char* dataPointer;
}

int reverseWav(const char* wavPath, const char* outputPath);
int validateWav(char* wavByteData);

#endif
