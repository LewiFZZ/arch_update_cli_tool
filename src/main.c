#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"

#define SCRIPT_PATH "./scripts/get_updates.sh"

int process_args(int, char*[]);
int file_exists(const char *);
void create_script_if_missing(void);
void check_updates(void);
void check_news(void);


int process_args(int argc, char *argv[]) {

    if(argc > 2){
        printf("ERROR: The program should have one argument (Example: ./archCheck news)");
        return 1;
    }

     if (argc == 1) {
        printf("No arguments passed. Checking updates and news...\n");
        check_updates();
        check_news();
    } else {
        if (strcmp(argv[1], "updates") == 0) {
            check_updates();
        } else if (strcmp(argv[1], "news") == 0) {
            check_news();
        } else {
            printf("ERROR: Unknown argument passed.\nOnly use update or news as arguments, or leave it blank for both\n");
            return 1;
        }
    }


    return 0;
    

}
int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void create_script_if_missing() {
    if (file_exists(SCRIPT_PATH)) {
        printf("Script '%s' already exists!\n", SCRIPT_PATH);
        return;
    }

    FILE *fp = fopen(SCRIPT_PATH, "w");
    if (fp == NULL)
    {
        perror("ERROR: The script could not be created");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "#!/bin/bash\n\n");
    fprintf(fp, "checkupdates | awk '{print $1}' | while read pkg; do\n");
    fprintf(fp, "    echo \"==== $pkg ====\"\n");
    fprintf(fp, "    pacman -Si \"$pkg\" | grep -E \"Name|Repo|Version\"\n");
    fprintf(fp, "    echo \"\"\n");
    fprintf(fp, "done\n");

    fclose(fp);

    // Make the script +X
    if (chmod(SCRIPT_PATH, 0755) != 0) {
        perror("ERROR: could not make the script with execution privilegdes\n");
        exit(EXIT_FAILURE);
    }


    printf("Script correctly created and made executable!");

    
}


void  check_updates() {
    create_script_if_missing();
}


void check_news() {
    printf("\nNot implemented\n");
}






int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}