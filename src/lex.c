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

static const char *KEYWORDS[] = {"def", "for", "while"};
static const char OPERATORS[] = {'+', '-', '*', '/', '%', '^'};
static const char SEPORATOR[] = {'(', ')', '[', ']', ','};

static const size_t key_len = sizeof(KEYWORDS)  / sizeof(KEYWORDS[0]);
static const size_t opt_len = sizeof(OPERATORS) / sizeof(OPERATORS[0]);
static const size_t sep_len = sizeof(SEPORATOR) / sizeof(SEPORATOR[0]);
// returns true if it found a know symbol
static bool is_keyword(char *buf, i32 offset, i32 len) {
  //TODO ... 
  return false;
}

static bool is_operator(char c) {
  for (size_t i = 0; i < opt_len; i++) {
    if (c == OPERATORS[i]) return true;
  }
  return false;
}

static bool is_seporator(char c) {
  for (size_t i = 0; i < sep_len; i++) {
    if (c == SEPORATOR[i]) return true;
  }
  return false;
}

// append and update the offset and len
static void append_token(Token *tokens, size_t *count, i32 *offset, i32 *len, TokenKind kind) {
  tokens[(*count)++] = (Token){*offset, *len, kind};
  *offset += *len;
  *len = 0;
}

#define GOTO_NEXT_CHAR(cp) (cp)++; continue;

FileTokens get_tokens(FileBuffer fb) {
  Token *tokens = (Token *)malloc(sizeof(Token) * fb.len);

  char *cp = fb.buf;
  i32 token_len = 0;
  i32 token_offset = 0;
  size_t count = 0;
  while (*cp) {
    token_len++; // we always have a token length of at least 1
                 
    // look for an operator or seporator
    if (is_operator(*cp)) {
      append_token(tokens, &count, &token_offset, &token_len, OPT);
      GOTO_NEXT_CHAR(cp);
    }

    if (is_seporator(*cp)) {
      append_token(tokens, &count, &token_offset, &token_len, SEP);
      GOTO_NEXT_CHAR(cp);
    }

    if (isspace(*cp)) {
      //skip whitespace
      if (token_len == 1) { 
        token_offset++;
        GOTO_NEXT_CHAR(cp); 
      } 
      token_len--; // ^ will skip next loop

      if (is_keyword(fb.buf, token_offset, token_len)) {
        append_token(tokens, &count, &token_offset, &token_len, KEY);
        GOTO_NEXT_CHAR(cp);
      }
      
      append_token(tokens, &count, &token_offset, &token_len, LIT);
      GOTO_NEXT_CHAR(cp);
    }
    
    GOTO_NEXT_CHAR(cp);
  }
  // TODO: flush last token
  return (FileTokens){tokens, count, fb};
}
