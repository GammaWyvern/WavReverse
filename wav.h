#ifndef __H_WAVE_LIB__
#define __H_WAVE_LIB__

struct wave_header {
	char* header;
	char* fileContainer; // Bytes 0-3
	unsigned int assumedSize; // Bytes 4-7
	char* fileType; // Bytes 8-11
	char* formatChunk; // Bytes 12-15
	unsigned int formatLength; // Bytes 16-19 
	unsigned short formatType; // Bytes 20-21
	unsigned short channels; // Bytes 22-23
	unsigned int sampleRate; // Bytes 24-27
	unsigned int bytesPerSecond; // Bytes 28-31
	unsigned short bytesPerSample; // Bytes 32-33
	unsigned short bitsPerSample; // Bytes 34-35
	char* dataLabel;  // Bytes 36-39 
	unsigned int dataSize; // Bytes 40-43
};

struct wave_file {
	struct wave_header* waveHeader;
	int fileSize;
	char* dataPointer;
};

/***********************************************************
 *
 * Creates a wave_header from a wave byte data array.
 * Use read_file() to easily get a wave byte data array.  
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
 * newly altered wave_file to the file path passed without
 * permanently changing wave_file data.
 *
 * Args:
 * 	waveFile: A pointer to a wave_file to reverse samples in
 * 		USE validate_wave_file FIRST before calling
 * 		this function on a wave_file
 * 	filePath: file path to output reversed waveFile to
 *
 * Return:
 * 	0 if waveFile was succesfully reversed and written
 * 		to filePath
 * 	1 if waveFile was not sucesfully reversed and 
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

#endif
