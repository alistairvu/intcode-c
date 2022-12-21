#ifndef _INTC_H
#define _INTC_H

#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 32
#define FILENAME_LEN 2048

void reset_buffer(char *buffer, int buf_length);

int string_length(const char *string);

#endif
