#include <stdio.h>
#include <stdlib.h>


int main() {
    FILE *fp;
    char buffer[256];

    fp = popen("fastfetch", "r");

    if (fp == NULL) {
        perror("Error al ejecutar el comando");
        return 1;
    }


    printf("¡Bienvenido al mundo CLI de Linux!\nPulsa cualquier tecla para confirmar");
    getchar();


    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }


    pclose(fp);

    return 0;


}