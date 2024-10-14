#define _GNU_SOURCE
#include "s21_grep.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  parameters params = {
      .fileForF = {0},
      .patterns = {0},
      .opt = {0},
      .numOfpatterns = 0,
      .PatternFiles = 0,
      .emptystr = 0,
      .err0 = 0,
      .inv_opt = ' '
  };

  getOption(argc, argv, &params);
  if (params.opt.e_flag == 0 && params.opt.f_flag == 0) {
    if (patternWithoutE(argc, argv, &params)) params.numOfpatterns = 1;
  }
  if (correctInput(argc, &params)) {
    reader(argc, argv, &params);
  }
  cleanMemory(&params);
  return 0;
}

int correctInput(int argc, parameters *params) {//+
    int res = 1;
    if (argc == 1 || params->err0) {
        res = 0;
        if (!params->err0) {
            printf(
                "usage: grep [-ivclnhs] [-e pattern] [-f file] [--null] [pattern] "
                "[file ...]");
        } else {
            if (params->err0 == 1) {
                printf(
                    "grep: invalid option -- %c\nusage: grep [-ivclnhs]\n"
                    "[-e pattern] [-f file] [--null] [pattern] [file ...]",
                    params->inv_opt);
            } else if (params->err0 == 2) {
                printf("grep: option requires an argument");
            } else if (params->err0 == 3) {
                printf("Insufficient memory available");
            }
        }
    }
    return res;
}

void getOption(int argc, char **argv, parameters *params) {//+
  int i = 1;
  while (i < argc) {
      if (argv[i][0] == '-') {
          parser(params, i, argv, argc);
          memset(argv[i], '\0', strlen(argv[i]));
      }
      i++;
  }
}

int parser(parameters *params, int i, char **argv, int argc) {//+
    int k = 1;
    while (argv[i][k] != '\0') {
        if (argv[i][k] == 'e') {
            params->opt.e_flag = 1;
            if (i == (argc - 1) && argv[i][k + 1] == '\0') {
                params->err0 = 2;
            } else {
                patternEorFileF(argv, i, k, params);
            }
        } else if (argv[i][k] == 'i')
            params->opt.i_flag = 1;
        else if (argv[i][k] == 'v')
            params->opt.v_flag = 1;
        else if (argv[i][k] == 'c')
            params->opt.c_flag = 1;
        else if (argv[i][k] == 'l')
            params->opt.l_flag = 1;
        else if (argv[i][k] == 'n')
            params->opt.n_flag = 1;
        else if (argv[i][k] == 'h')
            params->opt.h_flag = 1;
        else if (argv[i][k] == 's')
            params->opt.s_flag = 1;
        else if (argv[i][k] == 'o')
            params->opt.o_flag = 1;
        else if (argv[i][k] == 'f') {
            params->opt.f_flag = 1;
            if (i == (argc - 1) && argv[i][k + 1] == '\0') {
                params->err0 = 2;
            } else {
                printf("i'm here, mother's facker");
                patternEorFileF(argv, i, k, params);
                patternsFromFile(params);
            }
        } else {
            params->err0 = 1;
            params->inv_opt = argv[i][k];
        }
        k++;
    }
    return params->err0;
}


int patternWithoutE(int argc, char **argv, parameters *params) {//+
    int i = 1, flag = 1, res = 0;
    while (i < (argc - 1) && flag) {
        if (argv[i][0] != '\0') {
            params->patterns[0] = malloc((strlen(argv[i]) + 1) * sizeof(char));
            if (params->patterns[0] != NULL)
                strcpy(params->patterns[0], argv[i]);
            else
                params->err0 = 3;
            memset(argv[i], '\0', strlen(argv[i]));
            flag = 0;
            res = 1;
        }
        i++;
    }
    return res;
}

void patternEorFileF(char **argv, int i, int k, parameters *params) {//+
    if (argv[i][k + 1] != '\0') {  // копируем часть строки i после флага -е в паттерн
        params->patterns[params->numOfpatterns] = malloc(((strlen(argv[i]) - k) + 1) * sizeof(char));
        if (params->patterns[params->numOfpatterns] != NULL) {
            copyEOstr(params->patterns[params->numOfpatterns], argv[i], k + 1);
        } else {
            params->err0 = 3;
        }
        memset(argv[i], '\0', strlen(argv[i]));
    } else {
        params->patterns[params->numOfpatterns] = malloc((strlen(argv[i + 1]) + 1) * sizeof(char));
        if (params->patterns[params->numOfpatterns] != NULL) {
            strcpy(params->patterns[params->numOfpatterns], argv[i + 1]);
        } else {
            params->err0 = 3;
        }
        memset(argv[i + 1], '\0', strlen(argv[i + 1]));
    }
    params->numOfpatterns += 1;
}

void copyEOstr(char *dest, const char *src,
               int k) {  // копирует src в dest начиная с k-го элемента
  int m = 0;
  while (src[k] != '\0') {
    dest[m] = src[k];
    m++;
    k++;
  }
  dest[m] = '\0';
}

int numberOfFiles(int argc, char **argv) {
  int numfiles = 0, i = 1;
  while (i < argc) {
    if (argv[i][0] != '\0') numfiles++;
    i++;
  }
  return numfiles;
}

void patternsFromFile(parameters *params) {//+ check: might have a mistake
    FILE *f = NULL;
    char *buffer = NULL;
    size_t len = 0;
    for (int i = 0; i < params->PatternFiles; i++) {
        printf("%s\n", params->fileForF[i]);
        f = fopen(params->fileForF[i], "r");
        if (f != NULL) {
            ssize_t read = 0;
            while ((read = getline(&buffer, &len, f)) != -1) {
                if (buffer[0] == '\n') params->emptystr = 1;
                params->patterns[params->numOfpatterns] = malloc((strlen(buffer) + 1) * sizeof(char));
                if (params->patterns[params->numOfpatterns] != NULL)
                    strcpy(params->patterns[params->numOfpatterns], buffer);
                else
                    params->err0 = 3;
                if (params->patterns[params->numOfpatterns][strlen(buffer) - 1] == '\n') {
                    params->patterns[params->numOfpatterns][strlen(buffer) - 1] = '\0';
                }
                params->numOfpatterns += 1;
            }
            if (buffer) free(buffer);
            fclose(f);
        } else {
            fprintf(stderr, "grep: %s: No such file or directory\n", params->fileForF[i]);
        }
    }
}

void grepFunc(char *line, char *patterns, parameters *params, int *isMatch) {
    int r = 0;
    regex_t reg;
    if (strcmp(patterns, ".") == 0) {
        r = regcomp(&reg, patterns, REG_NEWLINE);
    } else {
        r = regcomp(&reg, patterns, params->opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
    }
    if (r == 0) {
        int res = regexec(&reg, line, 0, NULL, 0);
        if (res == 0) {
            *isMatch = 1;
        }
        regfree(&reg);
    }
}

void reader(int argc, char **argv, parameters *params) {//+
    FILE *f = NULL;
    size_t len = 0;
    int cntFilesForSearch = numberOfFiles(argc, argv);
    int currentFile = 1;
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
            int counter = 0;  // для флага -с
            if (f != NULL) {
                char *buffer = NULL;
                int num = 1;  // для вывода номера строки
                ssize_t read;
                while ((read = getline(&buffer, &len, f)) != -1) {
                    int match = 0;
                    for (int i = 0; i < params->numOfpatterns && match == 0; i++) {
                        grepFunc(buffer, params->patterns[i], params, &match);
                    }
                    if (params->emptystr) match = 1;
                    if (params->opt.v_flag) v_flag(&match);
                    if (params->opt.c_flag || params->opt.l_flag) cl_flag(match, &counter, params->opt);
                    if (!params->opt.c_flag && !params->opt.l_flag)
                        output(cntFilesForSearch, params, argv[currentFile], num, buffer, match);
                    num++;
                }
                if (params->opt.c_flag || params->opt.l_flag)
                    cl_output(params->opt, cntFilesForSearch, argv[currentFile], counter);
                if (buffer) free(buffer);
                fclose(f);
            } else {
                if (!params->opt.s_flag)
                    fprintf(stderr, "grep: %s: No such file or directory\n", argv[currentFile]);
            }
        }
        currentFile++;
    }
}

void v_flag(int *match) {
  if (*match == 1)
    *match = 0;
  else
    *match = 1;
}

void cl_flag(int match, int *counter, grepOptions opt) {
  if (match) {
    *counter += 1;
    if (opt.l_flag) *counter = 1;
  }
}

void cl_output(grepOptions opt, int cntFilesForSearch, const char *nameOfFile,
               int counter) {
  if (opt.c_flag) {
    if (cntFilesForSearch > 1 && !opt.h_flag) printf("%s:", nameOfFile);
    printf("%d\n", counter);
  }
  if (counter && opt.l_flag) {
    printf("%s\n", nameOfFile);
  }
}

void output(int cntFilesForSearch, parameters *params, const char *nameOfFile, int num, const char *buffer, int match) {//+
    if (match) {
        if (cntFilesForSearch > 1 && !params->opt.h_flag) printf("%s:", nameOfFile);
        if (params->opt.n_flag) printf("%d:", num);
        if (params->opt.o_flag) {
            // Вывод только совпавших частей строки
            printMatchedParts(buffer, params);
        } else {
            print(buffer);
        }
    }
}

void print(const char *buffer) {
  for (int i = 0; buffer[i] != '\0'; i++) {
    printf("%c", buffer[i]);
    if (buffer[i + 1] == '\0' && buffer[i] != '\n') printf("\n");
  }
}

void cleanMemory(parameters *params) {//+
    for (int i = 0; i < params->numOfpatterns; i++) {
        // Проверка на NULL перед free
        if (params->patterns[i] != NULL) {
            free(params->patterns[i]);
            params->patterns[i] = NULL;  // Нулевое значение указателя после освобождения
        }
    }
    for (int i = 0; i < params->PatternFiles; i++) {
        // Проверка на NULL перед free
        if (params->fileForF[i] != NULL) {
            free(params->fileForF[i]);
            params->fileForF[i] = NULL;  // Нулевое значение указателя после освобождения
        }
    }
}

void printMatchedParts(const char *buffer, parameters *params) {
    regex_t reg;

    for (int i = 0; i < params->numOfpatterns; i++) {
        int r = regcomp(&reg, params->patterns[i], REG_EXTENDED);
        if (r != 0) {
            // Обработка ошибок при компиляции регулярного выражения
            fprintf(stderr, "Error compiling regex: %s\n", params->patterns[i]);
            return;
        }

        regmatch_t matches[1];  // Одно совпадение

        while (regexec(&reg, buffer, 1, matches, 0) == 0) {
            // Вывод совпавшей части строки
            for (int j = matches[0].rm_so; j < matches[0].rm_eo; j++) {
                printf("%c", buffer[j]);
            }
            printf("\n");

            // Сдвиг указателя буфера на следующую часть строки
            buffer += matches[0].rm_eo;
        }

        regfree(&reg);
    }
}
