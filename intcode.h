#ifndef _INTCODE_H
#define _INTCODE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 30000

#define VM_ADD 1
#define VM_MULT 2
#define VM_INP 3
#define VM_OUT 4
#define VM_JMP_TRUE 5
#define VM_JMP_FALSE 6
#define VM_LESS_THAN 7
#define VM_EQUALS 8
#define VM_REL_BASE 9

#define VM_RET 99

#define POS_MODE 0
#define IMM_MODE 1
#define REL_MODE 2

typedef struct vm VM;

struct vm {
  long long int memory[MEM_SIZE];
  int pc;
  long long int rel_base;
  int mem_size;
  int ascii;
};

VM *vm_new(int ascii);
void vm_free(VM *self);

void vm_load(VM *self, const char *filename);
void vm_run(VM *self);

void vm_handle_add(VM *self);
void vm_handle_mult(VM *self);
void vm_handle_input(VM *self);
void vm_handle_output(VM *self);

void vm_handle_jump_true(VM *self);
void vm_handle_jump_false(VM *self);
void vm_handle_less_than(VM *self);
void vm_handle_equals(VM *self);
void vm_handle_rel_base(VM *self);

void vm_print_memory(VM *self);

long long int vm_get_param(VM *self, int addr, int mode);

#endif
