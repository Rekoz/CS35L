#include<stdio.h>
#include<stdlib.h>

int rot13cmp(const char *a, const char *b)
{
  while (*a == *b && *a != '\n' && *b != '\n')
    { a++; b++; }
  if (*a == *b) return 0; /* two words are identical */
  if (*a == '\n') return -1; /* length of first word is shorter */
  if (*b == '\n') return 1; /* length of second word is shorter */
  char x=*a, y=*b;
  /* decode the current characters and compare */
  if ((x >= 'A' && x <='M') || (x >= 'a' && x <= 'm')) x+=13;
  else if ((x >= 'N' && x <='Z') || (x >= 'n' && x <= 'z')) x-=13;
  if ((y >= 'A' && y <='M') || (y >= 'a' && y <= 'm')) y+=13;
  else if ((y >= 'N' && y <='Z') || (y >= 'n' && y <= 'z')) y-=13;
  return x-y;
}

int cmp( const void *a, const void *b)
{
  return rot13cmp( *(const char**)a, *(const char**)b );
}

int main() {
  int bsize=2048; /* define buffer size */
  char *buffer = (char*) malloc(sizeof(char)*bsize);
  if (!buffer)
    {
      fprintf(stderr, "Memory allocation failed.");
      exit(1);
    }
  int cc=0, nc=0; /* cc==character count, nc==newline count */
  int c = getchar();
  /* read from stdin */
  while ( c != EOF )
    {
      buffer[cc++]=(char)c;
      if (c=='\n') nc++;
      if (cc == bsize)
	{
	  bsize *= 2;
	  buffer = (char*) realloc(buffer, bsize);
	  if (!buffer)
	    {
	      fprintf(stderr, "Memory allocation failed.");
	      exit(1);
	    }
	}
      c = getchar();
    }
  if (cc == 0) exit(0); /* if stdin is empty */
  /* if input does not end with \n */
  if (buffer[cc-1] != '\n') 
    {
      buffer[cc++]='\n';
      nc++;
    }
  char **words = (char**) malloc(sizeof(char*) * nc);
  if (!words)
    {
      fprintf(stderr, "Memory allocation failed.");
      exit(0);
    }
  words[0]=buffer;
  int i, j = 1;
  /* an array of pointers pointing to the start of each word */
  for (i = 1; i < cc-1; i++)
    if (buffer[i] == '\n') words[j++]=&buffer[i+1];
  qsort(words, nc, sizeof(char*), cmp);
  char *ch;
  /* output */
  for (i = 0; i < nc; i++)
    {
      ch = words[i];
      while (*ch != '\n')
	{
	  putc(*ch, stdout);
	  ch++;
	}
      putc(*ch, stdout);
    }
  /* release memory */
  free(buffer);
  free(words);
  return 0;
}
