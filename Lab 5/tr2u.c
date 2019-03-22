#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  char* from = argv[1];
  char* to = argv[2];
  if(argc != 3)
    {
      fprintf(stderr,"Must have exactly 2 arguments!");
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
  
  char current[1];
  ssize_t rchar = read(0,current,1);
  int flag = 0;
  while(rchar > 0)
    {
      size_t k;
      for(k=0;k<strlen(from);k++)
        {
          if(current[0] == from[k])
            {
	      current[0] = to[k];
	      write(1,current,1);
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
          write(1,current,1);
	}
      rchar = read(0,current,1);
    }

  return 0;
}
