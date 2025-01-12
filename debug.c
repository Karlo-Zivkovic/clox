#include "debug.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constantIndex = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constantIndex);
  printValue(chunk->constants.values[constantIndex]);
  printf("'\n");
  return offset + 2;
}

static int constantLongInstruction(const char *name, Chunk *chunk, int offset) {
  uint32_t index = (chunk->code[offset + 1] << 16) |
                   (chunk->code[offset + 2] << 8) | chunk->code[offset + 3];
  printf("%-16s %4d '", name, index);
  printValue(chunk->constants.values[index]);
  printf("'\n");

  return offset + 4; // Advance by 1 opcode byte + 3 operand bytes.
}

static int getLine(Chunk *chunk, int instructionIndex) {
  int instructionCount = 0;

  for (int i = 0; i < chunk->linesCount; i++) {
    instructionCount += chunk->lines[i].runLength;
    if (instructionIndex < instructionCount) {
      return chunk->lines[i].line;
    }
  }
  return -1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  if (offset > 0 &&
      // getLine(chunk, offset) == chunk->lines[chunk->linesCount - 1].line) {
      getLine(chunk, offset) == getLine(chunk, offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", getLine(chunk, offset));
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_CONSTANT_LONG:
    return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 2;
  }
}

const char *getOpcodeName(uint8_t opcode) {
  switch (opcode) {
  case OP_CONSTANT:
    return "OP_CONSTANT";
  case OP_RETURN:
    return "OP_RETURN";
  default:
    return "UNKNOWN_OPCODE";
  }
}
