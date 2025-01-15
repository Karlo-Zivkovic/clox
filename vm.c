#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "memory.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>

VM vm;

static void resetStack() { vm.stackTop = vm.stack; }

void initVM() {
  vm.stackCapacity = STACK_INIT;
  vm.stack = malloc(sizeof(Value) * vm.stackCapacity);

  if (!vm.stack) {
    fprintf(stderr, "Failed to allocate initial stack memory.\n");
    exit(1);
  }
  resetStack();
}

void freeVM() {
  FREE_ARRAY(Value, vm.stack, (Value)(vm.stackTop - vm.stack));
  initVM();
}

void push(Value value) {
  if (vm.stackTop - vm.stack == vm.stackCapacity) {
    size_t oldSize = vm.stackCapacity;
    size_t newCapacity = vm.stackCapacity * 2;
    vm.stack = GROW_ARRAY(Value, vm.stack, oldSize, newCapacity);
    vm.stackCapacity = newCapacity;
    vm.stackTop = vm.stack + oldSize;
  }
  *vm.stackTop = value;
  vm.stackTop++;
}
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop();                                                          \
    double a = pop();                                                          \
    push(a op b);                                                              \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));

#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_NEGATE: {
      vm.stackTop[-1] = -vm.stackTop[-1];
      break;
    }
    case OP_RETURN: {
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
  }
}

InterpretResult interpret(const char *source) {
  Chunk chunk;
  initChunk(&chunk);

  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }
  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;
  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
}
