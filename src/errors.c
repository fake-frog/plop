#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "base.h"
#include "errors.h"

FileLoc locateError(Token t, FileBuffer fb) {
  char *cp = fb.buf;
  u32 line_count = 1;
  u32 char_count = 0;
  
  for(size_t i = 0; i < t.offset; i++) {
    if (*cp == '\n') {
      line_count++;
      char_count = 0;
    }
    char_count++;
    cp++;
  }

  return (FileLoc){line_count, char_count};
}
