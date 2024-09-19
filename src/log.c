#include "log.h"

void create_directories(char *directory) {
	char *command = (char *)malloc(strlen("mkdir -p ") + strlen(directory) + 1);
	
	sprintf(command, "mkdir -p %s", directory);
	system(command);
	
	free(command);
}

void initialize_log_file(char *directory, char *filename) {
	char *path = (char *)malloc(strlen(directory) + strlen(filename) + 2);
	
	sprintf(path, "%s/%s", directory, filename);
	
	FILE *file = fopen(path, "w");
	
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	fclose(file);
	free(path);
}

void write_to_log_file(char *directory, char *filename, char *message) {
	char *path = (char *)malloc(strlen(directory) + strlen(filename) + 2);
	
	sprintf(path, "%s/%s", directory, filename);
	
	FILE *file = fopen(path, "a+");
	
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	fprintf(file, "%s\n", message);
	
	fclose(file);
	free(path);
}



