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

void print_located_error(char *msg, FileLoc eloc, const char *file_name, const char *str, const int str_len) {
  // FIXME: apparently passing escape sequnces into stderr is bad?
  fprintf(stderr, "\x1b[31m---------\n| ERROR | \x1b[0m%s [%d:%d] '%.*s' -> %s \x1b[31m\n---------\x1b[0m\n", file_name, eloc.line_num, eloc.char_num, str_len, str, msg);
}

