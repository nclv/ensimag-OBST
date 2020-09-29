Un _arbre binaire de recherche_ (**ABR**) est un arbre binaire tel que la valeur en tout nœud est strictement supérieure aux valeurs des nœuds de son sous arbre gauche et inférieure ou égale à celles de son sous arbre droit.
Les ABR sont une structure de données utilisée pour implémenter un dictionnaire, c’est à dire un ensemble totalement ordonné d’éléments. La complexité en pire cas de la recherche d’un élément dans un ABR est la profondeur de l’ABR.

Le but de ce TP est de générer un programme implémentant un ABR qui minimise le nombre moyen de
comparaisons lors d’une opération de recherche (i.e. sa profondeur moyenne) ; un tel ABR est dit _optimal_.

Demo pour Adrien: https://blog.asarkar.com/assets/docs/algorithms-curated/Optimal%20BST%20-%20Carnegie%20Mellon%20University.pdf

Code: https://stackoverflow.com/questions/16987670/dynamic-programming-why-knuths-improvement-to-optimal-binary-search-tree-on2
https://github.com/titu1994/Optimal-Binary-Search-Tree/blob/master/Optimal%20Binary%20Search%20Tree/src/Optimal%20Binary%20Search%20Tree.c
http://www.eecs.tufts.edu/~aloupis/comp150/projects/Cori_Jacoby_Alex_King_Final_Report_v2.pdf


# Q1
Voir http://web.eecs.utk.edu/~leparker/Courses/CS581-spring14/Lectures/7-Jan-30-Opt-Binary-Search-no-answers.pdf et http://www.euroinformatica.ro/documentation/programming/!!!Algorithms_CORMEN!!!/DDU0091.html

On effectue un pré-calcul $somme_p(i, j) = \sum_{l=i}^{j-1}p_l$ pour tout $0 \leq i < j < n$.

```C
#define N (10)

/* 
  * La valeur à l'emplacement (i, j) dans la matrice triangulaire supérieure se trouve 
  * dans la mémoire à l'emplacement (i * (2 * (n - 1) - i + 1)) / 2 + j.
*/
#define sommes_p(i, j) (somme_p[((i) * (2 * (N - 1) - (i) + 1)) / 2 + (j)])

/* On a n * (n + 1) / 2 éléments dans la matrice triangulaire supérieure avec la diagonale centrale nulle */
double *sommes_p = malloc((N * (N + 1)) / 2 * sizeof(double));

assert(0 <= i && i < j && j < N);

/* Calcul des sommes des probabilités */
for (size_t i = 0; i < N; ++i) {
    /* Initialisation de la diagonale à 0 */
    somme_p(i, i) = 0;
    for (size_t j = i + 1; j < n; ++j) {
	    somme_p(i, j) = somme_p(i, j - 1) + probabilites[j];
    }
}
```

# Q2
voir https://pdf.sciencedirectassets.com/271538/1-s2.0-S0304397500X00722/1-s2.0-S0304397596003209/main.pdf

# Q3
voir lien Q2
voir ce que le résultat permet https://www.student.cs.uwaterloo.ca/~cs466/Old_courses/F03/3-optimal_search_trees.pdf
