#ifndef S21_GREP_H
#define S21_GREP_H

typedef struct {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
} grepOptions;

typedef struct {
  char *fileForF[1024];
  char *patterns[1024];
  grepOptions opt;
  int numOfpatterns;  // количество шаблонов для поиска
  int PatternFiles;  // количество файлов, содержащих шаблоны для поиска
  int emptystr;
  int err0;
  char inv_opt;  
} parameters;


int correctInput(int argc, parameters *params); 
void getOption(int argc, char **argv, parameters *params);
int parser(parameters *params, int i, char **argv, int argc);
int patternWithoutE(int argc, char **argv, parameters *params);
void patternEorFileF(char **argv, int i, int k, parameters *params);
void copyEOstr(char *dest, const char *src, int k);
int numberOfFiles(int argc, char **argv);
void patternsFromFile(parameters *params);
void grepFunc(char *line, char *patterns, parameters *params, int *isMatch);
void reader(int argc, char **argv, parameters *params);
void v_flag(int *match);
void cl_flag(int match, int *counter, grepOptions opt);
void cl_output(grepOptions opt, int cntFilesForSearch, const char *nameOfFile,
               int counter);
void output(int cntFilesForSearch, parameters *params, const char *nameOfFile, int num, const char *buffer, int match);
void print(const char *buffer);
void cleanMemory(parameters *params);
void printMatchedParts(const char *buffer, parameters *params);

#endif  // S21_GREP_H
