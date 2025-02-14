#pragma once
#define  DIM   3            // la largeur de la plaque (3x3 cases en tout)
#define  VIDE  0            // la valeur de la case vide 
#define  NBCOUPS  200  // le nombre de coups à faire pour mélanger la plaque initiale

/* Type énuméré pour les 4 déplacements possibles */
typedef enum { NULLE = -1, HAUT, GAUCHE, BAS, DROITE } t_direction;
/* Type-tableau pour la plaque de jeu */
typedef int t_plaque[DIM][DIM];
/* Constante qui illustre la plaque-solution à trouver */
static const t_plaque SOLUTION = { {1, 2, 3}, {4, 5, 6}, {7, 8, VIDE} };

//Cette fonction recherche la position(ligne et colonne) du numéro « num » dans la plaque de jeu pla.Elle met à jour les références posy et posx avec les indices de la ligne et de la colonne où se trouve ce numéro.NOTE : Cette fonction suppose que le numéro num existe dans la plaque.
void trouver_numero(const t_plaque pla, int num, int* posy, int* posx);

//Cette fonction reçoit une plaque de jeu, une direction valide ainsi que la position[py_0, px_0] de la case vide.Si le déplacement demandé est valide – ne déplacera pas la case vide en dehors des limites de la plaque – la fonction effectuera le déplacement de la case vide dans la direction voulue, va mettre à jour la position[py_0, px_0] de cette case et va renvoyer 1.  Sinon, elle renvoie 0 et la plaque restera inchangée.
int changer_plaque(t_plaque pla, t_direction dir, int* py_0, int* px_0);

//Cette fonction commence par copier l’état - solution dans la plaque reçue en paramètre.Ensuite elle va effectuer « NBCOUPS » déplacements aléatoires de la case vide pour bien mélanger la plaque de départ.Assurez - vous que la position[py_0, px_0] de la case vide a bien été initialisée après avoir mélangée la plaque.
void melanger_plaque(t_plaque pla, int* py_0, int* px_0);

//Cette fonction sera utile pour l’algorithme de recherche de la solution(2ième partie du TP).Elle évalue le « coût minimal estimé » pour atteindre l’état - solution à partir de la plaque reçue en paramètre.Le « coût minimal estimé » est simplement la somme des distances verticales / horizontales qui séparent chaque numéro(ou chaque case non - vide) de sa position finale désirée(dans la plaque - solution).Voici la formule du coût minimal estimé :
int  evaluer_plaque(const t_plaque pla);

//Fonction qui affiche simplement la plaque reçue à l’écran, « DIM » valeurs par ligne.
void afficher_plaque(const t_plaque pla);
