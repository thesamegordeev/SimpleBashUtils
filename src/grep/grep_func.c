#include "grep.h"

void parser(int argc, char *argv[], opt *options, int *cflags, char *pattern,
            int *HowMuchPatterns) {
  static struct option long_option[] = {
      {"e", required_argument, NULL, 'e'},
      {"f", required_argument, NULL, 'f'},
      {0, 0, 0, 0},
  };
  int tmp;
  int option_index;
  while ((tmp = getopt_long(argc, argv, "ivclnhsf:oe:", long_option,
                            &option_index)) != -1) {
    switch (tmp) {
      case 'e':
        options->e = 1;
        flag_e(pattern, HowMuchPatterns);
        break;
      case 'f':
        options->f = 1;
        flag_f(pattern, HowMuchPatterns);
        break;
      case 'i':
        options->i = 1;
        *cflags |= REG_ICASE;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'o':
        options->o = 1;
        *cflags &= ~REG_NOSUB;
        break;
      default:
        fprintf(stderr,
                "usage: ./grep [-ivclnhsoe] template [-f] [file ...]\n");
    }
  }
  if (!options->f && !options->e) {
    flag_e_noflag(pattern, HowMuchPatterns, argv);
  }
}

void reader(char *argv[], opt *options, int cflags, char *pattern,
            int countFiles) {
  FILE *file;
  regex_t regex;
  int regres;
  int str_c = 0;
  int str_reg_c = 0;
  char *filename = argv[optind];
  regcomp(&regex, pattern, cflags);
  char str[4096] = {0};
  if ((file = fopen(filename, "r")) != NULL) {
    while (fgets(str, 4096, file) != NULL) {
      str_c++;
      regres = regexec(&regex, str, 0, NULL, 0);

      if (options->v) {
        regres = !regres;
      }

      if (regres == 0) {
        str_reg_c++;

        if (str[strlen(str) - 1] != '\n') {
          int end = strlen(str);
          str[end] = '\n';
          str[end + 1] = '\0';
        }

        if (!options->c && !options->l) {
          if (!options->h && countFiles > 1) {
            printf("%s:", filename);
          }
          if (options->n) {
            printf("%d:", str_c);
          }
          printf("%s", str);
        }
      }
    }

    if (options->c) {
      if (!options->h && countFiles > 1) {
        printf("%s:", filename);
      }
      if (options->c && options->l && str_reg_c > 0) {
        str_reg_c = 1;
      }
      printf("%d\n", str_reg_c);
    }
    if (options->l && str_reg_c > 0) {
      printf("%s\n", filename);
    }
  } else if (!options->s) {
    fprintf(stderr, "No such file or directiory: %s\n", filename);
  }
  regfree(&regex);
  fclose(file);
}

int howMuchFiles(int argc, char **argv) {
  int howMuch = 0;
  for (int counter = optind; counter < argc; counter++) {
    if (is_regular_file(argv[counter])) {
      howMuch++;
    }
  }
  return howMuch;
}

int is_regular_file(char *filename) {
  struct stat file_stat;
  stat(filename, &file_stat);
  return S_ISREG(file_stat.st_mode);
}

void flag_e(char *pattern, int *HowMuchPatterns) {
  if (*HowMuchPatterns != 0) {
    strcat(pattern, "|");
    *HowMuchPatterns = *HowMuchPatterns + 1;
  }
  strcat(pattern, optarg);
  *HowMuchPatterns = *HowMuchPatterns + strlen(optarg);
  if (*HowMuchPatterns != 0) {
    pattern[*HowMuchPatterns] = 0;
  }
}

void flag_e_noflag(char *pattern, int *HowMuchPatterns, char **argv) {
  if (*HowMuchPatterns != 0) {
    strcat(pattern, "|");
    *HowMuchPatterns = *HowMuchPatterns + 1;
  }
  strcat(pattern, argv[optind]);
  *HowMuchPatterns = *HowMuchPatterns + strlen(argv[optind]);
  optind++;
}

void flag_f(char *pattern, int *HowMuchPatterns) {
  FILE *file;
  char str[4096] = {0};
  if ((file = fopen(optarg, "r")) != NULL) {
    while (fgets(str, 4096, file) != NULL) {
      if (*HowMuchPatterns != 0) {
        pattern[*HowMuchPatterns] = '|';
      }
      strcat(pattern, str);
      *HowMuchPatterns = *HowMuchPatterns + strlen(str);
    }
  } else {
    fprintf(stderr, "No such file or directiory: %s\n", optarg);
  }
  fclose(file);
  if (*HowMuchPatterns != 0 && pattern[*HowMuchPatterns] == '\n') {
    pattern[*HowMuchPatterns] = 0;
  }
}