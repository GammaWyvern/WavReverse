#ifndef __H_WAVE_LIB__
#define __H_WAVE_LIB__

struct wave_header {
	char* header;
	short channels;
	short formatType;
	short bytesPerSample;
	unsigned int dataSize;
};

struct wave_file {
	struct wave_header* waveHeader;
	unsigned int fileSize;
	char* dataPointer;
};

/***********************************************************
 *
 * Creates a wave_header from a wave byte data array.
 * Use read_file() to easily get a wave byte data array.  
 * Use free_wave_header() when finished to free all
 * related memeory allocated.
 *
 * Args:
 * 	headerData: A pointer to 44 bytes of wave header data
 *
 * Returns:
 * 	A pointer to the newly created wave_header  
 *
 **********************************************************/
struct wave_header* create_wave_header(char* headerData);

/***********************************************************
 *
 * Creates a wave_file from a wave file on disk.
 * Use free_wave_file() when finished to free all
 * related memory allocated.
 *
 * Args:
 * 	filePath: A path to a wave file 
 *
 * Returns:
 * 	A pointer to the newely created wave_file if succesful.
 * 	NULL if unsuccesful.
 *
 **********************************************************/
struct wave_file* load_wave(const char* filePath);

/***********************************************************
 *
 * Reverses the samples in a wave_file and writes the
 * newly altered wave_file to the file path passed.
 *
 * Args:
 * 	waveFile: A pointer to a wave_file to reverse samples in
 * 	filePath: file path to output reversed waveFile to
 *
 * Return:
 * 	0 if waveFile was succesfully reversed and written
 * 		back to filePath
 * 	1 if waveFile was sucesfully reversed but not 
 * 		written to filePath
 *
 **********************************************************/
int reverse_wave_file(struct wave_file* waveFile, const char* filePath);

/***********************************************************
 *
 * Ensures a wave_file meets specific criteria by checking
 * its header and prints out any critia not met.
 * Criteria:
 * 	waveFile is not NULL
 * 	waveFile contains "RIFF" in header bytes 0-3
 * 	waveFile contains "WAVE" in header bytes 8-11
 * 	waveFile contains "fmt " in header bytes 12-15
 * 	waveFile contains "data" in header bytes 36-39
 * 	waveFile has a format value of 1 in header bytes 20-21
 * 	waveFile has the size of the wave file - 8 in header bytes 4-7
 * 	waveFile has 2 channels in header bytes 22-23
 * TODO update for channels
 *
 * Args:
 * 	waveFile: wave file to validate based on criteria
 *
 * Returns:
 * 	1 if waveFile is valid and meets criteria
 * 	0 if waveFile is not valid and doesn't meet all criteria
 *
 **********************************************************/
int validate_wave_file(struct wave_file* waveFile);

/***********************************************************
 *
 * Frees all memory allocated from a wave_file created
 * with load_wave().
 *
 * Args:
 * 	waveFile: A pointer to the wave_file to free  
 *
 **********************************************************/
void free_wave_file(struct wave_file* waveFile);

/***********************************************************
 *
 * Frees all memory allocated from a wave_header created
 * with create_wave_header().
 *
 * Args:
 * 	waveHeader: A pointer to the wave_header to free  
 *
 **********************************************************/
void free_wave_file(struct wave_file* waveFile);

#endif
