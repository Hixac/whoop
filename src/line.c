#include <stdlib.h>
#include <stdio.h>

#include "line.h"
#include "memory.h"

static int has(lines_t *lines, int line) {
    for (int i = 0; i < lines->count; ++i) {
        if (lines->lines[i] == line)
            return i;
    }
    return -1;
}

void init_lines(lines_t *lines) {
    lines->count = 0;
    lines->capacity = 0;
    lines->lines = NULL;
    lines->repeats = NULL;
}

void free_lines(lines_t *lines) {
    FREE_ARRAY(int, lines->lines, lines->capacity);
    FREE_ARRAY(int, lines->repeats, lines->capacity);
    init_lines(lines);
}

void write_lines(lines_t *lines, int line) {
    if (lines->capacity < lines->count + 1) {
        int old_capacity = lines->capacity;
        lines->capacity = GROW_CAPACITY(old_capacity);
        lines->lines = GROW_ARRAY(int, lines->lines, old_capacity, lines->capacity);
        lines->repeats = GROW_ARRAY(int, lines->repeats, old_capacity, lines->capacity);
    }

    int where = has(lines, line);
    if (where != -1) {
        lines->repeats[where] += 1;
        return;
    }
    
    lines->lines[lines->count] = line;
    lines->repeats[lines->count] = 1;
    lines->count++;
}
 
int get_line(lines_t *lines, int index) {
    int repeats = 0;
    for (int i = 0; i < lines->count; ++i) {
        repeats += lines->repeats[i];
        if (index + 1 <= repeats)
            return lines->lines[i];
    }
    
    return -1;
}
