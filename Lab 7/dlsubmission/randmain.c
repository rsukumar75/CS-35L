#include "randcpuid.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Create handle for dynamic loading*/
  void* handle;
  char* error;
  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  unsigned long long (*rand64) (void);
  if (rdrand_supported ())
    {
      handle = dlopen("randlibhw.so", RTLD_LAZY);
      if(!handle)
	{
	  fprintf(stderr, "dlopen error: %s\n", dlerror());
	  return 1;
	}
      dlerror();
      rand64 = dlsym(handle,"rand64");
      if((error = dlerror() != NULL))
	{
	  fprintf(stderr, "dlsym randlibhw error: %s\n", error);
	  return 1;
	}
    }
  else
    {
      handle = dlopen("randlibsw.so", RTLD_LAZY);
      if(!handle)
	{
	  fprintf(stderr, "dlopen error: %s\n", dlerror());
	  return 1;
	}
      dlerror();
      rand64 = dlsym(handle,"rand64");
      if((error = dlerror() != NULL))
	{
	  fprintf(stderr, "dlsym randlibsw error: %s\n", error);
	  return 1;
	}
    }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if(fwrite(&x, 1, outbytes, stdout) != outbytes)
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if(dlclose(handle))
    {
      fprintf(stderr, "dlclose error: %s\n", dlerror());
      return 1;
    }
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }
  return 0;
}

