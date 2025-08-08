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

     if (argc == 1) {
        printf("No arguments passed. Checking updates and news...\n");
        check_updates();
        check_news();
    } else {
        if (strcmp(argv[1], "update") == 0) {
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









int main(int argc, char *argv[] ) {
    return process_args(argc, argv);
}