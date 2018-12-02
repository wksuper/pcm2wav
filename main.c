#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned char ID[4];

typedef struct {
	ID       chunkID;  /* {'f', 'm', 't', ' '} */
	int32_t  chunkSize;
	int16_t  wFormatTag;
	uint16_t wChannels;
	uint32_t dwSamplesPerSec;
	uint32_t dwAvgBytesPerSec;
	uint16_t wBlockAlign;
	uint16_t wBitsPerSample;
	/* Note: there may be additional fields here,
	   depending upon wFormatTag. */
} FormatChunk;

typedef struct {
	ID      chunkID;  /* {'d', 'a', 't', 'a'}  */
	int32_t chunkSize;
	uint8_t waveformData[];
} DataChunk;

int main(int argc, char *argv[])
{
	FILE *pcmfile, *wavfile;
	int32_t pcmfile_size, chunk_size;
	FormatChunk formatchunk;
	DataChunk datachunk;
	size_t read_len;
	char buf[1024];

	if (argc != 6)
		goto usage;

	pcmfile = fopen(argv[1], "rb");
	if (pcmfile == NULL) {
		printf("error: Can't open pcmfile.\n");
		return -1;
	}
	fseek(pcmfile, 0, SEEK_END);
	pcmfile_size = ftell(pcmfile);
	fseek(pcmfile, 0, SEEK_SET);

	wavfile = fopen(argv[2], "wb");
	if (wavfile == NULL) {
		printf("error: Can't create wavfile.\n");
		return -1;
	}

	fwrite("RIFF", 1, 4, wavfile);
	fwrite("xxxx", 1, 4, wavfile);  //reserved for the total chunk size
	fwrite("WAVE", 1, 4, wavfile);

	formatchunk.chunkID[0] = 'f';
	formatchunk.chunkID[1] = 'm';
	formatchunk.chunkID[2] = 't';
	formatchunk.chunkID[3] = ' ';
	formatchunk.chunkSize = sizeof(FormatChunk) - sizeof(ID) - sizeof(int32_t);
	formatchunk.wFormatTag = 1;   /* uncompressed */
	formatchunk.wChannels = atoi(argv[3]);
	formatchunk.dwSamplesPerSec = atoi(argv[4]);
	formatchunk.wBitsPerSample = atoi(argv[5]);
	formatchunk.wBlockAlign = formatchunk.wChannels * (formatchunk.wBitsPerSample >> 3);
	formatchunk.dwAvgBytesPerSec = formatchunk.wBlockAlign * formatchunk.dwSamplesPerSec;
	fwrite(&formatchunk, 1, sizeof(formatchunk), wavfile);

	datachunk.chunkID[0] = 'd';
	datachunk.chunkID[1] = 'a';
	datachunk.chunkID[2] = 't';
	datachunk.chunkID[3] = 'a';
	datachunk.chunkSize = pcmfile_size;
	fwrite(&datachunk, 1, sizeof(ID) + sizeof(int32_t), wavfile);

	while ((read_len = fread(buf, 1, sizeof(buf), pcmfile)) != 0) {
#if 0
		/* revert the endiean */
		int i;

		for (i = 0; i < read_len; i += 2) {
			tmp = buf[i];
			buf[i] = buf[i + 1];
			buf[i + 1] = tmp;
		}
#endif
		fwrite(buf, 1, read_len, wavfile);
	}

	fseek(wavfile, 4, SEEK_SET);
	chunk_size = 4 + (8 + formatchunk.chunkSize) + (8 + datachunk.chunkSize);
	fwrite(&chunk_size, 1, 4, wavfile);

	fclose(pcmfile);
	fclose(wavfile);

	return 0;

usage:
	printf("usage:\n"
	       "\tpcm2wav <pcm-file> <wav-file> <channels> <sample-rate> <bits-per-sample>\n");
	return 0;
}
