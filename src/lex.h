#ifndef LEX_H
#define LEX_H

#include "base.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  SEP,
  OPT,
  LIT,
  SYM,
} TokenKind;

typedef struct {
  i32 offset;
  i32 len;
  TokenKind kind;
} Token;

typedef struct {
  char *buf;
  size_t len;
} FileBuffer;

typedef struct {
  Token *tokens;
  const FileBuffer file_buf;
} FileTokens;

FileBuffer read_file(const char *path);
void print_token(Token t, char *buf);
FileTokens get_tokens(FileBuffer buf);

#endif
