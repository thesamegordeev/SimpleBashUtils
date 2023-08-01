#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} opt;

void parser(int argc, char *argv[], opt *options, int *cflags, char *pattern,
            int *HowMuchPatterns);
void reader(char *argv[], opt *options, int cflags, char *pattern,
            int countFiles);
int is_regular_file(char *filename);
int howMuchFiles(int argc, char **argv);
void flag_e(char *pattern, int *HowMuchPatterns);
void flag_e_noflag(char *pattern, int *HowMuchPatterns, char **argv);
void flag_f(char *pattern, int *HowMuchPatterns);