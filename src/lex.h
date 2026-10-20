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
  KEY,
  NIL,
} TokenKind;

typedef struct {
  u32 offset;
  u32 len;
  TokenKind kind;
} Token;

typedef struct {
  char *buf;
  size_t len;
} FileBuffer;

typedef struct {
  Token *tokens;
  size_t token_count;
  const FileBuffer file_buf;
} FileTokens;

FileBuffer read_file(const char *path);
void print_token(Token t, char *buf);
FileTokens get_tokens(FileBuffer buf);

#endif
