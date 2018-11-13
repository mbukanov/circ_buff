#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "circ_buf.h"

#define MAX_STRING_SIZE 512

static int max_string_size = 0;
static int current_strings_count = 0;
static int max_strings_count = 0;


int file_exists(const char * filename) {
    /* 1 - file exists; 0 - file not exists */
    return (access( filename, F_OK ) != -1);
}

int circ_find_last_entry_offset(FILE * fp) {
	char buffer[MAX_STRING_SIZE] = "";
	long current_offset = 0;
	int index = 0;
	while(fread(buffer, max_string_size, 1, fp) > 0) {
		sscanf(buffer, "%d, %s", &index, buffer);	
		if(index >= current_strings_count) {
			current_strings_count = index;
			current_offset = ftell(fp);
		}
	}
	return current_offset;
}

void format_fwrite(FILE * fp, const char * format, ...) {
	char buffer[MAX_STRING_SIZE] = "";
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	fwrite(buffer, max_string_size, 1, fp);
}

int circ_write_entry(FILE *fp, char * buffer) {
	if(!fp) return -1;
	format_fwrite(fp, "%ld, %s\n", (++current_strings_count), buffer);
	return 0;
}

void create_file(const char *filename) {
	FILE * fp = fopen(filename, "w");
	if(fp)
		fclose(fp);
}

int circ_open_and_write(const char * filename, const int strings_size, const int strings_count, char * buffer) {
	max_strings_count = strings_count;
	max_string_size = strings_size;
	int max_size = max_strings_count * max_string_size;
	if(!file_exists(filename)) 
		create_file(filename);

	FILE * fp = fopen(filename, "r+");
	if(!fp) return -1;
	int last_entry_offset = circ_find_last_entry_offset(fp);
    if( (last_entry_offset + max_string_size) > max_size ) {
        fseek(fp, 0, SEEK_SET);
    } else {
        fseek(fp, last_entry_offset, SEEK_SET);
    }
    circ_write_entry(fp, buffer);
    fseek(fp, 0, SEEK_END);
    fclose(fp);
	return 0;
}

