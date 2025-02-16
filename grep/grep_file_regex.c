#include "s21_grep.h"
int output_result_regex_in_file(setting_grep *settings, char *name_file,
                                char *buf_regex) {
  int exit = 0;
  regex_t regex;
  regmatch_t pmatch[1];
  size_t nmatch = 1;
  int len = strlen(buf_regex);
  if (buf_regex[len - 1] == '\n') {
    buf_regex[len - 1] = '\0';
  }
  char msgbuf[100];
  int reti = regcomp(&regex, buf_regex, settings->choose_flags.i);

  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
  }
  reti = regexec(&regex, settings->control_value.read_str, nmatch, pmatch, 0);
  if (!reti) {
    exit =
        output_string_base_regex(settings, name_file, &regex, nmatch, pmatch);

  } else if (reti == REG_NOMATCH && settings->choose_flags.v == 1 &&
             settings->control_value.match == 0) {
    settings->control_value.match = 2;

  } else if (reti != REG_NOMATCH) {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
  }
  regfree(&regex);
  return exit;
}
int flag_f(setting_grep *settings, char *name_file) {
  list *tmp = settings->list_file_regex;
  int exit = 0;
  while (settings->list_file_regex != NULL && exit == 0) {
    FILE *file_regex;
    if ((file_regex = fopen(settings->list_file_regex->value, "r")) != NULL) {
      settings->control_value.match = 0;
      char buf_regex[1024];
      while ((fgets(buf_regex, 1024, file_regex)) != NULL &&
             settings->control_value.same_file == 0 && exit == 0) {
        exit = output_result_regex_in_file(settings, name_file, buf_regex);
      }
      check_flags_after_check_all_word(settings, name_file);
      fclose(file_regex);
    } else if (settings->choose_flags.s == 0) {
      printf("%s: No such file or directory\n",
             settings->list_file_regex->value);
      exit = 1;
    } else {
      exit = 1;
    }
    settings->list_file_regex = settings->list_file_regex->next;
  }
  settings->list_file_regex = tmp;
  return exit;
}
void check_flags_after_check_all_word(setting_grep *settings, char *name_file) {
  if (settings->control_value.match == 2) {
    if (settings->choose_flags.c == 1) {
      settings->control_value.count_repeat_str++;
    } else {
      if (settings->choose_flags.l == 1) {
        printf("%s\n", name_file);

        settings->control_value.same_file = 1;

        settings->control_value.match = 1;

      } else {
        if (settings->control_value.lots_of_file >= 2 &&
            settings->choose_flags.h == 0) {
          printf("%s:", name_file);
        }
        if (settings->choose_flags.n == 1) {
          printf("%d:", settings->control_value.number_str);
        }

        printf("%s", settings->control_value.read_str);

        int len = strlen(settings->control_value.read_str);
        if (settings->control_value.read_str[len - 1] != '\n') {
          printf("\n");
        }
      }
    }
  }
  settings->control_value.number_str++;
}