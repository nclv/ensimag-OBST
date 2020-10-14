Un _arbre binaire de recherche_ (**ABR**) est un arbre binaire tel que la valeur en tout nœud est strictement supérieure aux valeurs des nœuds de son sous arbre gauche et inférieure ou égale à celles de son sous arbre droit.
Les ABR sont une structure de données utilisée pour implémenter un dictionnaire, c’est à dire un ensemble totalement ordonné d’éléments. La complexité en pire cas de la recherche d’un élément dans un ABR est la profondeur de l’ABR.

Le but de ce TP est de générer un programme implémentant un ABR qui minimise le nombre moyen de
comparaisons lors d’une opération de recherche (i.e. sa profondeur moyenne) ; un tel ABR est dit _optimal_.

Code: https://stackoverflow.com/questions/16987670/dynamic-programming-why-knuths-improvement-to-optimal-binary-search-tree-on2
https://github.com/titu1994/Optimal-Binary-Search-Tree/blob/master/Optimal%20Binary%20Search%20Tree/src/Optimal%20Binary%20Search%20Tree.c
http://www.eecs.tufts.edu/~aloupis/comp150/projects/Cori_Jacoby_Alex_King_Final_Report_v2.pdf


```bash
make binary
./bin/compileBST 3 frequences.txt
make clean
```

`frequences.txt` contient `2 4 4`, `5 3 2`, `6 2 2`, `3 2 5`, `1 3 6`. `n` vaut le nombre d'éléments dans `frequences.txt`

# Q1
Voir http://web.eecs.utk.edu/~leparker/Courses/CS581-spring14/Lectures/7-Jan-30-Opt-Binary-Search-no-answers.pdf et http://www.euroinformatica.ro/documentation/programming/!!!Algorithms_CORMEN!!!/DDU0091.html

On effectue un pré-calcul $somme_p(i, j) = \sum_{l=i}^{j-1}p_l$ pour tout $0 \leq i < j < n$. On accède ensuite à la somme partielle des probabilités entre $i$ et $j$ par $\lstinline{sommes_p(i, j)}$.

```C
/** 
 * La valeur à l'emplacement (i, j) dans la matrice triangulaire supérieure se trouve 
 * dans la mémoire à l'emplacement (i * (2 * (n - 1) - i + 1)) / 2 + j.
*/
#define sommes_p(i, j, n) (sommes_p[((i) * (2 * ((n) - 1) - (i) + 1)) / 2 + (j)])


/**
 * Calcul des probabilitées et stockage dans un array 1d. Complexité spatiale et temporelle en O((n * (n + 1)) / 2).
 *
 * @param probabilites tableau des probabilités
 * @param n taille de la matrice carrée générée, n == len(probabilites) + 1
 */
double *calculer_sommes_opti(double *probabilites, size_t n) {
    /* On a n * (n + 1) / 2 éléments dans la matrice triangulaire supérieure avec la diagonale centrale nulle */
    double *sommes_p = malloc((n * (n + 1)) / 2 * sizeof(double));
    /* Calcul des sommes des probabilités */
    for (size_t i = 0; i < n; ++i) {
        /* Initialisation de la diagonale à p_0 */
        sommes_p(i, i, n) = 0;
        for (size_t j = i + 1; j < n; ++j) {
            sommes_p(i, j, n) = sommes_p(i, j - 1, n) + probabilites[j - 1];
        }
    }
    return sommes_p;
}
```
ou
```C
#define sommes_p(i, j) (sommes_p[j] - sommes_p[i])

double *sommes_p = malloc(N * sizeof(double));

double somme = 0;
for (size_t i = 0; i < N; ++i) {
	somme += probabilites[i];
	sommes_p[i] = somme;
}
```

# Q2
voir https://pdf.sciencedirectassets.com/271538/1-s2.0-S0304397500X00722/1-s2.0-S0304397596003209/main.pdf et https://blog.asarkar.com/assets/docs/algorithms-curated/Optimal%20BST%20-%20Carnegie%20Mellon%20University.pdf

On a $\frac{n(n-1)}{2}$ possibilités de $i$ et $j$ tels que $0 \leq i < j < n$. L'espace requis est donc en $\Theta(n^2)$.

On sait que tout sous arbre d'un ABR optimal est un ABR optimal. On choisit donc de prendre comme racine tous les noeuds de l'ABR en faisant varier $k$ de $i$ à $j$. Lorsque l'on choisit que le $k$ème noeuds est la racine, on calcule récursivement le coût optimal de $i$ à $k - 1$ et de $k + 1$ à $j$. Comme l'on choisit $k$ parmi $j - i$ possibilités pour $i$ et $j$ tels que $0 \leq i < j < n$, la complexité temporelle est en $\Theta(n^3)$

On peut aussi le voir de cette façon:
Il y a un total de $n^2$ sous-problèmes (ou du moins en $\Theta(n^2)$) et chaque sous-problème est de complexité $\Theta(n)$ si l'on suppose que tous ses sous-problèmes sont déjà résolus. On a donc une complexité totale en $\Theta(n^3)$.

# Q3
voir les lien Q2
voir ce que le résultat permet https://www.student.cs.uwaterloo.ca/~cs466/Old_courses/F03/3-optimal_search_trees.pdf


---

Nous considérons maintenant la recherche dans un arbre binaire équilibré contenant
$n$ éléments et de hauteur $O(log_2(n))$.

Pour stocker notre arbre binaire, on utilise un tableau `bst[n][2]`. On ne stocke pas la racine, obtenu en `C(0, n - 1)`, et on stocke les fils gauche et droit du noeud stocké en `bst[i]` dans les cases `bst[?][0]` et `bst[?][1]`.

Dès que l’on s’éloigne de la racine, une ligne de cache ne contient que des noeuds qui ne sont pas sur le même chemin racine-feuille. On a donc besoin d’un défaut de cache par noeud de l’arbre pour un coût total de : $Q(n) = h = O(log_2(n))$.

The algorithm has poor cache performance. To see this consider the computation of the d-th diagonal. In computing the entries of the d-th diagonal all the entries of C to _the left and below the d-th diagonal are accessed._

Time complexity = n(n + 1)(n + 2)/6

---

```bash
$gnuplot
gnuplot> set key outside
gnuplot> set grid
gnuplot> plot for [col=1:3] 'benchmarks/benchmark6.perf' using 0:col with yerrorlines
```

```bash
$lscpu > hardware
$valgrind --tool=cachegrind --log-file=valgrind-cachegrindijk.txt ./bin/compileBST 5000 benchmarks/benchmark6.in
$sudo perf stat -e task-clock,cycles,instructions,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./bin/compileBST 5000 benchmarks/benchmark6.in
$sudo perf record -e cache-misses ./bin/compileBST 5000 benchmarks/benchmark6.in
$sudo perf report --stdio --header

function colstat {
	awk -v col=$1 '{if(min==""){min=max=$col}; if(col>max) {max=$col}; if($col<min) {min=$col}; total+=$col; count+=1} END {print "avg " total/count," | max "max," | min " min}' $2;
}
```