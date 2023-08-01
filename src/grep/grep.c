#include "grep.h"

int main(int argc, char **argv) {
  opt options = {0};
  int cflags = REG_EXTENDED;
  int HowMuchPatterns = 0;
  char pattern[4096] = {0};
  if (argc > 2) {
    parser(argc, argv, &options, &cflags, pattern, &HowMuchPatterns);
    int countFiles = howMuchFiles(argc, argv);
    while (argc > optind) {
      reader(argv, &options, cflags, pattern, countFiles);
      optind++;
    }
  } else {
    fprintf(
        stderr,
        "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
        "[-C[num]]"
        "[-e pattern] [-f file] [--binary-files=value] [--color=when]"
        "[--context[=num]] [--directories=action] [--label] [--line-buffered]"
        "[--null] [pattern] [file ...]");
  }
  return 0;
}
