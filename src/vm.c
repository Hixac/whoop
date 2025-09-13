#include <stdio.h>

#include "common.h"
#include "compile.h"
#include "debug.h"
#include "vm.h"

vm_t vm;

static void reset_stack() {
    vm.stack_top = vm.stack;
}

void init_vm() {
    reset_stack();
}

void free_vm() {}

static interpret_res_t run() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
    
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG() (vm.chunk->constants.values[READ_BYTE() + (READ_BYTE() << 8) + (READ_BYTE() << 16)])
#define BINARY_OP(op)                           \
    do {                                        \
        value_t b = pop();                      \
        value_t a = pop();                      \
        push(a op b);                           \
    } while (false)                             \
    
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("      ");
        for (value_t *slot = vm.stack; slot < vm.stack_top; ++slot) {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT_LONG: {
            value_t constant = READ_CONSTANT_LONG();
            push(constant);
            break;
        }
        case OP_CONSTANT: {
            value_t constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_NEGATE:   push(-pop()); break;
        case OP_ADD:      BINARY_OP(+); break;
        case OP_SUBTRACT: BINARY_OP(-); break;
        case OP_MULTIPLY: BINARY_OP(*); break;
        case OP_DIVIDE:   BINARY_OP(/); break;
        case OP_RETURN:
            return INTERPRET_OK;
        }
    }

#undef READ_CONSTANT_LONG
#undef READ_CONSTANT
#undef READ_BYTE
#undef BINARY_OP
    
#pragma GCC diagnostic pop
}

interpret_res_t interpret(const char *source) {
    chunk_t chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    interpret_res_t result = run();

    free_chunk(&chunk);
    return result;
}

void process_chunk_vm(chunk_t *chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    run();
}

void push(value_t value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

value_t pop() {
    vm.stack_top--;
    return *vm.stack_top;
}
