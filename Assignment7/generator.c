#include <stdlib.h>
#include <stdio.h>

int main( int argc, char *argv[]){
  int n = atoi(argv[1]);
  srand(time(NULL));
  int i;
  for (i = 0; i < n; i++)
    {
      int r = rand();
      printf("%d\n", r);
    }
}
