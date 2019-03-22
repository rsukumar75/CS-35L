#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

char unfrob(char const *a)
{
  return *a ^ 42;
}
int frobcmp(const void *x, const void *y)
{
  const char* a = *((char **) x);
  const char* b = *((char **) y);
  for(;; a++, b++)
    {
      if(*a == ' ' && *b == ' ')
	return 0;
      else if(*a == ' ' || (unfrob(a) < unfrob(b)))
	return -1;
      else if(*b == ' ' || (unfrob(a) > unfrob(b)))
	return 1;
    }
}

int main()
{
  int (*frobcmp_ptr)(const void *, const void *);
  frobcmp_ptr = &frobcmp;
  char* word;
  char** words;
  word = (char *) malloc(sizeof(char));
  words = (char **) malloc(sizeof(char *));
  int wordindex = 0;
  int wordsindex = 0;
  char curr;
  char next;
  curr = getchar();
  if(ferror(stdin))
    {
      fprintf(stderr, "Invalid input");
      exit(1);
    }
  while(1) 
    {
      next = getchar();
      if(ferror(stdin))
	{
	  fprintf(stderr, "Invalid input");
	  exit(1);
	}
      if(next == ' ' && curr == ' ')
	{
	  curr = next;
	  continue;
	}
      else if(next != EOF)
	{
	  word[wordindex] = curr;
	  if(curr == ' ')
	    {
	      words[wordsindex] = word;
	      char** temp;
	      temp = realloc(words, (wordsindex + 2)*sizeof(char *));
	      if(temp == NULL)
		{
		  free(words);
		  fprintf(stderr, "Reallocation of memory failed");
		  exit(1);
		}
	      else
		{
		  wordsindex++;
		  words = temp;
		  word = NULL;
		  word = (char*)malloc(sizeof(char));
		  wordindex = 0;
		}
	    }
	  else 
	    {
	      wordindex++;
	      char* temp;
	      temp = realloc(word, (wordindex+1) * sizeof(char));
	      if(temp == NULL)
		{
		  free(word);
		  fprintf(stderr, "Reallocation of memory failed!");
		  exit(1);
		}
	      else
		{
		  word = temp;
		}
	    }
	}
      else if(next == EOF)
	{
	  word[wordindex] = curr;
	  wordindex++;
	  if(curr == ' ')
	    {
	      words[wordsindex] = word;
	      wordsindex++;
	    }
	  else if(curr != ' ')
	    {
	      word[wordindex] = ' ';
	      words[wordsindex] = word;
	      wordindex++;
	      wordsindex++;
	    }
	  break;
	}      
      curr = next;
    }

 
  qsort(words, wordsindex, sizeof(char *), frobcmp_ptr);  
  size_t i;
  size_t j;
  for(i = 0; i < wordsindex; i++)
  {
    for(j = 0;; j++)
    {
      int e = putchar(words[i][j]);
      if(e == EOF)
      {
	fprintf(stderr, "Failed to write character to stdout!");	  
	exit(1);
      }
      if(words[i][j] == ' ')
	break;
    }
  }
  size_t i1;
  for(i1 = 0; i1 < wordsindex; i1++)
  {
    free(words[i1]);
  }
  free(words);

  exit(0);
}
