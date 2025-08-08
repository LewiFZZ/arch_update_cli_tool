#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"

#define SCRIPT_PATH "./scripts/get_updates.sh"

int process_args(int, char*[]);
int file_exists(const char *);
int create_script_if_missing(void);
int check_updates(void);
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

int create_script_if_missing() {
    if (file_exists(SCRIPT_PATH)) {
        printf("Script '%s' already exists!\n", SCRIPT_PATH);

        if (chmod(SCRIPT_PATH, 0755) != 0) {
            perror("ERROR: could not set execution permissions on existing script");
            return 1;
        }
        return 0;
    }

    FILE *fp = fopen(SCRIPT_PATH, "w");
    if (fp == NULL)
    {
        perror("ERROR: The script could not be created");
        return 1;
    }

    fprintf(fp, "#!/bin/bash\n\n");

    fprintf(fp, "echo \"Getting updates on packages...\"\n\n");

    fprintf(fp, "count=0\n\n");

    fprintf(fp, "# Store the list in an array\n");
    fprintf(fp, "mapfile -t packages < <(checkupdates | awk '{print $1}')\n\n");

    fprintf(fp, "for pkg in \"${packages[@]}\"; do\n");
    fprintf(fp, "    echo \"==== $pkg ====\"\n");
    fprintf(fp, "    pacman -Si \"$pkg\" | grep -E \"Name|Repo|Version\"\n");
    fprintf(fp, "    echo \"\"\n");
    fprintf(fp, "    count=$((count + 1))\n");
    fprintf(fp, "done\n\n");

    fprintf(fp, "echo \"Total packages with available updates: $count\"\n");


    fclose(fp);

    // Make the script +X
    if (chmod(SCRIPT_PATH, 0755) != 0) {
        perror("ERROR: could not make the script with execution privilegdes\n");
        exit(EXIT_FAILURE);
    }


    printf("Script correctly created and made executable!");

    return 0;
}


int  check_updates() {
    if (create_script_if_missing() != 0) {
        printf("ERROR: Could not search for updates!\n");
        return 1;
    }

    FILE *fp = popen(SCRIPT_PATH, "r");
    if (!fp)
    {
        perror("ERROR: Could not execute the updates script!\n");
        return 2;
    }
    

    print_output_from_fp(fp);



    pclose(fp);
    remove(SCRIPT_PATH);
    
    printf("Press any key for the next step of the update checking");
    getchar();

    return 0;
}


void check_news() {
    printf("\nNot implemented\n");
}






int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}