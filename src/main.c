#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "utils.h"

#define SCRIPT_PATH "./scripts/get_updates.sh"
#define MAX_PACKAGES 1000
#define MAX_LEN 256

int process_args(int, char*[]);
int file_exists(const char *);
// Update functions
int check_updates(void);
int execute_pacman_script(void);
void evaluate_package_danger(FILE*,char **,int);
void get_repo_updates(FILE*);
void get_aur_updates(FILE*);

// News Functions
void check_news(void);


int process_args(int argc, char *argv[]) {

    if(argc > 2){
        printf("ERROR: The program should have one argument (Example: arch-checker news)");
        return 1;
    }

     if (argc == 1) {
        printf("No arguments passed.\n Checking updates...\n");
        check_updates();
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

int execute_pacman_script() {
    const char *cmd = 
        "echo \"Getting updates on packages...\"\n"
        "count=0\n"
        "mapfile -t packages < <(checkupdates | awk '{print $1}')\n"
        "for pkg in \"${packages[@]}\"; do\n"
        "    echo \"==== $pkg ====\"\n"
        "    pacman -Si \"$pkg\" | grep -E \"Name|Repo|Version\"\n"
        "    echo \"\"\n"
        "    count=$((count + 1))\n"
        "done\n"
        "echo \"Total packages with available updates: $count\"\n";

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("ERROR: Could not run the update check script");
        return 1;
    }

    print_output_from_fp(fp);

    return 0;
}



void get_repo_updates(FILE *logfile) {
    
    printf(
        "______                 ______          _                         \n"
        "| ___ \\                | ___ \\        | |                        \n"
        "| |_/ / __ _ ___  ___  | |_/ /_ _  ___| | ____ _  __ _  ___  ___ \n"
        "| ___ \\/ _` / __|/ _ \\ |  __/ _` |/ __| |/ / _` |/ _` |/ _ \\/ __|\n"
        "| |_/ / (_| \\__ \\  __/ | | | (_| | (__|   < (_| | (_| |  __/\\__ \\\n"
        "\\____/ \\__,_|___/\\___| \\_|  \\__,_|\\___|_|\\_\\__,_|\\__, |\\___||___/\n"
        "                                                  __/ |          \n"
        "                                                 |___/           \n"
    );

    
    if (execute_pacman_script() != 0) {
        printf("ERROR: Could not search for updates!\n");
        return;
    }

    char *packages[MAX_PACKAGES];
    int count = run_command_and_collect_lines("checkupdates | awk '{print $1}'", packages, MAX_PACKAGES, MAX_LEN);
    if (count < 0) return;

    evaluate_package_danger(logfile, packages, count);

    for (int i = 0; i < count; i++) {
        free(packages[i]);
    }

    printf("Press any key for the next step of the update checking");
    getchar();
}

void get_aur_updates(FILE *logfile) {
    printf(
        "  ___  _   _______  ______          _                         \n"
        " / _ \\| | | | ___ \\ | ___ \\        | |                        \n"
        "/ /_\\ \\ | | | |_/ / | |_/ /_ _  ___| | ____ _  __ _  ___  ___ \n"
        "|  _  | | | |    /  |  __/ _` |/ __| |/ / _` |/ _` |/ _ \\/ __|\n"
        "| | | | |_| | |\\ \\  | | | (_| | (__|   < (_| | (_| |  __/\\__ \\\n"
        "\\_| |_/\\___/\\_| \\_| \\_|  \\__,_|\\___|_|\\_\\__,_|\\__, |\\___||___/\n"
        "                                               __/ |          \n"
        "                                              |___/           \n"
    );

    
    
    const char *cmd =
        "yay -Qum | awk '{print $1}' | while read pkg; do "
        "echo \"==== $pkg ====\"; "
        "yay -Si \"$pkg\" | grep -E \"Name|Repo|Version\"; "
        "echo \"\"; "
        "done";

    char *packages[MAX_PACKAGES];
    int count = run_command_and_collect_lines("yay -Qum | awk '{print $1}'", packages, MAX_PACKAGES, MAX_LEN);
    if (count < 0) return;

    evaluate_package_danger(logfile, packages, count);

    for (int i = 0; i < count; i++) {
        free(packages[i]);
    }


    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("ERROR: Could not execute the AUR updates script!\n");
        return;
    }
    print_output_from_fp(fp);
    pclose(fp);

    printf("Press any key to continue...\n");
    getchar();
}



int check_updates() {
    FILE *logfile = fopen("update.log", "a");
    if (!logfile) {
        perror("Error: Could not open the log file\n");
        return 1;
    }

    time_t start = time(NULL);
    char *datetime = ctime(&start);
    datetime[strcspn(datetime, "\n")] = 0;

    fprintf(logfile, "-------------------- Log start time -> %s --------------------\n", datetime);    

    fprintf(logfile, "\n"
    "==================================================================\n"
    "==================  BASE PACKAGE UPDATES  ========================\n"
    "==================================================================\n\n");
    
    get_repo_updates(logfile);

    fprintf(logfile, "\n"
    "==================================================================\n"
    "==================  AUR PACKAGE UPDATES  =========================\n"
    "==================================================================\n\n");

    get_aur_updates(logfile);

    time_t end = time(NULL);
    datetime = ctime(&end);
    datetime[strcspn(datetime, "\n")] = 0;
    fprintf(logfile, "\n-------------------- Log end time -> %s --------------------\n\n", datetime);
    

    printf("Please check the logs to see the information in text format\nPress any key to close the program");
    getchar();

    
    fclose(logfile);
    return 0;
}



void check_news() {
    printf("\nNot implemented\n");
}



int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}