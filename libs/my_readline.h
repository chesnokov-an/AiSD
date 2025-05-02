#ifndef MY_READLINE_H
#define MY_READLINE_H

char *txt_readline(FILE* file_name);
char *bin_readline(FILE* file_name);
char *bin_read_n_symbols(FILE* file_name, unsigned n);
char *my_strip(char const * const);
FILE *input_correct_file();

#endif
