#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

int cmpCounter = 0;

int rot13cmps(char s1, char s2) {
	// rot13 affects the result when s1 s2 are both upper/lower
	if (isupper(s1) && isupper(s2)) {
		s1 = s1 + (s1 < 78 ? 13 : -13);
		s2 = s2 + (s2 < 78 ? 13 : -13);
	}
	else if (islower(s1) && islower(s2)) {
		s1 = s1 + (s1 < 110 ? 13 : -13);
		s2 = s2 + (s2 < 110 ? 13 : -13);
	}
	if (s1 < s2)
		return -1;
	if (s1 == s2)
		return 0;
	return 1;
}

int rot13cmp(const void *a, const void *b) {
	cmpCounter += 1;
	const char *s1 = *(const char **) a;
	const char *s2 = *(const char **) b;
	while (*s1 != '\n' && *s2 != '\n') {
		int result = rot13cmps(*s1, *s2);
		if (result != 0)
			return result;
		s1 += 1;
		s2 += 1;
	}
	if (*s1 != '\n') 
		return 1;  // Prefix => -1
	if (*s2 != '\n')
		return -1;  // Longer => 1
	return 0;  // Completely the same
}

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "wrong number of arguments!");
        exit(1);
    }
	int bufferSize = 1024;  // 1k initial buffer
	int numLine = 0;
	int numChar = 0;
	char *text = (char *) malloc(sizeof(char)*bufferSize);
	if (text == NULL) {
		fprintf(stderr, "error in allocating memory");
		exit(1);
	}
	char c;
    while (read(0, &c, 1) > 0) {
		text[numChar++] = c;
		if (numChar == bufferSize) {
			// need a larger array
			bufferSize *= 2;
			text = (char*) realloc(text, bufferSize);
			if (text == NULL) {
				fprintf(stderr, "error in allocating memory");
				exit(1);
			}
		}
		if (c == '\n')
			numLine ++;
	}
	// empty input
	if (numChar == 0) {
		free(text);
		return 0;
	}
	// append a trailing newline if necessary
	if (text[numChar-1] != '\n') {
		text[numChar++] = '\n';
		numLine ++;
	}

	char **words = (char **) malloc(sizeof(char*)*numLine);
	if (words == NULL) {
		fprintf(stderr, "error in allocating memory");
		exit(1);
	}
	int start = 0;
	int wordCount = 0;
	for (int i = 0; i < numChar; i ++) {
		if (text[i] == '\n') {
			words[wordCount ++] = text + start;
			start = i + 1;
		}
	}
	
	qsort(words, wordCount, sizeof(char*), rot13cmp);
	
	errno = 0;  // initialize

    char newline = '\n';
	for (int i = 0; i < numLine; i ++) {
		int j = 0;
		while (*(words[i]+j) != '\n') {
            write(1, words[i] + j, 1);
			if (errno != 0) {
				fprintf(stderr, "output error");
				exit(1);
			}
			j ++;
		}
        if (j != 0)
            write(1, &newline, 1);
		if (errno != 0) {
			fprintf(stderr, "output error");
			exit(1);
		}
	}
	
	// free dynamic variable
	free(text);
	free(words);

	fprintf(stderr, "Number of comparisons: %d\n", cmpCounter);
	return 0;
}
