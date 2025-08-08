#include "utils.h"


void print_output_from_fp(FILE *fp) {
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
}