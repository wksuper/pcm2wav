.PHONY: all
all: pcm2wav combine

.PHONY: clean
clean:
	-rm -f pcm2wav combine

pcm2wav:
	gcc -O2 pcm2wav.c -o pcm2wav

combine:
	gcc -O2 combine.c -o combine
