#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

int fflag = 0;
char unfrob(char const *a)
{
	if(fflag == 1)
	{
		if(*a == EOF || (*a >= 0 && *a < UCHAR_MAX))
		{
			return toupper(*a ^ 42);
		}
		else
		{
			fprintf(stderr, "Character is larger than UCHAR_MAX");
			exit(1);
		}
	}
	else
	{
		return *a ^ 42;
	}
}

int frobcmp(const void *x, const void *y)
{
  const char* a = *((char **) x);
  const char* b = *((char **) y);
  for(;; a++, b++)
    {
    	if(*a ==  ' ' && *b == ' ')
	{
		return 0;
	}
	else if(*a == ' ' || (unfrob(a) < unfrob(b)))
	{
		return -1;
	}
	else if(*b == ' ' || (unfrob(a) > unfrob(b)))
	{
		return 1;
	}
    }
}

int main(int argc, char** argv)
{
	char c;
	fflag = 0;
	while((c = getopt(argc, argv, "f")) != -1)
	{
		switch(c)
		{
			case 'f':
				fflag = 1;
				break;
			case '?':
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				exit(1);
				break;
		}
	}
	struct stat fileStat;
	if(fstat(0,&fileStat) < 0)
	{
		fprintf(stderr, "Error with filestat!");
		exit(1);
	}
	int (*frobcmp_ptr)(const void *, const void *);
	frobcmp_ptr = &frobcmp;
	char* files;
	size_t wordsindex = 0;
	char** words;
	int wordcount = 0;
	if(S_ISREG(fileStat.st_mode))
	{
		files = (char *) malloc(fileStat.st_size*sizeof(char));
		ssize_t rfile = read(0,files,fileStat.st_size);
		size_t i;
		int count = 1;
		if(rfile > 0)
		{
			for(i=0;i < fileStat.st_size; i+=count)
			{
				count = 1;
				if(i == fileStat.st_size-1)
				{
					files[i] = ' ';
				}
				if(files[i] == ' ')
				{
					size_t j;
					for(j=i;files[j] != ' '; j++)
					{ 
						count++;
					}
					wordcount++;
				}
				
			}
		}	
		words = (char **) malloc(wordcount * sizeof(char *));
		size_t j;
		int flag = 0;
		for(j = 0; j < fileStat.st_size; j++)
		{
			if(flag == 0 && files[j] != ' ')
			{
				words[wordsindex] = &files[j];
				wordsindex++;
				flag = 1;
			}
			else if(flag == 1 && files[j] == ' ')
			{
				flag = 0;
			}
		}
	}
	
	else
	{
		words = (char **) malloc(sizeof(char*));
	}
	
	char* word;
	word = (char *) malloc(sizeof(char));
	size_t wordindex = 0;
	char curr[1];
	char next[1];
	ssize_t rcurr = read(0,curr,1);
	if(rcurr < 0)
	{
		fprintf(stderr, "Error while reading input");
		exit(1);
	}
	while(rcurr > 0)
	{
		ssize_t rnext = read(0,next,1);
		if(rnext < 0)
		{
			fprintf(stderr, "Error while reading input!");
			exit(1);
		}
		if(next[0] == ' ' && curr[0] == ' ')
		{
			curr[0] = next[0];
			continue;
		}
		else if(rnext != 0)
		{
			word[wordindex] = curr[0];
			if(curr[0] == ' ')
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
					word = (char*) malloc(sizeof(char));
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
		else if(rnext == 0)
		{
			word[wordindex] = curr[0];
			wordindex++;
			if(curr[0] == ' ')
			{
				words[wordsindex] = word;
				wordsindex++;
			}
			else if(curr[0] != ' ')
			{
				word[wordindex] = ' ';
				words[wordsindex] = word;
				wordindex++;
				wordsindex++;
			}
			break;
		}
		curr[0] = next[0];
	}
	qsort(words, wordsindex, sizeof(char *), frobcmp_ptr);
	size_t i;
	size_t j;
	for(i = 0; i < wordsindex; i++)
	{
		size_t j;
		long wordSize = 0;
		for(j=0;;j++)
		{
			wordSize++;
			if(words[i][j] == ' ')
        	        {
        	                break;
        	        }
		}
		if(write(1,words[i],wordSize) == 0)
		{
			fprintf(stderr, "Error while writing word!");
			exit(1);
		}
	}
	size_t i1;
	free(words);
	exit(0);
}

