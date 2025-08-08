#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_args(int, char*[]);
void check_updates(void);
void check_news(void);


int process_args(int argc, char *argv[]) {

    if(argc > 2){
        printf("ERROR: The program should have one argument (Example: ./archCheck news)");
        return 1;
    }
    

}









int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}