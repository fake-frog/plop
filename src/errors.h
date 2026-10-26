#ifndef ERRORS_H
#define ERRORS_H
#include "base.h"
#include "lex.h"

typedef struct {
  u32 line_num;
  u32 char_num;
} FileLoc;

FileLoc locateError(Token t, FileBuffer fb);

#endif
