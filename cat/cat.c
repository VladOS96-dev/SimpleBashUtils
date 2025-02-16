#include "cat.h"

void change_simvol_linux(const int ch, int view[4], setting *setting_flags) {
  bool is_ctrl = iscntrl(ch) && ch != '\n' && ch != '\t';
  if (is_ctrl && setting_flags->flag.v == 1) {
    view[0] = '^';
    view[1] = ch + 64;
    if (ch == 127) view[1] = '?';
  } else if (ch > 127 && setting_flags->flag.v == 1) {
    view[0] = 'M';
    view[1] = '-';
    if (ch < 160) {
      view[2] = '^';
      view[3] = ch - 64;
    } else {
      view[2] = ch - 128;
      view[3] = 0;
    }
    if (ch == 255) {
      view[2] = '^';
      view[3] = '?';
    }
  } else {
    view[0] = ch;
  }
  if (ch == '\t' && setting_flags->flag.t == 1) {
    view[0] = '^';
    view[1] = 'I';
  }
  if (ch == '\n' && setting_flags->flag.e == 1) {
    view[0] = '$';
    view[1] = ch;
  }
}

void change_simvol_macos(const int ch, int view[4], setting *setting_flags) {
  bool is_ctrl = iscntrl(ch) && ch != '\n' && ch != '\t';
  if (is_ctrl && setting_flags->flag.v == 1) {
    view[0] = '^';
    view[1] = ch + 64;
    if (ch == 127) view[1] = '?';
  } else if (ch > 127 && setting_flags->flag.v == 1) {
    if (ch < 160) {
      view[0] = 'M';
      view[1] = '-';
      view[2] = '^';
      view[3] = ch - 64;
    } else {
      view[0] = ch;
    }
  } else {
    view[0] = ch;
  }
  if (ch == '\t' && setting_flags->flag.t == 1) {
    view[0] = '^';
    view[1] = 'I';
  }
  if (ch == '\n' && setting_flags->flag.e == 1) {
    view[0] = '$';
    view[1] = ch;
  }
}
bool pass_empty_str(setting *setting_flags, char simvol, int *count_empty_str,
                    int new_line) {
  if (new_line == 1) {
    if (simvol == '\n') {
      *count_empty_str += 1;

    } else {
      *count_empty_str = 0;
    }
  }
  return *count_empty_str > 1 && setting_flags->flag.s == 1;
}
void print_number_str(setting *setting_flags, int *number_str, int new_line,
                      int count_empty_str) {
  if (new_line == 1 && (setting_flags->flag.b == 0 || count_empty_str == 0)) {
    if (setting_flags->flag.b == 1 || setting_flags->flag.n == 1) {
      *number_str += 1;

      printf("%6d\t", *number_str);
    }
  }
}
void print_simvol(setting *setting_flags, char simvol, int *new_line) {
  int view[5] = {0};
#ifdef LINUX
  (simvol == 0 &&
   (!setting_flags->flag.e && !setting_flags->flag.v && !setting_flags->flag.t))
      ? printf("%c", simvol)
      : change_simvol_linux(simvol, view, setting_flags);
  for (int *p = view; *p; printf("%c", *p++)) {
  }
#else
  (simvol == 0 &&
   (!setting_flags->flag.e && !setting_flags->flag.v && !setting_flags->flag.t))
      ? printf("%c", simvol)
      : change_simvol_macos(simvol, view, setting_flags);

#endif
  for (int *p = view; *p; printf("%c", *p++)) {
  }
  *new_line = (simvol == '\n');
}
void print_file(FILE *file, char *name, setting *setting_flags) {
  if ((file = fopen(name, "r")) != NULL && setting_flags->is_file == 0) {
    setting_flags->is_file = 1;
    char simvol;
    int new_line = 1;
    int count_empty_str = 0;
    int number_str = 0;
    while ((simvol = fgetc(file)) != EOF) {
      if (pass_empty_str(setting_flags, simvol, &count_empty_str, new_line)) {
        continue;
      }
      print_number_str(setting_flags, &number_str, new_line, count_empty_str);

      print_simvol(setting_flags, simvol, &new_line);
    }
    fclose(file);
  } else {
    printf("%s: No such file or directory\n", name);
  }
}

void check_flag(char *commandStr, setting *setting_flags) {
  setting_flags->is_flag = 1;
  if (commandStr[0] == '-') {
    int length = strlen(commandStr);
    if (length == 2 && setting_flags->is_file != 1) {
      if (commandStr[1] == 'b') {
        setting_flags->flag.b = 1;

      } else if (commandStr[1] == 'e') {
        setting_flags->flag.e = 1;
        setting_flags->flag.v = 1;

      } else if (commandStr[1] == 'n') {
        setting_flags->flag.n = 1;

      } else if (commandStr[1] == 's') {
        setting_flags->flag.s = 1;

      } else if (commandStr[1] == 't') {
        setting_flags->flag.t = 1;
        setting_flags->flag.v = 1;

      } else if (commandStr[1] == 'v') {
        setting_flags->flag.v = 1;

      } else if (commandStr[1] == 'E') {
        setting_flags->flag.e = 1;
        setting_flags->flag.v = 1;

      } else if (commandStr[1] == 'T') {
        setting_flags->flag.t = 1;
        setting_flags->flag.v = 1;
      } else {
        setting_flags->is_flag = 0;
      }
    } else {
      setting_flags->is_flag = 0;
    }
  } else {
    setting_flags->is_flag = 0;
  }
}
void init_cat(int argc, char *argv[]) {
  FILE *file = NULL;
  setting setting_flags = {0, 1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

  if (argc >= 2) {
    for (int i = 1; i < argc; i++) {
      setting_flags.is_flag = 0;
      check_flag(argv[i], &setting_flags);
      if (setting_flags.is_flag == 0) {
        print_file(file, argv[i], &setting_flags);
        setting_flags.is_file = 0;
      }
    }

  } else {
    printf("N/A");
  }
}
