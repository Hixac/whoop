#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
    chunk_t *chunk;
    uint8_t *ip;
    value_t stack[STACK_MAX];
    value_t *stack_top;
} vm_t;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_res_t;

void init_vm();
void free_vm();
interpret_res_t interpret(const char *source);
void process_chunk_vm(chunk_t *chunk); 
void push(value_t value);
value_t pop();

#endif
