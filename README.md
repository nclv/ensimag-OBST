Un _arbre binaire de recherche_ (**ABR**) est un arbre binaire tel que la valeur en tout nœud est strictement supérieure aux valeurs des nœuds de son sous arbre gauche et inférieure ou égale à celles de son sous arbre droit.
Les ABR sont une structure de données utilisée pour implémenter un dictionnaire, c’est à dire un ensemble totalement ordonné d’éléments. La complexité en pire cas de la recherche d’un élément dans un ABR est la profondeur de l’ABR.

Le but de ce TP est de générer un programme implémentant un ABR qui minimise le nombre moyen de
comparaisons lors d’une opération de recherche (i.e. sa profondeur moyenne) ; un tel ABR est dit _optimal_.