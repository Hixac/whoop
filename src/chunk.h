#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
#include "line.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NEGATE,
    OP_NOT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_RETURN
} op_code;

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    lines_t lines;
    value_array constants;
} chunk_t;

void init_chunk(chunk_t *chunk);
void free_chunk(chunk_t *chunk);
void write_chunk(chunk_t *chunk, uint8_t byte, int line);
int add_constant(chunk_t *chunk, value_t value);
void write_constant(chunk_t *chunk, value_t value, int line);

#endif
