#include "utils.h"

// Consts and Variables

#define HIGH_RISK_COUNT 9

const char *high_risk_packages[HIGH_RISK_COUNT] = {
    "linux",
    "linux-firmware",
    "glibc",
    "systemd",
    "grub",
    "bash",
    "pacman",
    "openssl",
    "networkmanager"
};




// Functions
void print_output_from_fp(FILE *fp) {
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
}

int is_high_risk_package(const char *pkg) {
    for (int i = 0; i < HIGH_RISK_COUNT; i++) {
        size_t len = strlen(high_risk_packages[i]);
        if (strstr(pkg, high_risk_packages[i]) == 0) {
            if (pkg[len] == '\0' || pkg[len] == '-') {
                return 1;
            }
        }
    }
    return 0; 
}

int run_command_and_collect_lines(const char *cmd, char **lines, int max_lines, int max_len) {
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("Error running command");
        return -1;
    }

    int count = 0;
    char buffer[max_len];
    while (fgets(buffer, max_len, fp) != NULL && count < max_lines) {
        buffer[strcspn(buffer, "\n")] = 0;
        lines[count] = strdup(buffer);
        if (!lines[count]) {
            perror("strdup failed");
            break;
        }
        count++;
    }

    pclose(fp);
    return count;
}


void evaluate_package_danger(FILE *logfile,char **packages,int count) {
    int high_risk_found = 0;
    fprintf(logfile,"%d files needed to be updated\n", count);
    
    for (int i = 0; i < count; i++)
    {
        if(is_high_risk_package(packages[i]))
        {
            high_risk_found++;
            printf("\033[1;31m ALERT: Critical Package to update -> %s\033[0m\n", packages[i]);
            fprintf(logfile, "CRITICAL: %s needs updating\n", packages[i]);
        }else
        {
            fprintf(logfile, "Package: %s\n", packages[i]);
        }
        
    }

    if(high_risk_found > 0){
        printf("\033 Watch out! There are %d critical packages with pending updates!\033\n", high_risk_found);
        printf("\033[1;33m A backup before updating is highly recommended!.\033[0m\n");
        fprintf(logfile, "WARNING: %d critical packages with pending updates. Backup recommended.\n", high_risk_found);
        fprintf(logfile," ");
    } else
    {
        printf("Hurray!!! No critical packages need updating!\n");
        fprintf(logfile, "No critical packages needed updating!\n");
    }
    
}


