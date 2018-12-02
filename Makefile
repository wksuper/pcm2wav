all:
	clang -Ofast main.c -o pcm2wav
clean:
	rm pcm2wav
