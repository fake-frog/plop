#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char  *data;
  size_t len;
  size_t size; // alloc size
} String;

#define STACK_STRING(n) (String){(char[(n)]){0}, 0, (n)};

String string_from(char *cstr, size_t size) {
  return (String){
    .data = cstr,
    .len = strlen(cstr),
    .size = size,
  };
}

void string_append_char(String *str, char c) {
  if (str->len >= str->size) {
    printf("String: %.*s is out of room with the addtion of %c", (int)str->len, str->data, c);
    exit(1);
  }
  str->data[str->len] = c;
  str->len++;
}

void string_clear(String *str) { str->len = 0; }

String string_copy(char *data_dest, String *src_str) {
  memcpy(data_dest, src_str->data, src_str->len);
  return string_from(data_dest, src_str->size); // note: got to make these the same
}

typedef enum {
  SEP,
  OPT,
  LIT,
  SYM,
} TokenType;



typedef struct {
  String prev_token;
  String rel_token;
  String curr_token;
} Lexer;

typedef struct {
  String val;
  TokenType type;
} Token;

/*
void build_token(Lexer *lex, char c) {
  const int END_TOKEN = isspace(c);
  if (!END_TOKEN) {
    string_append_char(&lex->curr_token, c);
  }
}
*/

int main(int argc, char *argv[]) {

  char *file_path = NULL;
  
  if (argc > 1) {
    file_path = argv[1];
  } else {
    printf("NO FILE PATH GIVEN\n");
  }
  
  FILE *file = fopen(file_path, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }
  
  int raw_token_size = 1000;
  String raw_tokens[raw_token_size];
  int raw_token_index = 0;
  // char base_raw[16];
  // String curr_raw_token = string_from(base_raw, 256);
  String curr_raw_token = STACK_STRING(16);
  
  char c;
  while ((c = fgetc(file)) != EOF) {
    
    if (raw_token_index == raw_token_size){
      printf("Token Limit Reached\n");
      break;
    }
    
    if (isspace(c)) {
      raw_tokens[raw_token_index++] = string_copy((char[16]){0}, &curr_raw_token);
      string_clear(&curr_raw_token);
      continue;
    }
    string_append_char(&curr_raw_token, c);
  }
  raw_tokens[raw_token_index] = string_copy(calloc(256, sizeof(char)), &curr_raw_token);

  for (int i = 0; i <= raw_token_index; i++) {
    printf("%.*s\n", (int)raw_tokens[i].len, raw_tokens[i].data);
  }

  fclose(file);
}
