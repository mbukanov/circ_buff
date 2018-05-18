#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_STRING_SIZE 512

static int max_string_size = 0;
static int max_strings_count = 0;

int file_exists(char * filename) {
    /* 1 - file exists; 0 - file not exists */
    return (access( filename, F_OK ) != -1);
}

int circ_find_last_entry_offset(FILE * fp) {
	char buffer[MAX_STRING_SIZE] = "";
	time_t max_timestamp = 0;
	long current_offset = 0;
	time_t timestamp = 0;
	while(fread(buffer, max_string_size, 1, fp) > 0) {
		sscanf(buffer, "%ld, %s", &timestamp, buffer);	
		if(timestamp >= max_timestamp) {
			max_timestamp = timestamp;
			current_offset = ftell(fp);
		}
	}
	return current_offset;
}


int circ_write_entry(FILE *fp, char * buffer) {
	if(fp == NULL) return -1;
	char new_buffer[MAX_STRING_SIZE] = "";
	time_t timestamp = time(NULL);
	snprintf(new_buffer, max_string_size, "%ld, %s", timestamp, buffer);
	new_buffer[max_string_size-1] = '\n';
	fwrite(new_buffer, max_string_size, 1, fp);
	return 0;
}

int circ_open_and_write(char * filename, const int strings_size, const int strings_count, char * buffer) {
	FILE * fp = NULL;
	max_strings_count = strings_count;
	max_string_size = strings_size;
	int max_size = max_strings_count * max_string_size;
	if(!file_exists(filename)) {
		fp = fopen(filename, "w");
		if(fp != NULL) fclose(fp);
	}
	fp = fopen(filename, "r+");
	int last_entry_offset = circ_find_last_entry_offset(fp);
    if( (last_entry_offset + max_string_size) > max_size ) {
        fseek(fp, 0, SEEK_SET);
    } else {
        fseek(fp, last_entry_offset, SEEK_SET);
    }
    circ_write_entry(fp, buffer);
    lseek(fp, 0, SEEK_END);
    fclose(fp);
}

