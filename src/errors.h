#ifndef ERRORS_H
#define ERRORS_H
#include "base.h"
#include "lex.h"

typedef struct {
  u32 line_num;
  u32 char_num;
} FileLoc;

FileLoc locate_error(u32 offset, FileBuffer fb);
void print_located_error(char *msg, FileLoc eloc, const char *file_name,
                         const char *str, const int str_len);
#endif
