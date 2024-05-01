#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define S_EQ(str, str2) (str && str2 && (strcmp(str, str2) == 0))
struct pos {
  int line;
  int col;
  const char *filename;
};
//  MACRO  qa on fait premier étape puis q pour arrêter répeter @a
//  v: visual mode: first line 0 cursor then 8j  then g then CTRL-a
#define NUMERIC_CASE                                                           \
  case '1':                                                                    \
  case '2':                                                                    \
  case '3':                                                                    \
  case '4':                                                                    \
  case '5':                                                                    \
  case '6':                                                                    \
  case '7':                                                                    \
  case '8':                                                                    \
  case '9'

#define SYMBOL_CASE                                                            \
  case '{':                                                                    \
  case '}':                                                                    \
  case ':':                                                                    \
  case ';':                                                                    \
  case '#':                                                                    \
  case '\\':                                                                   \
  case ')':                                                                    \
  case ']'

#define OPERATOR_CASE_EXCLUDING_DIVISION                                       \
  case '+':                                                                    \
  case '-':                                                                    \
  case '*':                                                                    \
  case '>':                                                                    \
  case '<':                                                                    \
  case '^':                                                                    \
  case '%':                                                                    \
  case '!':                                                                    \
  case '=':                                                                    \
  case '~':                                                                    \
  case '|':                                                                    \
  case '&':                                                                    \
  case '(':                                                                    \
  case '[':                                                                    \
  case ',':                                                                    \
  case '.':                                                                    \
  case '?'

enum { LEXICAL_ANALYSIS_ALL_OK, LEXICAL_ANALYSIS_INPUT_ERROR };
enum {
  TOKEN_TYPE_IDENTIFIER,

  TOKEN_TYPE_KEYWORD,
  TOKEN_TYPE_OPERATOR,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_COMMENT,
  TOKEN_TYPE_NEWLINE
};
struct token {
  int type;
  int flags;
  struct pos pos;
  // union is memory shared
  union {
    char cval;
    const char *sval;
    unsigned int inum;
    unsigned long lnum;
    unsigned long long llnum;
    void *any;
  };
  bool whitespace;
  //{hello world}
  const char *between_brackets;
};
struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process *process);

typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process *process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process *process, char c);
struct lex_process_functions {
  LEX_PROCESS_NEXT_CHAR next_char;
  LEX_PROCESS_PEEK_CHAR peek_char;
  LEX_PROCESS_PUSH_CHAR push_char;
};
struct lex_process {
  struct pos pos;
  struct vector *token_vec;
  struct compile_process *compiler;
  // ex: ((50))  2
  int current_expression_count;
  struct buffer *parenthesis_buffer;
  struct lex_process_functions *function;

  void *privatee;
};
enum { COMPILER_FILE_COMPILED_OK, COMPILER_FAILED_WITH_ERRORS };
struct compile_process {
  // the flags in regards to how this file should be compiled
  int flags;
  struct pos pos;
  struct compile_process_input_file {
    FILE *fp;
    const char *abs_path;
  } cfile;
  FILE *ofile;
};
int compile_file(const char *filename, const char *out_filename, int flags);

struct compile_process *compile_process_create(const char *filename,
                                               const char *filename_out,
                                               int flags);

char compile_process_next_char(struct lex_process *lex_process);

char compile_process_peek_char(struct lex_process *lex_process);
void compile_process_push_char(struct lex_process *lex_process, char c);

struct lex_process *lex_process_create(struct compile_process *compiler,
                                       struct lex_process_functions *functions,
                                       void *privatee);

void lex_process_free(struct lex_process *process);

void *lex_process_private(struct lex_process *process);

struct vector *lex_process_tokens(struct lex_process *process);

int lex(struct lex_process *process);
void compiler_error(struct compile_process *compiler, const char *msg, ...);
void compiler_warning(struct compile_process *compiler, const char *msg, ...);

bool token_is_keyword(struct token *token, const char *value);

struct lex_process *tokens_build_for_string(struct compile_process *compiler,
                                            const char *str);
#endif
