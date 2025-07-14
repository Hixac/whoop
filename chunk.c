#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void init_chunk(chunk_t *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    init_lines(&chunk->lines);
    init_value_array(&chunk->constants);
}

void free_chunk(chunk_t *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    free_lines(&chunk->lines);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

void write_chunk(chunk_t *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    write_lines(&chunk->lines, line);
    chunk->count++;
}

int add_constant(chunk_t *chunk, value_t value) {
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void write_constant(chunk_t *chunk, value_t value, int line) {
    if (chunk->constants.count <= UINT8_MAX) {
        int constant = add_constant(chunk, value);
        write_chunk(chunk, OP_CONSTANT, line);
        write_chunk(chunk, constant, line);
        return;
    }

    int constant = add_constant(chunk, value);
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    write_chunk(chunk, 0xff & constant, line);
    write_chunk(chunk, 0xff & (constant >> 8), line);
    write_chunk(chunk, 0xff & (constant >> 16), line);
}
