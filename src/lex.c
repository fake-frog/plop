#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "base.h"
#include "lex.h"
#include "errors.h"
/* ----------------- [OPEN FILE] ----------------- */

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

/* ----------------- [TOKEN CONSTANTS] ----------------- */

static const char *KIND_LUT[] = {
    [NYD] = "NYD", [SEP] = "SEP", [OPT] = "OPT", [LIT] = "LIT",
    [SYM] = "SYM", [KEY] = "KEY", [END] = "END", [SPC] = "SPC",
    [NLN] = "NLN"};

static const size_t kind_lut_len = sizeof(KIND_LUT) / sizeof(KIND_LUT[0]);

static const char *TYPE_LUT[] = {
    [NIL] = "NIL",
    [ADD] = "ADD",
    [MINUS] = "MINUS",
    [DIV] = "DIV",
    [NUM] = "NUM",
    [STR] = "STR",
    [CHR] = "CHR",
    [PROC] = "PROC"
};

static const size_t type_lut_len = sizeof(TYPE_LUT) / sizeof(TYPE_LUT[0]);

// KNOWN SYMBOLS
static const char *KEYWORDS[] = {"def", "for", "while"};
static const char OPERATORS[] = {'+', '-', '*', '/', '%', '^', '=', '&', '?', '!', '#'};
static const char SEPORATOR[] = {'(', ')', '[', ']', ',', ';', ':', '{', '}', '"', '\''};

static const size_t key_len = sizeof(KEYWORDS)  / sizeof(KEYWORDS[0]);
static const size_t opt_len = sizeof(OPERATORS) / sizeof(OPERATORS[0]);
static const size_t sep_len = sizeof(SEPORATOR) / sizeof(SEPORATOR[0]);

/* --- op procedures --- */

f32 add(f32 a, f32 b)    { return a + b; }
f32 sub(f32 a, f32 b)    { return a - b; }
f32 mult(f32 a, f32 b)   { return a * b; }
f32 divide(f32 a, f32 b) { return a / b; }

void print_token(Token t, char *buf) {
  const char *kind_string = "UNKNOWN KIND";
  const char *type_string = "UNKNOWN TYPE";
  
  if (t.kind > 0 || t.kind < kind_lut_len) kind_string = KIND_LUT[t.kind];
  if (t.value.tag > 0 || t.value.tag < type_lut_len) type_string = TYPE_LUT[t.value.tag];

  if (*(buf + t.offset) == '\n') {
    printf("NEWLINE ");
  } else if (*(buf + t.offset) == ' '){
    printf("SPACE ");
  } else {
    printf("%.*s ", t.len, buf + t.offset);
  }
  
  printf("- KIND: %s ", kind_string);
  
  switch (t.value.tag) {
  case NUM:
    printf("- TYPE: %s ", type_string);
    printf("- VALUE: %f \n", t.value.val.num_v);
    break;
  default:
    printf("- TYPE: %s\n", type_string);
  }
}

/* ----------------- [TOKEN PARSE] ----------------- */

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

/* --- get_token helpers --- */

static void append_token(Token *tokens, size_t *count, u32 *offset, u32 *len, TokenKind kind) {
  tokens[(*count)++] = (Token){*offset, *len, kind, {0}};
  *offset += *len;
  *len = 0;
}

static void append_token_before(Token *tokens, size_t *count, u32 *offset, u32 *len, TokenKind kind) {
  (*len)--;
  append_token(tokens, count, offset, len, kind); // resets token length to 0
  (*len)++;                                       // set back to 1 (not sure if len = 1 is more clear)
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
      // if there is an unappended token before the operator, we have to add it
      
      if (token_len > 1) {
        append_token_before(tokens, &count, &token_offset, &token_len, NYD);
      }
      append_token(tokens, &count, &token_offset, &token_len, OPT);
      GOTO_NEXT_CHAR(cp);
    }

    if (is_seporator(*cp)) {
      if (token_len > 1) {
        append_token_before(tokens, &count, &token_offset, &token_len, NYD);
      }
      append_token(tokens, &count, &token_offset, &token_len, SEP);
      GOTO_NEXT_CHAR(cp);
    }

    if (isspace(*cp)) {
      // skip whitespace
      // maybe we keep whitespaces? // only one whitespace?
      if (token_len == 1) { 
        if(*cp == '\n') {
          append_token(tokens, &count, &token_offset, &token_len, NLN);
	  GOTO_NEXT_CHAR(cp); 
        }
        // token_offset++;
        // token_len = 0;
	append_token(tokens, &count, &token_offset, &token_len, SPC);
        GOTO_NEXT_CHAR(cp); 
      }
      token_len--; // dont include the whitespace

      if (is_keyword(fb.buf, token_offset, token_len)) {
        append_token(tokens, &count, &token_offset, &token_len, KEY);
        continue; // dont go to next char, we will skip the whitespace next time around
      }
      
      append_token(tokens, &count, &token_offset, &token_len, NYD);
      continue;
    }

    GOTO_NEXT_CHAR(cp);
  }

  // flush last token
  if (token_len > 0) {
    append_token(tokens, &count, &token_offset, &token_len, NYD);
  }

  return (FileTokens){tokens, count, fb};
}

/* ----------------- [TOKEN PARSE] ----------------- */

static bool parse_num(f32 *dest, const char *str_start, const size_t len) {
  // TODO: find a way to use strof with a lenght of the string instead of '\0'
  char num_cpy[len + 1];
  strncpy(num_cpy, str_start, len);
  num_cpy[len] = '\0';
  char *end;
  *dest = strtof(num_cpy, &end);
  return (end != num_cpy) && (*end == '\0');
}

void parse_tokens(FileTokens *file_tokens) {
  for (size_t i = 0; i < file_tokens->token_count; i++) {
    const char *token_str = file_tokens->file_buf.buf + file_tokens->tokens[i].offset;
    const size_t token_len = file_tokens->tokens[i].len;
    if (isdigit(*token_str)) {
      f32 num = 0;
      if (!parse_num(&num, token_str, token_len)) {
        // TODO: Should probably impliment better error handling sooner than
        // later
        fprintf(stderr, "error parsing string to number: ");
        printf("%.*s \n", (int)token_len, token_str);
	FileLoc eloc= locateError(file_tokens->tokens[i], file_tokens->file_buf);
        printf("Line: %d Char %d\n", eloc.line_num, eloc.char_num);
      }
      file_tokens->tokens[i].kind = LIT;
      file_tokens->tokens[i].value.tag = NUM;
      file_tokens->tokens[i].value.val.num_v = num;
    }
  }
}
