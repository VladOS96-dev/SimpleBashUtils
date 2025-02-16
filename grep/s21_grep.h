#ifndef S21_GREP
#define S21_GREP
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
typedef struct flags {
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
} flags;
typedef struct control_value_grep {
  char *read_str;
  int count_repeat_str;
  int number_str;
  int lots_of_file;
  int match;
  int same_file;
  int transfer_new_line;
} control_value_grep;
typedef struct setting_grep {
  flags choose_flags;
  control_value_grep control_value;
  list *list_str_find;
  list *list_file_regex;
} setting_grep;
void command_o(regex_t *reg, regmatch_t pm[], const size_t nmatch, char *buf,
               int error);
int executing_comand(setting_grep *settings, char *name_file);
void check_flags_after_check_all_word(setting_grep *settings, char *name_file);
int output_result(setting_grep *settings, char *name_file, char *str_find,
                  regex_t *regex, int *is_find);
void check_flag_c(setting_grep *settings, char *name_file);
void open_read_file(setting_grep *settings, char *name_file);
void add_value_in_list(list **node, char *value);
int output_result_regex_in_file(setting_grep *settings, char *name_file,
                                char *buf_regex);
int output_string_base_regex(setting_grep *settings, char *name_file,
                             regex_t *regex, size_t nmatch, regmatch_t *pmatch);
void check_transfer_new_line(setting_grep *settings);
int flag_f(setting_grep *settings, char *name_file);
int check_flags(setting_grep *settings, char flag, char *optarg);
void sampling_from_file(setting_grep *settings, int argc, char *argv[]);
void find_flags(setting_grep *settings, int argc, char *argv[]);
#endif