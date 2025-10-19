#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)        (AS_OBJ(value)->type)

#define IS_STRING(value)       is_obj_type(value, OBJ_STRING)

#define AS_STRING(value)       ((obj_string_t*)AS_OBJ(value))
#define AS_CSTRING(value)      (((obj_string_t*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
} obj_type_e;

struct obj { // obj_t
    obj_type_e type;
    obj_t *next;
};

struct obj_string {
    obj_t obj;
    int length;
    char *chars;
    uint32_t hash;
};

static inline bool is_obj_type(value_t value, obj_type_e type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

obj_string_t *take_string(char *chars, int length);
obj_string_t *copy_string(const char *chars, int length);
void print_object(value_t value);

#endif
