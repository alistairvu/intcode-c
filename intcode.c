#include "intcode.h"

/**
 * Creates a new VM
 *
 * @return A malloc'ed VM
 */
VM *vm_new(int ascii) {
  VM *vm = malloc(sizeof(VM));
  assert(vm);

  vm->pc = 0;
  vm->rel_base = 0;
  vm->ascii = ascii;

  return vm;
}

/**
 * Frees a malloc'ed VM
 *
 * @param self The VM to free
 */
void vm_free(VM *self) { free(self); }

/**
 * Load data from binary file
 *
 * @param self The VM to load data into
 * @param filename The binary file to load
 */
void vm_load(VM *self, const char *filename) {
  FILE *input_file = fopen(filename, "rb");
  if (input_file == NULL) {
    printf("Cannot open input file %s\n", filename);
    exit(1);
  }

  long long int val = 0;
  int step = 0;
  int c;
  int n = 0;

  while ((c = fgetc(input_file)) != EOF) {
    long long int current = c;

    for (int i = 0; i < step; i++) {
      current = current << 8;
    }

    val = val + current;
    step = (step + 1) % 8;

    if (step == 0) {
      self->memory[n++] = val;
      val = 0;
    }
  }

  self->mem_size = n;
}

/** Run the VM
 *
 * @param self The VM to run.
 */
void vm_run(VM *self) {
  while (self->pc < self->mem_size) {
    switch (self->memory[self->pc] % 100) {
      case VM_ADD: {
        vm_handle_add(self);
        break;
      }

      case VM_MULT: {
        vm_handle_mult(self);
        break;
      }

      case VM_INP: {
        vm_handle_input(self);
        break;
      }

      case VM_OUT: {
        vm_handle_output(self);
        break;
      }

      case VM_JMP_TRUE: {
        vm_handle_jump_true(self);
        break;
      }

      case VM_JMP_FALSE: {
        vm_handle_jump_false(self);
        break;
      }

      case VM_LESS_THAN: {
        vm_handle_less_than(self);
        break;
      }

      case VM_EQUALS: {
        vm_handle_equals(self);
        break;
      }

      case VM_REL_BASE: {
        vm_handle_rel_base(self);
        break;
      }

      case VM_RET: {
        return;
      }

      default: {
        self->pc++;
      }
    }
  }
}

/** Handles the ADD command
 *
 * @param self The VM to handle the ADD command.
 */
void vm_handle_add(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  int destination_mode = (self->memory[self->pc] / 10000) % 10;
  long long int destination = self->memory[self->pc + 3];

  if (destination_mode == 0) {
    self->memory[destination] = first_param + second_param;
  } else {
    self->memory[destination + self->rel_base] = first_param + second_param;
  }

  self->pc += 4;
}

/** Handles the MULT command
 *
 * @param self The VM to handle the MULT command.
 */
void vm_handle_mult(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  int destination_mode = (self->memory[self->pc] / 10000) % 10;
  long long int destination = self->memory[self->pc + 3];
  if (destination_mode == 0) {
    self->memory[destination] = first_param * second_param;
  } else {
    self->memory[destination + self->rel_base] = first_param * second_param;
  }

  self->pc += 4;
}

/** Handles input
 *
 * @param self VM
 */
void vm_handle_input(VM *self) {
  long long int val;

  if (self->ascii) {
    val = getchar();
  } else {
    scanf("%lld", &val);
  }

  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int destination = self->memory[self->pc + 1];

  if (first_mode == 2) {
    self->memory[destination + self->rel_base] = val;
  } else {
    self->memory[destination] = val;
  }
  self->pc += 2;
}

/** Handles output
 *
 * @param self VM
 */
void vm_handle_output(VM *self) {
  int mode = (self->memory[self->pc] / 100) % 10;
  long long int val = vm_get_param(self, self->memory[self->pc + 1], mode);

  if (self->ascii) {
    putchar(val);
  } else {
    printf("%lld\n", val);
  }

  self->pc += 2;
}

/** Handles jump-if-true
 *
 * @param self VM
 */
void vm_handle_jump_true(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  if (first_param) {
    self->pc = second_param;
    return;
  }

  self->pc += 3;
}

/**
 * Handles jump-if-false
 *
 * @param self VM
 */
void vm_handle_jump_false(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  if (first_param == 0) {
    self->pc = second_param;
    return;
  }

  self->pc += 3;
}

/**
 * Handles less-than
 *
 * @param self VM
 */
void vm_handle_less_than(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  int destination_mode = (self->memory[self->pc] / 10000) % 10;
  long long int destination = self->memory[self->pc + 3];
  int val = first_param < second_param ? 1 : 0;
  if (destination_mode == 0) {
    self->memory[destination] = val;
  } else {
    self->memory[destination + self->rel_base] = val;
  }

  self->pc += 4;
}

/**
 * Handles equals
 *
 * @param self VM
 */
void vm_handle_equals(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);

  int second_mode = (self->memory[self->pc] / 1000) % 10;
  long long int second_param =
      vm_get_param(self, self->memory[self->pc + 2], second_mode);

  int destination_mode = (self->memory[self->pc] / 10000) % 10;
  long long int destination = self->memory[self->pc + 3];
  int val = first_param == second_param ? 1 : 0;
  if (destination_mode == 0) {
    self->memory[destination] = val;
  } else {
    self->memory[destination + self->rel_base] = val;
  }

  self->pc += 4;
}

/**
 * Change the relative base of VM
 *
 * @param self VM
 */
void vm_handle_rel_base(VM *self) {
  int first_mode = (self->memory[self->pc] / 100) % 10;
  long long int first_param =
      vm_get_param(self, self->memory[self->pc + 1], first_mode);
  self->rel_base += first_param;
  self->pc += 2;
}

/**
 * Prints out the memory of the VM
 *
 * @param self The VM to print out the memory of.
 */
void vm_print_memory(VM *self) {
  for (int i = 0; i < self->mem_size; i++) {
    printf("%lld", self->memory[i]);

    if (i + 1 == self->mem_size) {
      putchar('\n');
    } else {
      putchar(',');
    }
  }
}

/**
 * Gets the correct parameter value
 *
 * @param self The current VM
 * @param val Value of parameter
 * @param mode Mode of parameter
 */
long long int vm_get_param(VM *self, int val, int mode) {
  switch (mode) {
    case POS_MODE: {
      return self->memory[val];
    }

    case IMM_MODE: {
      return val;
    }

    case REL_MODE: {
      return self->memory[self->rel_base + val];
    }
  }

  perror("Unknown mode found");
  vm_free(self);
  exit(1);
  return -1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./intc <filename>\n");
    exit(1);
  }

  int ascii = 0;

  if (argc >= 3) {
    if (argv[2][0] == '-' && argv[2][1] == 'a') {
      ascii = 1;
    }
  }

  VM vm = {{0LL}, 0, 0LL, 0, ascii};
  vm_load(&vm, argv[1]);
  vm_run(&vm);
}
