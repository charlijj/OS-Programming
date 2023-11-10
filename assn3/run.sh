#!/bin/bash


make

ls

if [ $# -eq 1 ]; then
    if [ "$1" = "test" ]; then
        ./assign3 < "test.txt"
    else
        echo "Invalid argument. Usage: $0 test"
        exit 1
    fi
else
    while true; do
        ./assign3
        read -p "Do you want to run it again? (y/n): " choice
        if [ "$choice" = "n" ]; then
            break
        fi
    done
fi


exit 0
