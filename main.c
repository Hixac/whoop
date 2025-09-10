#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void run_file(const char* path) {
    char* source = read_file(path);
    interpret_res_t result = interpret(source);
    free(source); 

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

void unit_vm_add() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 1.2);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   constant = add_constant(&chunk, 3.4);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   write_chunk(&chunk, OP_ADD, 1);
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == 4.6 && "ADD TEST FAILED");
   printf("[TEST]: INSTRUCTION ADD TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_vm_sub() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 1.2);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   constant = add_constant(&chunk, 3.4);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   write_chunk(&chunk, OP_SUBTRACT, 1);
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == -2.2 && "SUB TEST FAILED");
   printf("[TEST]: INSTRUCTION SUB TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_vm_mul() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 8);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   constant = add_constant(&chunk, 9);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   write_chunk(&chunk, OP_MULTIPLY, 1);
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == 72 && "MULTIPLY TEST FAILED");
   printf("[TEST]: INSTRUCTION MUL TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_vm_div() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 8);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   constant = add_constant(&chunk, 4);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);
   
   write_chunk(&chunk, OP_DIVIDE, 1);
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == 2 && "DIV TEST FAILED");
   printf("[TEST]: INSTRUCTION DIV TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_vm_neg() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 8);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);

   write_chunk(&chunk, OP_NEGATE, 1);
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == -8 && "NEGATE TEST FAILED");
   printf("[TEST]: INSTRUCTION NEG TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_vm_constant() {
   chunk_t chunk;
   init_chunk(&chunk);

   int constant = add_constant(&chunk, 8);
   write_chunk(&chunk, OP_CONSTANT, 1);
   write_chunk(&chunk, constant, 1);
   
   write_chunk(&chunk, OP_RETURN, 1);
   process_chunk_vm(&chunk);
   assert(pop() == 8 && "CONSTANT TEST FAILED");
   printf("[TEST]: INSTRUCTION CONSTANT TEST SUCCESS\n");

   free_chunk(&chunk);
}

void unit_test_vm_run() {
    unit_vm_add();
    unit_vm_sub();
    unit_vm_mul();
    unit_vm_div();
    unit_vm_neg();
    unit_vm_constant();
}

int main(int argc, const char **argv) {
    init_vm();
    unit_test_vm_run();
    
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    
    free_vm();
    return 0;
}
