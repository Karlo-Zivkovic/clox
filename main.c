#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>

int main() {
  initVM();
  Chunk chunk;
  initChunk(&chunk);

  for (int i = 0; i < 300; i++) {
    Value value = i * 2.0;
    writeConstant(&chunk, value, 123);
  }

  printf("%d", chunk.lines[0].runLength);

  writeChunk(&chunk, OP_RETURN, 124);
  disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
