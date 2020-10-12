#!/bin/bash
# check_benchmarks.sh, version 1

# Vérifie le résultat des programmes Bellman
# N'exécuter que la fonction Bellman qui vous intéresse (pas les 3) dans
# le fichier compileBST.c

# Usage: ./check_benchmarks.sh (dik.out|ijk.out|jik.out)

BENCHMARKS_DIR=benchmarks
BELLMAN_FUNCTION="$1"

for input_file in "$BENCHMARKS_DIR"/*.in;
do
    echo "Input : ${input_file##*/}"
    n=$(($(grep -o '  ' "$input_file" | wc -l) + 1))
    echo "Nombre d'éléments: $n"

    output_file="${input_file%.in}"$BELLMAN_FUNCTION
    echo "Output: ${output_file##*/}"
    ./bin/compileBST $n "$input_file" > "$output_file"

    diff "${input_file%.in}.out" $output_file
    echo 
done

exit 0