#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <stdio.h>

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->capacity = 0;
  chunk->count = 0;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }
  if (chunk->linesCapacity < chunk->linesCount + 1) {
    int oldLinesCapacity = chunk->linesCapacity;
    chunk->linesCapacity = GROW_CAPACITY(oldLinesCapacity);
    chunk->lines = GROW_ARRAY(LineEntry, chunk->lines, oldLinesCapacity,
                              chunk->linesCapacity);
  }
  if (chunk->lines[chunk->linesCount - 1].line == line) {
    chunk->lines[chunk->linesCount - 1].runLength++;
  } else {
    chunk->lines[chunk->linesCount].line = line;
    chunk->lines[chunk->linesCount].runLength = 1;
    chunk->linesCount++;
  }
  chunk->code[chunk->count] = byte;
  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineEntry, chunk->lines, chunk->linesCapacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}
