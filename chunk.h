#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_NEGATE,
  OP_CONSTANT_LONG,
  OP_RETURN,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
} OpCode;

typedef struct {
  int line;
  int runLength;
} LineEntry;

typedef struct {
  // instruction code
  int count;
  int capacity;
  uint8_t *code;

  // lines
  int linesCount;
  int linesCapacity;
  LineEntry *lines;

  // constants
  ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);
void freeChunk(Chunk *chunk);
void writeConstant(Chunk *chunk, Value value, int line);

#endif
