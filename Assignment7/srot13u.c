#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>

int ncom = 0;

int rot13cmp(const char *a, const char *b)
{
  ncom++;
  if (a == NULL || b == NULL) fprintf(stderr, "a or b null");
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

int main( int argc, char *argv[]) {
  if (argc != 1)
    {
      fprintf(stderr, "Wrong number of arguments!");
      exit(1);
    }
  int bsize=2048; /* define buffer size */
  struct stat fileStats;
  fstat(0, &fileStats);
  char *buffer;
  if (S_ISREG(fileStats.st_mode))
    bsize = fileStats.st_size+1;/* +1 in case the last character is not newline*/
  buffer = (char*) malloc(sizeof(char)*bsize);
  if (!buffer)
    {
      fprintf(stderr, "Memory allocation failed.");
      exit(1);
    }
  int cc=0, nc=0; /* cc==character count, nc==newline count */
  /* read from stdin */
  char c;
  while ( read(0, &c, 1)>0 ) 
    {
      buffer[cc++] = c;
      if (c =='\n') nc++;
      if (cc == bsize) /* reallocate memory */
	{
	  bsize <<= 1;
	  buffer = (char*) realloc(buffer, bsize);
	  if (!buffer)
	    {
	      fprintf(stderr, "Memory allocation failed.");
	      exit(1);
	    }
	}
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
  int i, j = 0;
  if (buffer[0] != '\n') words[j++]=buffer;
  /* an array of pointers pointing to the start of each word */
  for (i = 1; i < cc-1; i++)
    if (buffer[i] == '\n' && buffer[i+1] != '\n')
      {
	words[j++]=buffer+i+1;
	if (!words[j-1]) fprintf(stderr, "NULL ptr!");
      }
  qsort(words, j, sizeof(char*), cmp);
  char *ch;
  /* output */
  for (i = 0; i < j; i++)
    {
      ch = words[i];
      while (*ch != '\n')
	{
	  write(1, ch, 1);
	  ch++;
	}
      write(1, ch, 1);
    } 
  fprintf(stderr, "Number of comparisons: %d\n", ncom);
  /* release memory */
  free(buffer);
  free(words);
  return 0;
}
