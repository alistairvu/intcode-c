#include "intc.h"

void reset_buffer(char *buffer, int buf_length) {
  for (int i = 0; i < buf_length; i++) {
    buffer[i] = 0;
  }
}

int string_length(const char *string) {
  int res = 0;

  while (*(string + res) != 0) {
    res++;
  }

  return res;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./intc <filename>\n");
    exit(1);
  }

  FILE *input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("Cannot open input file %s\n", argv[1]);
    exit(1);
  }

  char filename_buf[BUF_LEN];

  for (int i = 0; i <= string_length(argv[1]); i++) {
    filename_buf[i] = argv[1][i];
  }

  int filename_length = string_length(filename_buf);
  int i = filename_length - 1;

  for (; i >= 0; i--) {
    if (filename_buf[i] == '.') {
      break;
    }
  }

  filename_buf[i + 1] = 'o';
  filename_buf[i + 2] = 'u';
  filename_buf[i + 3] = 't';
  filename_buf[i + 4] = 0;
  printf("%s\n", filename_buf);

  FILE *output_file = fopen(filename_buf, "wb");

  if (output_file == NULL) {
    printf("Cannot open output file\n");
    exit(1);
  }

  char buffer[BUF_LEN];
  int n = 0;
  char c = 0;

  while ((c = fgetc(input_file)) != EOF) {
    if (c == ',') {
      long long int converted = strtoll(buffer, NULL, 0);
      fwrite(&converted, sizeof(long long int), 1, output_file);
      reset_buffer(buffer, BUF_LEN);
      n = 0;
      continue;
    }

    buffer[n++] = (char)c;
  }

  long long int converted = strtoll(buffer, NULL, 0);
  fwrite(&converted, sizeof(long long int), 1, output_file);

  fclose(input_file);
  fclose(output_file);
}
