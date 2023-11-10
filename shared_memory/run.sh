#!/bin/bash

echo -e "\nRunning program...\n\n"
make
./P_shared_mem && ./Q_shared_mem
