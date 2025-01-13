#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define STACK_INIT 256

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  Value *stack;
  int stackCapacity;
  Value *stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
void push(Value value);
Value pop();
InterpretResult interpret(Chunk *chunk);

#endif
