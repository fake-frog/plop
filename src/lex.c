#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "base.h"
#include "lex.h"

FileBuffer read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) return (FileBuffer){NULL, 0};

  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  rewind(f);

  char *buf = malloc(len + 1);
  if (!buf) { fclose(f); return (FileBuffer){NULL, 0}; }
  
  size_t n = fread(buf, 1, len, f);
  buf[n] = '\0';
  fclose(f);

  return (FileBuffer){buf, n};
}

void print_token(Token t, char *buf) {
  printf("%.*s\n", t.len, buf + t.offset);
}

FileTokens get_tokens(FileBuffer fb) {
  Token *tokens = (Token *)malloc(sizeof(Token) * fb.len);

  char *cp = fb.buf;
  i32 token_len;
  while (*cp) {
    Token t = {0};
    if (isspace(*cp)) {
      if(token_len == 0) continue;
    }
    cp++;
  }
  return (FileTokens){tokens, fb};
}
