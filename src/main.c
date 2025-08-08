#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"

#define SCRIPT_PATH "./scripts/get_updates.sh"
#define MAX_PACKAGES 1000
#define MAX_LEN 256

int process_args(int, char*[]);
int file_exists(const char *);
// Update functions
int check_updates(void);
int create_script_if_missing(void);
void evaluate_package_danger(char**, int);
void get_repo_updates(void);
void get_aur_updates(void);

// News Functions
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

void evaluate_package_danger(char **packages,int count) {
    int high_risk_found = 0;
    
    for (int i = 0; i < count; i++)
    {
        if(is_high_risk_package(packages[i]))
        {
            high_risk_found++;
            printf("\033[1;31mALERT: Critical Package to update -> %s\033[0m\n", packages[i]);
        }
    }

    if(high_risk_found > 0){
        printf("\033 Watch out! There are %d critical packages with pending updates!\033\n", high_risk_found);
        printf("\033[1;33mA backup before updating is highly recommended!.\033[0m\n");
    } else
    {
        printf("Hurray!!! No critical packages need updating!\n");
    }
    
    
}

void get_repo_updates() {
    if (create_script_if_missing() != 0) {
        printf("ERROR: Could not search for updates!\n");
        return;
    }

    FILE *fp = popen(SCRIPT_PATH, "r");
    if (!fp) {
        perror("ERROR: Could not execute the updates script!\n");
        return;
    }

    print_output_from_fp(fp);

    pclose(fp);
    remove(SCRIPT_PATH);

    char *packages[MAX_PACKAGES];
    int count = 0;
    char buffer[MAX_LEN];

    FILE *packageNames = popen("checkupdates | awk '{print $1}'", "r");
    if (!packageNames) {
        perror("ERROR: Could not get the package names");
        return;
    }

    while (fgets(buffer, MAX_LEN, packageNames) != NULL && count < MAX_PACKAGES) {
        buffer[strcspn(buffer, "\n")] = '\0';

        packages[count] = strdup(buffer); 
        if (packages[count] == NULL) {
            perror("strdup failed");
            
            break;
        }
        count++;
    }

    pclose(packageNames);

    evaluate_package_danger(packages, count);

    for (int i = 0; i < count; i++) {
        free(packages[i]);  
    }


    printf("Press any key for the next step of the update checking");
    getchar();
}

void get_aur_updates(void) {
    const char *aur_script_path = "./scripts/get_aur_updates.sh";

    if (!file_exists(aur_script_path)) {
        FILE *fp = fopen(aur_script_path, "w");
        if (fp == NULL) {
            perror("ERROR: Could not create AUR update script");
            return;
        }

        fprintf(fp, "#!/bin/bash\n\n");
        fprintf(fp, "echo \"Getting updates on AUR packages...\"\n\n");
        fprintf(fp, "count=0\n\n");
        fprintf(fp, "mapfile -t packages < <(yay -Qum | awk '{print $1}')\n\n");
        fprintf(fp, "for pkg in \"${packages[@]}\"; do\n");
        fprintf(fp, "    echo \"==== $pkg ====\"\n");
        fprintf(fp, "    yay -Si \"$pkg\" | grep -E \"Name|Repo|Version\"\n");
        fprintf(fp, "    echo \"\"\n");
        fprintf(fp, "    count=$((count + 1))\n");
        fprintf(fp, "done\n\n");
        fprintf(fp, "echo \"Total AUR packages with available updates: $count\"\n");

        fclose(fp);

        if (chmod(aur_script_path, 0755) != 0) {
            perror("ERROR: could not make AUR script executable");
            return;
        }
    } else {
        if (chmod(aur_script_path, 0755) != 0) {
            perror("ERROR: could not set execution permissions on AUR script");
            return;
        }
        printf("Script '%s' already exists!\n", aur_script_path);
    }


    FILE *fp = popen(aur_script_path, "r");
    if (!fp) {
        perror("ERROR: Could not execute the AUR updates script!");
        return;
    }

    print_output_from_fp(fp);

    pclose(fp);
    remove(aur_script_path);

    printf("Press any key to continue...");
    getchar();
}



int  check_updates() {
    get_repo_updates();
    get_aur_updates();


    return 0;
}


void check_news() {
    printf("\nNot implemented\n");
}



int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}