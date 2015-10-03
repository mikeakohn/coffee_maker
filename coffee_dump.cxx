#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ClassReader.h"

int main(int argc, char *argv[])
{
  FILE *in;
  struct stat file_stat;
  uint8_t *buffer;

  printf("\ncoffee_dump by Michael Kohn\n"
         "http://www.mikekohn.net/\n\n");

  if (argc != 2)
  {
    printf("Usage: %s <filename.class>\n", argv[0]);
    exit(0);
  }

  in = fopen(argv[1], "rb");

  if (in == NULL)
  {
    printf("Could not open %s for reading\n", argv[1]);
    exit(1);
  }

  stat(argv[1], &file_stat);

  buffer = (uint8_t *)malloc(file_stat.st_size);

  if (fread(buffer, 1, file_stat.st_size, in) != (uint32_t)file_stat.st_size)
  {
    printf("Error: fread() couldn't read file\n");
  }

  fclose(in);

  ClassReader::dump_class(buffer);

  free(buffer);

  return 0;
}

