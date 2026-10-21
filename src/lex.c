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

char *KIND_LUT[] = {[SEP] = "SEP", [OPT] = "OPT", [LIT] = "LIT",
                    [SYM] = "SYM", [KEY] = "KEY", [NIL] = "NIL"};

static const size_t kind_lut_len = sizeof(KIND_LUT) / sizeof(KIND_LUT[0]);

void print_token(Token t, char *buf) {
  char *kind_string = "UNKNOWN";
  if (t.kind > 0 || t.kind < kind_lut_len) kind_string = KIND_LUT[t.kind];
  printf("%.*s", t.len, buf + t.offset);
  printf(" -- %s\n", kind_string);
}

// KNOWN SYMBOLS
static const char *KEYWORDS[] = {"def", "for", "while"};
static const char OPERATORS[] = {'+', '-', '*', '/', '%', '^', '=', '&', '?', '!', '#'};
static const char SEPORATOR[] = {'(', ')', '[', ']', ',', ';', ':', '{', '}', '"', '\''};

static const size_t key_len = sizeof(KEYWORDS)  / sizeof(KEYWORDS[0]);
static const size_t opt_len = sizeof(OPERATORS) / sizeof(OPERATORS[0]);
static const size_t sep_len = sizeof(SEPORATOR) / sizeof(SEPORATOR[0]);
// returns true if it found a know symbol
static bool is_keyword(char *buf, u32 offset, u32 len) {
  for (size_t i = 0; i < key_len; i++) {
    int cmp = strncmp(KEYWORDS[i], buf + offset, len);
    if (cmp == 0) return true;
  }
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

// append and update the offset and length
static void append_token_then_offset(Token *tokens, size_t *count, u32 *offset, u32 *len, TokenKind kind) {
  tokens[(*count)++] = (Token){*offset, *len, kind};
  *offset += *len;
  *len = 0;
}

#define GOTO_NEXT_CHAR(cp) (cp)++; continue;

FileTokens get_tokens(FileBuffer fb) {
  Token *tokens = (Token *)malloc(sizeof(Token) * fb.len);
  if(!tokens) return (FileTokens){0};

  char *cp = fb.buf;
  u32 token_len = 0;
  u32 token_offset = 0;
  size_t count = 0;
  while (*cp) {
    token_len++; // we always have a token length of at least 1

    // look for an operator or seporator
    if (is_operator(*cp)) {
      // append first token at len - 1 then append the operator
      if (token_len > 1) {
        token_len--;
        append_token_then_offset(tokens, &count, &token_offset, &token_len, LIT); // resets token length to 0
        token_len++;                                                              // set back to 1 (not sure if token_len = 1 is more clear)
      }
      append_token_then_offset(tokens, &count, &token_offset, &token_len, OPT);
      GOTO_NEXT_CHAR(cp);
    }

    if (is_seporator(*cp)) {
      if (token_len > 1) {
        token_len--;
        append_token_then_offset(tokens, &count, &token_offset, &token_len, LIT);
        token_len++;
      }
      append_token_then_offset(tokens, &count, &token_offset, &token_len, SEP);
      GOTO_NEXT_CHAR(cp);
    }

    if (isspace(*cp)) {
      // skip whitespace
      if (token_len == 1) { 
        token_offset++;
        token_len = 0;
        GOTO_NEXT_CHAR(cp); 
      }
      token_len--; // dont include the whitespace

      if (is_keyword(fb.buf, token_offset, token_len)) {
        append_token_then_offset(tokens, &count, &token_offset, &token_len, KEY);
        continue; // dont go to next char, we will skip the whitespace next time around
      }
      
      append_token_then_offset(tokens, &count, &token_offset, &token_len, LIT);
      continue;
    }

    GOTO_NEXT_CHAR(cp);
  }

  // flush last token
  if (token_len > 0) {
    append_token_then_offset(tokens, &count, &token_offset, &token_len, LIT);
  }
  
  return (FileTokens){tokens, count, fb};
}
