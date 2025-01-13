#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main() {
  initVM();
  Chunk chunk;
  initChunk(&chunk);

  // 4 - 3 * -2
  writeConstant(&chunk, 4, 123);
  writeConstant(&chunk, 3, 123);
  writeConstant(&chunk, 2, 123);
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_MULTIPLY, 123);
  writeChunk(&chunk, OP_SUBTRACT, 123);

  writeChunk(&chunk, OP_RETURN, 124);
  // disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
