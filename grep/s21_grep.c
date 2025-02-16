#include "s21_grep.h"

int main(int argc, char *argv[]) {
  setting_grep settings = {{0}, {0}, NULL, NULL};
  find_flags(&settings, argc, argv);

  sampling_from_file(&settings, argc, argv);
  if (settings.list_file_regex != NULL) {
    destroy(settings.list_file_regex);
  }
  if (settings.list_str_find != NULL) {
    destroy(settings.list_str_find);
  }
  return 0;
}