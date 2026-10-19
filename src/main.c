#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.c"

/*
void build_token(Lexer *lex, char c) {
  const int END_TOKEN = isspace(c);
  if (!END_TOKEN) {
    string_append_char(&lex->curr_token, c);
  }
}
*/

int main(int argc, char *argv[]) {

  char *file_path;
  
  if (argc > 1) {
    file_path = argv[1];
  } else {
    printf("NO FILE PATH GIVEN\n");
    return 1;
  }
  

  FileBuffer file_buffer = read_file(file_path);

  Token t = {2, 5, SYM};

  print_token(t, file_buffer.buf);
  printf("%s", file_buffer.buf);
  
  /*
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
  */
}
