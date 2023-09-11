#include <stdlib.h>
#include <stdio.h>
#include "wav.h"

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("\nIncorrect number of arguments.\n");
		printf("Correct usage is:\n");
		printf("\treversewave [src .wav file] [dest .wav file]\n\n");
		return 1;
	}

	// Create Wave file data
	struct wave_file* waveFile = load_wave(argv[1]);

	// Verify Wave file data (contains NULL check)
	if(!validate_wave_file(waveFile)) {
		free_wave_file(waveFile);
		return 1;
	}

	// Reverse Wave file data and output to file
	if(reverse_wave_file(waveFile, argv[2])) {
		free_wave_file(waveFile);
		return 1;
	}

	// Deallocate memory for waveFile if no error
	free_wave_file(waveFile);
	return 0;
}
