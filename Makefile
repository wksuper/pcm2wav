.PHONY: all
all:
	gcc -O3 main.c -o pcm2wav

.PHONY: clean
clean:
	-rm -f pcm2wav
