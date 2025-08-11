#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

void print_output_from_fp(FILE *fp);
int is_high_risk_package(const char *pkg);
int run_command_and_collect_lines(const char *cmd, char **lines, int max_lines, int max_len);
void evaluate_package_danger(FILE *logfile,char **packages,int count);


#endif
