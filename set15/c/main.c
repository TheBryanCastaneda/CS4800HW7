#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(void) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 1.2, 123);
  writeConstant(&chunk, 3.4, 123);
  writeChunk(&chunk, OP_ADD, 123);

  writeConstant(&chunk, 5.6, 123);
  writeChunk(&chunk, OP_DIVIDE, 123);
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  disassembleChunk(&chunk, "arithmetic test");
  InterpretResult result = interpret(&chunk);

    freeChunk(&chunk);

  if (result != INTERPRET_OK) {
    freeVM();
    return 1;
  }

  for (int i = 1; i <= 1000; i++) {
    writeConstant(&chunk, i, 200);
  }

  for (int i = 1; i < 1000; i++) {
    writeChunk(&chunk, OP_ADD, 200);
  }

  writeChunk(&chunk, OP_RETURN, 200);
  result = interpret(&chunk);

  freeVM();
  freeChunk(&chunk);

  return result == INTERPRET_OK ? 0 : 1;
}