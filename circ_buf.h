#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_STRING_SIZE 512


int circ_find_last_entry_offset(FILE * fp);
int circ_write_entry(FILE *fp, char * buffer);
int circ_open_and_write(char * filename, const int strings_size, const int strings_count, char * buffer);


#endif
