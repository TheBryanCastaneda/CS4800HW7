#include <stdio.h>
#include <time.h>

#include "../c/chunk.h"
#include "../c/vm.h"

int main(void) {
  const int negations = 2000000;
  const int repetitions = 50;

  initVM();

  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 1.0, 1);

  for (int i = 0; i < negations; i++) {
    writeChunk(&chunk, OP_NEGATE, 1);
  }

  writeChunk(&chunk, OP_RETURN, 1);

  clock_t start = clock();

  for (int i = 0; i < repetitions; i++) {
    if (interpret(&chunk) != INTERPRET_OK) {
      freeVM();
      freeChunk(&chunk);
      return 1;
    }
  }

  double seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf("Seconds: %.6f\n", seconds);

  freeVM();
  freeChunk(&chunk);
  return 0;
}