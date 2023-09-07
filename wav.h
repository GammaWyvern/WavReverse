#ifndef __H_LIB__
#define __H_LIB__

struct wave_header {
	char* header;
};

struct wave_file {
	struct wave_header* waveHeader;
	int fileSize;
	char* dataPointer;
};

struct wave_header* create_wave_header(const char* headerData);
struct wave_file* load_wave(const char* filePath);
int create_wave_file(struct wave_file, const char* filePath);

int reverseWav(const char* wavPath, const char* outputPath);
int validateWav(char* wavByteData);

#endif
