#include <stdio.h>
#include <stdlib.h>

void show_usage_error(char *__name__) {
  fprintf(stderr, "Error: You must provide the CSV file as an argument.\n");
  fprintf(stderr, "Usage: %s <filename.csv>\n", __name__);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc == 1) show_usage_error(argv[0]);

  return 0;
}
