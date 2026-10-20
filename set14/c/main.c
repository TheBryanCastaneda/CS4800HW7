#include <assert.h>
#include <stdio.h>
#include "chunk.h"
#include "common.h"

int main(void) {
    Chunk chunk;
    initChunk(&chunk);

    assert(getLine(&chunk, 0) == -1);

    for (int i = 0; i < 20; i++) {
        int constant = addConstant(&chunk, i + 0.5);
        int line = 10 + i / 2;

        writeChunk(&chunk, OP_CONSTANT, line);
        writeChunk(&chunk, constant, line);
    }

    writeChunk(&chunk, OP_RETURN, 10);

    assert(chunk.count == 41);
    assert(chunk.lineCount == 11);

    for (int offset = 0; offset < 40; offset++) {
        assert(getLine(&chunk, offset) == 10 + offset / 4);
    }

    assert(getLine(&chunk, 40) == 10);
    assert(getLine(&chunk, -1) == -1);
    assert(getLine(&chunk, 41) == -1);

    printf("Bytes: %d\n", chunk.count);
    printf("Line runs: %d\n", chunk.lineCount);

    freeChunk(&chunk);
    assert(chunk.count == 0);
    assert(chunk.lineCount == 0);

    return 0;
}