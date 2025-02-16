#ifndef CAT
#define CAT
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAC_EOL 138

typedef struct flags_cat {
  int s;
  int b;
  int n;
  int v;
  int t;
  int e;
  int T;
  int E;

} flags;
typedef struct cat_settings {
  int number_flag;
  int is_flag;
  int count_flag;
  int is_file;
  flags flag;
} setting;

void change_simvol_linux(const int ch, int view[4], setting *setting_flags);
void change_simvol_macos(const int ch, int view[4], setting *setting_flags);
bool pass_empty_str(setting *setting_flags, char simvol, int *count_empty_str,
                    int new_line);
void print_number_str(setting *setting_flags, int *number_str, int new_line,
                      int count_empty_str);
void print_simvol(setting *setting_flags, char simvol, int *new_line);
void print_file(FILE *file, char *name, setting *setting_flags);

void check_flag(char *commandStr, setting *setting_flags);
void init_cat(int argc, char *argv[]);
#endif