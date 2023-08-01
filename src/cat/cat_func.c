#include "cat.h"

void parser(int argc, char *argv[], opt *options) {
  static struct option long_option[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };

  int tmp;
  int option_index;
  while ((tmp = getopt_long(argc, argv, "+benstvTE", long_option,
                            &option_index)) != -1) {
    switch (tmp) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'v':
        options->v = 1;
        break;

      default:
        fprintf(stderr, "usage: ./cat [-benstuv] [file ...]\n");
    }
  }
}

int reader(char *argv[], opt *options) {
  int flag = 0;
  int num = 1;
  int sym1;
  int sym2 = '\n';
  int srt_counter = 0;
  FILE *file = fopen(argv[optind], "r");
  if (file && is_regular_file(argv[optind])) {
    while ((sym1 = getc(file)) != EOF) {
      if (options->s && sym1 == '\n' && sym2 == '\n') {
        srt_counter++;
        if (srt_counter > 1) {
          continue;
        }
      }
      if (options->b && sym1 != '\n' && sym2 == '\n') {
        printf("%6d\t", num++);
      }
      if (options->n && options->b == 0 && sym2 == '\n') {
        printf("%6d\t", num++);
      }
      if (options->e && sym1 == '\n') {
        printf("$");
      }
      if (options->t && sym1 == '\t') {
        printf("^");
        sym1 = 'I';
      }
      if (options->v && sym1 != '\n' && sym1 != '\t') {
        if (sym1 >= 0 && sym1 < 32) {
          sym1 = sym1 + 64;
          printf("^");
        } else if (sym1 == 127) {
          printf("^");
          sym1 = sym1 - 64;
        }
      }
      if (sym1 != '\n') {
        srt_counter = 0;
      }
      sym2 = sym1;
      printf("%c", sym2);
    }
  } else if (!file) {
    flag = 1;
  } else {
    flag = 2;
  }
  fclose(file);
  return flag;
}

int is_regular_file(char *filename) {
  struct stat file_stat;
  stat(filename, &file_stat);
  return S_ISREG(file_stat.st_mode);
}