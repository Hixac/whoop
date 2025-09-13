#ifndef clox_line_h
#define clox_line_h

#include "common.h"

typedef struct {
    int count;
    int capacity;
    int *lines;
    int *repeats;
} lines_t;

void init_lines(lines_t *lines);
void free_lines(lines_t *lines);
void write_lines(lines_t *lines, int line);

int get_line(lines_t *lines, int index);

#endif
