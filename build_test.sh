#!/bin/bash

gcc main.c -o build/test

echo "Do you want to run the program? (Y/N)"
read -r ans

# Convertir a minúsculas para que acepte Y o y
ans=${ans,,}

if [[ "$ans" == "y" ]]; then
    ./build/test
else
    echo "Programa no ejecutado."
fi
