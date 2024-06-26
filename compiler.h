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
#define NUMERIC_CASE \
  case '1':          \
  case '2':          \
  case '3':          \
  case '4':          \
  case '5':          \
  case '6':          \
  case '7':          \
  case '8':          \
  case '9'

#define SYMBOL_CASE \
  case '{':         \
  case '}':         \
  case ':':         \
  case ';':         \
  case '#':         \
  case '\\':        \
  case ')':         \
  case ']'

#define OPERATOR_CASE_EXCLUDING_DIVISION \
  case '+':                              \
  case '-':                              \
  case '*':                              \
  case '>':                              \
  case '<':                              \
  case '^':                              \
  case '%':                              \
  case '!':                              \
  case '=':                              \
  case '~':                              \
  case '|':                              \
  case '&':                              \
  case '(':                              \
  case '[':                              \
  case ',':                              \
  case '.':                              \
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
enum {
  NUMBER_TYPE_NORMAL,
  NUMBER_TYPE_LONG,
  NUMBER_TYPE_FLOAT,
  NUMBER_TYPE_DOUBLE

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
  struct token_number {
    int type;
  } num;
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
  struct vector *token_vec;

  struct vector *node_tree_vec;

  struct vector *node_vec;
  FILE *ofile;
};
enum { PARSE_ALL_OK, PARSE_GENERAL_ERROR };
enum {
  NODE_TYPE_EXPRESSION,

  NODE_TYPE_EXPRESSION_PARENTHESIS,
  NODE_TYPE_NUMBER,
  NODE_TYPE_IDENTIFIER,
  NODE_TYPE_STRING,
  NODE_TYPE_VARIABLE,
  NODE_TYPE_VARIABLE_LIST,

  NODE_TYPE_FUNCTION,

  NODE_TYPE_BODY,

  NODE_TYPE_STATEMENT_RETURN,

  NODE_TYPE_STATEMENT_IF,

  NODE_TYPE_STATEMENT_ELSE,

  NODE_TYPE_STATEMENT_WHILE,

  NODE_TYPE_STATEMENT_DO_WHILE,

  NODE_TYPE_STATEMENT_FOR,

  NODE_TYPE_STATEMENT_BREAK,
  NODE_TYPE_STATEMENT_CONTINUE,

  NODE_TYPE_STATEMENT_SWITCH,

  NODE_TYPE_STATEMENT_CASE,

  NODE_TYPE_STATEMENT_GOTO,

  NODE_TYPE_STATEMENT_DEFAULT,

  NODE_TYPE_UNARY,

  NODE_TYPE_TENARY,

  NODE_TYPE_LABEL,

  NODE_TYPE_STRUCT,

  NODE_TYPE_UNION,

  NODE_TYPE_BRACKET,

  NODE_TYPE_CAST,

  NODE_TYPE_BLANK,
};
enum { NODE_FLAG_INSIDE_EXPRESSION = 0b00000001 };
struct exp {
  struct node *left;
  struct node *right;
  const char *op;
};
struct node {
  int type;
  int flags;
  struct pos pos;
  struct node_binded {
    struct node *function;
    struct node *owner;
  } binded;
  union {
    struct exp exp;
  };

  union {
    char cval;
    const char *sval;
    unsigned int inum;
    unsigned long lnum;
    unsigned long long llnum;
  };
};
enum {
  DATATYPE_FLAG_IS_SIGNED = 0b00000001,
  DATATYPE_FLAG_IS_STATIC = 0b00000010,
  DATATYPE_FLAG_IS_CONST = 0b00000100,
  DATATYPE_FLAG_IS_POINTER = 0b00001000,
  DATATYPE_FLAG_IS_ARRAY = 0b00010000,
  DATATYPE_FLAG_IS_EXTERN = 0b00100000,
  DATATYPE_FLAG_IS_RESTRICT = 0b01000000,
  DATATYPE_FLAG_IGNORE_TYPE_CHECKING = 0b10000000,
  DATATYPE_FLAG_IS_SECONDARY = 0b100000000,
  DATATYPE_FLAG_STRUCT_UNION_NO_NAME = 0b1000000000,
  DATATYPE_FLAG_IS_LITERAL = 0b10000000000

};
enum {
  DATA_TYPE_VOID,
  DATA_TYPE_CHAR,
  DATA_TYPE_SHORT,
  DATA_TYPE_INTEGER,
  DATA_TYPE_LONG,
  DATA_TYPE_FLOAT,
  DATA_TYPE_DOUBLE,
  DATA_TYPE_STRUCT,
  DATA_TYPE_UNION,
  DATA_TYPE_UNKNOWN
};
struct datatype {
  int flags;
  // i.e type of long,float,etc..
  int type;
  struct datatype *secondary;
  const char *type_str;
  size_t size;
  int pointer_depth;

  union {
    struct node *struct_node;
    struct node *union_node;
  };
};
enum {
  DATATYPE_EXPECT_PRIMITIVE,
  DATATYPE_EXPECT_UNION,
  DATATYPE_EXPECT_STRUCT
};
enum {
  DATA_SIZE_ZERO = 0,
  DATA_SIZE_BYTE = 1,
  DATA_SIZE_WORD = 2,
  DATA_SIZE_DWORD = 4,
  DATA_SIZE_DDWORD = 8,
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

int parse(struct compile_process *process);
bool token_is_symbol(struct token *token, char c);

bool token_is_nl_or_comment_or_newline_separator(struct token *token);

struct node *node_create(struct node *_node);

void node_set_vector(struct vector *vec, struct vector *root_vec);
void node_push(struct node *node);
struct node *node_peek_or_null();
struct node *node_peek();
struct node *node_pop();

bool node_is_expressionable(struct node *node);
struct node *node_peek_expressionable_or_null();
void make_exp_node(struct node *left_node, struct node *right_node,
                   const char *op);

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_OPERATORS_IN_GROUP 12

enum { ASSOCIATIVITY_LEFT_TO_RIGHT, ASSOCIATIVITY_RIGHT_TO_LEFT };

struct expressionable_op_precedence_group {
  char *operators[MAX_OPERATORS_IN_GROUP];
  int associativity;
};

bool keyword_is_datatype(const char *str);

bool token_is_primitive_keyword(struct token *token);

bool datatype_is_struct_or_union_for_name(const char *name);
bool token_is_operator(struct token *token, const char *val);
#endif
