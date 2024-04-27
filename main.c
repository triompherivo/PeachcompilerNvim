#include "compiler.h"
#include "helpers/vector.h"
#include <stdio.h>
int main() {
  // to comment a block of line : enter in v mode then :  type norm i//
  //   struct vector *vec = vector_create(sizeof(int));
  //   int x = 50;
  //   vector_push(vec, &x);
  //   x = 20;
  //   vector_push(vec, &x);
  //   vector_pop(vec);
  //   vector_set_peek_pointer(vec, 0);
  //   int *ptr = vector_peek(vec);
  //   while (ptr) {
  //     printf("%i\n", *ptr);
  //     ptr = vector_peek(vec);
  //   }
  //  printf("Hello world!\n");
  int res = compile_file("./test.c", "./test", 0);
  if (res == COMPILER_FILE_COMPILED_OK) {
    printf("everything compiled fine\n");
  } else if (res == COMPILER_FAILED_WITH_ERRORS) {
    printf("compile failed");
  } else {
    printf("unknown response for compile faile\n");
  }
  return 0;
}
