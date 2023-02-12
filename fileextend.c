#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// gcc fileextend.c -o fileextend.bin 

_Bool strcsp(char*str, const char*chars)  {
  unsigned long strlength = strlen(str);
  unsigned long charslength = strlen(chars);
  unsigned long j, i;
  char strj;
  _Bool matchfound = true;
  for (j = 0; j < strlength && matchfound; j++) {
    matchfound = false;
    strj = str[j];
    for (i = 0; i < charslength && !matchfound; i++) {
	if (strj == chars[i]) matchfound = true;
    }
  } 
  return matchfound;
}

int main(int argc, char*argv[])
{
  if (argc <= 2) {
    printf("Usage %s filename filesizeinhex \nThis program appends 0xff bytes to a file to make it filesizeinhex bytes long. The source file is overwritten.\n", argv[0]);
    exit(0);
  }
  const char*filechars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHILJKLMNOPQRSTUVWXYZ_./";
  if (!strcsp(argv[1], filechars)) {
    printf("Filename invalid.\n");
    exit(0);
  }
  const char*hexchars = "0123456789";
  if ((strlen(argv[2]) > 10) || (argv[2][0] != '0') || (argv[2][1] != 'x') || !strcsp(&argv[2][2], hexchars)) {
    printf("Filesizeinhex invalid.\n");
    exit(0);
  }
  uint64_t i, j, imax, jmax, initfilesize, newfilelen = strtol(argv[2], NULL, 16);
  printf("New file length = %li bytes.\n", newfilelen);
  FILE *fp = fopen(argv[1], "rb+");
  if (fp == NULL) {
    printf("Filename %s not found.\n", argv[1]);
    exit(0);
  }
  size_t blocks;
  fseek(fp, 0, SEEK_END);
  initfilesize = ftell(fp);
  if (initfilesize > newfilelen) {
    printf("File already contains %li bytes.\n", initfilesize);
    fclose(fp);
    exit(0);
  } else {
    printf("File contains %li bytes.\n", initfilesize);
  }
  char *output_buffer = malloc(4096);
  memset(output_buffer, 0xff, 4096);
  imax = (newfilelen - initfilesize)/4096;
  jmax = (newfilelen - initfilesize) - imax*4096;
  for (i=0; i<imax; i++) {
    blocks = fwrite(output_buffer, 4096, 1, fp);
    if (blocks != 1) {
      fprintf(stderr, "Error writing to file %s. Operation aborted.\n", argv[2]);
      free(output_buffer);
      fclose(fp);
      exit(1);
    }
  }
  for (j=0; j<jmax; j++) {
    blocks = fwrite(output_buffer, 1, 1, fp);
    if (blocks != 1) {
      fprintf(stderr, "Error writing to file %s. Operation aborted.\n", argv[2]);
      free(output_buffer);
      fclose(fp);
      exit(1);
    }
  }
  free(output_buffer);
  fclose(fp);
  printf("Operation completed successfully.\n");
}
