#include "cat.h"

int main(int argc, char *argv[]) {
  opt options = {0};
  int flag = 0;
  parser(argc, argv, &options);
  while (optind < argc) {
    if ((flag = reader(argv, &options)) == 0) {
    } else if (flag == 1) {
      fprintf(stderr, "No such file or directory\n");
    } else if (flag == 2) {
      fprintf(stderr, "Is a directory\n");
    }
    optind++;
  }

  return 0;
}
