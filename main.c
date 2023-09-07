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

	// Verify Wave file data
	if(!validate_wave_file(waveFile)) {
		return 1;
	}

	// Reverse Wave file data and output
	reverse_wave_file(waveFile, argv[2]);

	// TODO also need to free header???
	free(waveFile);

	return 0;
}
