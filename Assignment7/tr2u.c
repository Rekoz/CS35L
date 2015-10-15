#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char * argv[])
{
  if (argc != 3) 
    {
      fprintf(stderr, "Wrong number of operands.\n");
      exit(1);
    }
  int len1=strlen(argv[1]), len2=strlen(argv[2]);
  if (len1 != len2)
    {
      fprintf(stderr, "Two operands have different lengths!\n");
      exit(1);
    }
  int i, j;
  char map[256];
  int flag[256];
  for (i = 0; i < 256; i++)
    {
      map[i]=(char)i;
      flag[i]=0;
    }
  for (i = 0; i < len1; i++)
    {
      j = (int)argv[1][i];
      if (!flag[j])
	{
	  map[j]=map[argv[2][i]];
	  flag[j]=1;
     	}
      else
	{
	  fprintf(stderr, "Duplicate characters!\n");
	  exit(1);
	}
    }
  char c[1];
  while ( read(0, c, 1)>0 )
    write(1, &map[(int)*c], 1);
  return 0;
}
