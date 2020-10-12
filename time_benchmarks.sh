#!/bin/bash
# time_benchmarks.sh, version 1

# Calcul du temps mis par les différents programmes Bellman
# Exécuter les 3 fonctions Bellmans dans le fichier compileBST.c
# Exécution de bin/compileBST 10 fois, si deux appels à time_benchmarks.sh
# sont effectués alors il y aura 20 lignes dans les fichiers .perf

# Usage: ./time_benchmarks.sh

BENCHMARKS_DIR=benchmarks
TIME=".perf"

for input_file in "$BENCHMARKS_DIR"/*.in;
do
    echo "Input : ${input_file##*/}"
    n=$(($(grep -o '  ' "$input_file" | wc -l) + 1))
    echo "Nombre d'éléments: $n"

    output_file="${input_file%.in}"$TIME
    echo "Output: ${output_file##*/}"
    for _ in {1..10};
    do
        ./bin/compileBST $n "$input_file" >> "$output_file"
    done
    echo 
done

exit 0