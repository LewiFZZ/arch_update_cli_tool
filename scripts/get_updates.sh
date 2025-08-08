#!/bin/bash

# Check updates to obtain the list and then by packages
checkupdates | awk '{print $1}' | while read -r pkg; do
    pacman -Si "$pkg" | grep -E "Name|Repo|Version"
done
