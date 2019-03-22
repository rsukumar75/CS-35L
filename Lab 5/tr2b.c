#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char* from = argv[1];
  char* to = argv[2];
  if(argc != 3)
    {
      fprintf(stderr, "Must have exactly 2 arguments!");
      exit(1);
    }

  if(strlen(from) != strlen(to))
    {
      fprintf(stderr, "Both arguments are not of the same length!");
      exit(1);
    }
  
  size_t i,j;
  for(i=0;i<strlen(from);i++)
    {
      for(j=i+1;j<strlen(from);j++)
	{
	  if(from[i]==from[j])
	    {
	      fprintf(stderr,"There are duplicate bytes in the from input!");
	      exit(1);
	    }
	}
    }
  
  char current = getchar();
  int flag = 0;
  while(current != EOF)
    {
      size_t k;
      for(k=0;k<strlen(from);k++)
	{
	  if(current == from[k])
	    {
	      putchar(to[k]);
	      flag = 1;
	      break;
	    }
	  else
	    {
	      flag = 0;
	    }
	}
      if(!flag)
	{
	  putchar(current);
	}
      current = getchar();
    }
  return 0;
}
