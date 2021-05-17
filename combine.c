#include <stdio.h>
#include <stdlib.h>

#define CH_MAX  (64)

int main(int argc, char *argv[])
{
	FILE *fin[CH_MAX] = { NULL };
	FILE *fout = NULL;
	int chnum, ch, bitwidth;
	char sample[4/*32bit*/ * CH_MAX] = { 0 };

	if (argc < 5)
		goto usage;

	chnum = atoi(argv[1]);
	if (chnum > CH_MAX) {
		printf("error: channel num (%d) > CH_MAX(%d)\n", chnum, CH_MAX);
		goto usage;
	}
	if (argc != 4 + chnum)
		goto usage;
	for (ch = 0; ch < chnum; ++ch) {
		fin[ch] = fopen(argv[2 + ch], "rb");
		if (!fin[ch]) {
			printf("error: can't open %s\n", argv[2 + ch]);
			goto end;
		}
	}

	bitwidth = atoi(argv[2 + chnum]);
	if (bitwidth != 8 && bitwidth != 16 && bitwidth != 24 && bitwidth != 32) {
		printf("error: unsupported bitwidth(%d)\n", bitwidth);
		goto end;
	}
	fout = fopen(argv[2 + chnum + 1], "wb");
	if (!fout) {
		printf("error: can't open %s\n", argv[2 + chnum + 1]);
		goto end;
	}
	bitwidth /= 8;

	printf("Combining ");
	for (ch = 0; ch < chnum; ++ch) {
		printf("\"%s\" ", argv[2 + ch]);
	}
	printf("into \"%s\" ...\n", argv[2 + chnum + 1]);

	while (1) {
		for (ch = 0; ch < chnum; ++ch) {
			if (fread(sample + ch * bitwidth, bitwidth, 1, fin[ch]) != 1) {
				printf("Done.\n");
				goto end;
			}
		}
		fwrite(sample, bitwidth, chnum, fout);
	}

end:
	for (ch = 0; ch < chnum; ++ch)
		if (fin[ch]) {
			fclose(fin[ch]);
			fin[ch] = NULL;
		}
	if (fout) {
		fclose(fout);
		fout = NULL;
	}
	return 0;
usage:
	printf("Usage: combine <chnum> <inputfile_1> <inputfile_2> ... <inputfile_chnum> <bitwidth> <outputfile>\n");
	return 0;
}
