#include <stdio.h>


#include "common.h"
#include "vm.h"
#include "memory.h"

VM vm;

static void resetStack(void) {
  vm.stackTop = vm.stack;
}

void initVM(void) {
  vm.stack = NULL;
  vm.stackCapacity = 0;
  resetStack();
}

void freeVM(void) {
  FREE_ARRAY(Value, vm.stack, vm.stackCapacity);
  initVM();
}

void push(Value value) {
  int count = vm.stack == NULL
      ? 0
      : (int)(vm.stackTop - vm.stack);

  if (vm.stackCapacity < count + 1) {
    int oldCapacity = vm.stackCapacity;
    vm.stackCapacity = GROW_CAPACITY(oldCapacity);
    vm.stack = GROW_ARRAY(
        Value, vm.stack, oldCapacity, vm.stackCapacity);
    vm.stackTop = vm.stack + count;
  }

  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop(void) {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run(void) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

  for (;;) {
    uint8_t instruction = READ_BYTE();

    switch (instruction) {
      case OP_CONSTANT: {
        push(READ_CONSTANT());
        break;
      }

      case OP_CONSTANT_LONG: {
        uint32_t constant =
            ((uint32_t)vm.ip[0] << 16) |
            ((uint32_t)vm.ip[1] << 8) |
            (uint32_t)vm.ip[2];

        vm.ip += 3;
        push(vm.chunk->constants.values[constant]);
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

      case OP_NEGATE:
        push(-pop());
        break;

      case OP_RETURN:
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;

      default:
        return INTERPRET_RUNTIME_ERROR;
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  resetStack();
  vm.chunk = chunk;
  vm.ip = chunk->code;
  return run();
}