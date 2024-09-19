#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// To create a log file we need to
/*
1. ensure directories exist
2. open file
3. write to file
4. close file
*/

typedef struct {
	char *filename;
	char *directory;
} Log;

void create_directories(char *directory);
void initialize_log_file(char *directory, char *filename);
void write_to_log_file(char *directory, char *filename, char *message);


#endif