#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"


int main(int argc, char *argv[]) {

  char *file_path;
  
  if (argc > 1) {
    file_path = argv[1];
  } else {
    printf("NO FILE PATH GIVEN\n");
    return 1;
  }

  FileBuffer file_buffer = read_file(file_path);
  if (!file_buffer.buf) {
    printf("No file by that name\n");
    return 1;
  }
  
  FileTokens ft = get_tokens(file_buffer);


  printf("\n---------- BEGIN PROGRAM ----------\n\n");
  printf("Token Length: %d\n", (int)ft.token_count);
  for (size_t i = 0; i < ft.token_count; i++) {
    print_token(ft.tokens[i], ft.file_buf.buf);
  }
  printf("\n----------- END PROGRAM -----------\n\n");
}
