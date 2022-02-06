#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_line(char *pid, int depth) {
    // open stat file for pid
    FILE *fp;
    char *filename;
    asprintf(&filename, "/proc/%s/stat", pid);
    fp = fopen(filename, "r");
    free(filename);

    char file[200000];
    if (fp == NULL) {
    	printf("stat: No such file.\n");
    	return;
    }
    fgets(file, sizeof(file), fp);
    fclose(fp);

    // initialize variables
    char *ppid, *vsize, *comm;
    ppid = (char *) malloc(100);
    vsize = (char *) malloc(100);
    comm = (char *) malloc(1000);

    // scan & gather info
    char *save;
    char* token = strtok_r(file, " ", &save);
    int i = 0;
	while (token != NULL) {
		if (i == 1) {strcpy(comm, token);}
		else if (i == 3) {strcpy(ppid, token);}
		else if (i == 22) {strcpy(vsize, token);}
		token = strtok_r(NULL, " ", &save);
		i++;
	}

    // print current process info
	for (int j = 0; j < depth; j++) {printf("  ");}
    printf("%s %s, %s kb\n", pid, comm, vsize);

    // open child file
    FILE *cf;
    char *chi_filename;
    asprintf(&chi_filename, "/proc/%s/task/%s/children", pid, pid);
    cf = fopen(chi_filename, "r");
    free(chi_filename);

    // loop through and call print_line on all children
    char chi_file[200000];
    fgets(chi_file, sizeof(chi_file), cf);
    fclose(cf);

    // base case: no kids
    if (chi_file == NULL) {
    	printf("chi_file: No such file. \n");
    } else {
        char *chi_save;
        char* chi_token = strtok_r(chi_file, " ", &chi_save);
        while (chi_token != NULL) {
            print_line(chi_token, depth + 1);
            chi_token = strtok_r(NULL, " ", &chi_save);
    	}
    }

    free(ppid);
    free(vsize);
    free(comm);
}

int main(int argc, char **argv) {
    print_line("1", 0);
    return 0;
}
