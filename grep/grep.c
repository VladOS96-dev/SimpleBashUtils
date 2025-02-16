#include "s21_grep.h"
void command_o(regex_t *reg, regmatch_t pm[], const size_t nmatch, char *buf,
               int error) {
  char *temp = buf;
  while (error == 0) {
    for (int i = pm->rm_so; i < pm->rm_eo; i++) {
      fputc(temp[i], stdout);
    }
    fputc('\n', stdout);
    temp += pm->rm_eo;
    error = regexec(reg, temp, nmatch, pm, 0);
  }
}
void check_transfer_new_line(setting_grep *settings) {
  int len = strlen(settings->control_value.read_str);
  if (settings->control_value.read_str[len - 1] == '\n') {
    settings->control_value.transfer_new_line = 0;
  } else {
    settings->control_value.transfer_new_line = 1;
  }
}
int output_string_base_regex(setting_grep *settings, char *name_file,
                             regex_t *regex, size_t nmatch,
                             regmatch_t *pmatch) {
  int exit = 0;
  if (settings->choose_flags.v == 1) {
    settings->control_value.match = 3;
  }
  if (settings->choose_flags.v == 0) {
    if (settings->choose_flags.c == 1 && settings->control_value.match == 0) {
      settings->control_value.count_repeat_str++;
      settings->control_value.match = 1;
    } else if (settings->choose_flags.c == 0) {
      if (settings->choose_flags.l == 1) {
        printf("%s\n", name_file);
        settings->control_value.same_file = 1;

        settings->control_value.match = 1;
        exit = 1;
      } else {
        if (settings->control_value.lots_of_file >= 2 &&
            settings->choose_flags.h == 0 &&
            settings->control_value.match == 0) {
          printf("%s:", name_file);
        }
        if (settings->choose_flags.n == 1 &&
            settings->control_value.match == 0) {
          printf("%d:", settings->control_value.number_str);
        }
        if (settings->choose_flags.o == 1) {
          int error = 0;
          command_o(regex, pmatch, nmatch, settings->control_value.read_str,
                    error);
        } else {
          if (settings->control_value.match == 0)
            printf("%s", settings->control_value.read_str);
        }
        settings->control_value.match = 1;
      }
    }

    check_transfer_new_line(settings);
  }
  return exit;
}
int output_result(setting_grep *settings, char *name_file, char *str_find,
                  regex_t *regex, int *is_find) {
  char msgbuf[100];
  size_t nmatch = 1;
  int exit = 0;
  regmatch_t pmatch[1];
  int reti = regcomp(regex, str_find, settings->choose_flags.i);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
  }
  reti = regexec(regex, settings->control_value.read_str, nmatch, pmatch, 0);
  if (!reti) {
    exit = output_string_base_regex(settings, name_file, regex, nmatch, pmatch);
  } else if (reti == REG_NOMATCH && settings->choose_flags.v == 1 &&
             *is_find == 0) {
    if (settings->choose_flags.c == 1) {
      settings->control_value.count_repeat_str++;
    } else if (settings->choose_flags.l == 1) {
      printf("%s\n", name_file);
      exit = 1;
    } else {
      if (settings->control_value.lots_of_file >= 2 &&
          settings->choose_flags.h == 0) {
        printf("%s:", name_file);
      }
      if (settings->choose_flags.n == 1) {
        printf("%d:", settings->control_value.number_str);
      }
      *is_find = 1;
      printf("%s", settings->control_value.read_str);
      int len = strlen(settings->control_value.read_str);
      if (settings->control_value.read_str[len - 1] != '\n') {
        printf("\n");
      }
    }
  } else if (reti != REG_NOMATCH) {
    regerror(reti, regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
  }
  settings->control_value.number_str++;
  regfree(regex);
  return exit;
}

int executing_comand(setting_grep *settings, char *name_file) {
  int exit = 0;

  list *tmp = settings->list_str_find;
  int is_find = 0;
  regex_t regex;
  while (settings->list_str_find != NULL && exit == 0) {
    settings->control_value.match = 0;
    exit = output_result(settings, name_file, settings->list_str_find->value,
                         &regex, &is_find);
    settings->list_str_find = settings->list_str_find->next;
  }

  settings->list_str_find = tmp;
  return exit;
}
void check_flag_c(setting_grep *settings, char *name_file) {
  if (settings->choose_flags.c == 1) {
    if (settings->control_value.lots_of_file >= 2 &&
        settings->choose_flags.h == 0) {
      printf("%s:", name_file);
    }
    if (settings->choose_flags.l == 1 &&
        settings->control_value.count_repeat_str > 0) {
      settings->control_value.count_repeat_str = 1;
      printf("%d\n", settings->control_value.count_repeat_str);

      printf("%s\n", name_file);

    } else {
      printf("%d\n", settings->control_value.count_repeat_str);
    }
    settings->control_value.count_repeat_str = 0;
  }
}

void open_read_file(setting_grep *settings, char *name_file) {
  FILE *file;
  if ((file = fopen(name_file, "r")) != NULL) {
    char read_str[2048];
    settings->control_value.same_file = 0;
    int exit = 0;
    settings->control_value.transfer_new_line = 0;
    while ((fgets(read_str, 2048, file)) != NULL && exit == 0) {
      settings->control_value.read_str = read_str;
      if (settings->choose_flags.f == 1) {
        exit = flag_f(settings, name_file);
        if (settings->choose_flags.e == 1) {
          exit = executing_comand(settings, name_file);
        }
      } else {
        exit = executing_comand(settings, name_file);
      }
    }
    fclose(file);
    if (settings->control_value.transfer_new_line == 1) {
      printf("\n");
    }
  } else {
    if (settings->choose_flags.s == 0) {
      printf("%s:No such file or directory\n", name_file);
    }
  }
  check_flag_c(settings, name_file);
  settings->control_value.number_str = 1;
}
void add_value_in_list(list **node, char *value) {
  if (*node == NULL) {
    *node = init(value);
  } else {
    add(*node, value);
  }
}
int check_flags(setting_grep *settings, char flag, char *optarg) {
  int error = 0;
  switch (flag) {
    case 'e':
      settings->choose_flags.e = 1;
      add_value_in_list(&settings->list_str_find, optarg);
      break;
    case 'i':
      settings->choose_flags.i = REG_ICASE;
      break;
    case 'v':
      settings->choose_flags.v = 1;
      break;
    case 'c':
      settings->choose_flags.c = 1;
      break;
    case 'l':
      settings->choose_flags.l = 1;
      break;
    case 'n':
      settings->control_value.number_str = 1;
      settings->choose_flags.n = 1;
      break;
    case 'h':
      settings->choose_flags.h = 1;
      break;
    case 's':
      settings->choose_flags.s = 1;
      break;
    case 'f':
      settings->choose_flags.f = 1;
      add_value_in_list(&settings->list_file_regex, optarg);
      break;
    case 'o':
      settings->choose_flags.o = 1;
      break;
    default:
      error = 1;
  }
  if (settings->list_str_find == NULL && optarg != NULL &&
      settings->choose_flags.f == 0) {
    add_value_in_list(&settings->list_str_find, optarg);
  }
  return error;
}
void sampling_from_file(setting_grep *settings, int argc, char *argv[]) {
  if (optind < argc) {
    if (settings->choose_flags.f != 1 && settings->list_str_find == NULL) {
      add_value_in_list(&settings->list_str_find, argv[optind++]);
    }
    if (argc - optind >= 2) {
      settings->control_value.lots_of_file = argc - optind;
    }
    while (optind < argc) {
      open_read_file(settings, argv[optind++]);
    }
  }
}
void find_flags(setting_grep *settings, int argc, char *argv[]) {
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {{0, 0, 0, 0}};

    int flag =
        getopt_long(argc, argv, "e:icvlnhsf:o", long_options, &option_index);
    if (flag == -1) break;
    if (check_flags(settings, flag, optarg) == 1) {
      printf("no comand\n");
      printf("%s", argv[optind]);
    }
  }
}
