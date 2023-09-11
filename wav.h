#ifndef __H_WAVE_LIB__
#define __H_WAVE_LIB__

struct wave_header {
	char* header;
	unsigned short channels;
	unsigned short bytesPerSample;
	unsigned int dataSize;
};

struct wave_file {
	struct wave_header* waveHeader;
	unsigned int fileSize;
	char* dataPointer;
};

struct wave_header* create_wave_header(char* headerData);
struct wave_file* load_wave(const char* filePath);
int reverse_wave_file(struct wave_file*, const char* filePath);
int validate_wave_file(struct wave_file* waveFile);
void free_wave_file(struct wave_file* waveFile);

#endif
