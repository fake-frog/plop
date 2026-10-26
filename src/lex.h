#ifndef LEX_H
#define LEX_H

#include "base.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  END, // not sure I need this
  NYD, // Not Yet Definded
  SEP,
  OPT,
  LIT,
  SYM,
  KEY,
} TokenKind;

typedef enum {
  NIL,
  ADD,
  MINUS,
  MULT,
  DIV,
  NUM,
  STR,
  CHR,
  PROC,
} TokenType;

typedef struct {
  TokenType tag;
  union {
    f32 num_v;
    u8 char_v;
    u8 *str_v; // TODO: make a better string type
    f32 (*num_op)(f32 a, f32 b);
  } val;
} TokenValue;

typedef struct {
  u32 offset;
  u32 len;
  TokenKind kind;
  TokenValue value;
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

typedef struct TNode TNode;

struct TNode {
  Token value;
  TNode *next;
};

FileBuffer read_file(const char *path);
void print_token(Token t, char *buf);
FileTokens get_tokens(FileBuffer buf);
void assign_token_type(FileTokens *file_tokens);

#endif
