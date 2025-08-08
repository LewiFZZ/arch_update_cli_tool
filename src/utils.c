#include "utils.h"

// Consts and Variables

#define HIGH_RISK_COUNT 5

const char *high_risk_packages[HIGH_RISK_COUNT] = {
    "linux",
    "glibc",
    "systemd",
    "mesa",
    "nvidia"
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
        if (strcmp(pkg, high_risk_packages[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
} 