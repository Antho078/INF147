#pragma once
#define  DIM   3            // la largeur de la plaque (3x3 cases en tout)
#define  VIDE  0            // la valeur de la case vide 
#define  NBCOUPS  200  // le nombre de coups � faire pour m�langer la plaque initiale

/* Type �num�r� pour les 4 d�placements possibles */
typedef enum { NULLE = -1, HAUT, GAUCHE, BAS, DROITE } t_direction;
/* Type-tableau pour la plaque de jeu */
typedef int t_plaque[DIM][DIM];
/* Constante qui illustre la plaque-solution � trouver */
static const t_plaque SOLUTION = { {1, 2, 3}, {4, 5, 6}, {7, 8, VIDE} };

//Cette fonction recherche la position(ligne et colonne) du num�ro � num � dans la plaque de jeu pla.Elle met � jour les r�f�rences posy et posx avec les indices de la ligne et de la colonne o� se trouve ce num�ro.NOTE : Cette fonction suppose que le num�ro num existe dans la plaque.
void trouver_numero(const t_plaque pla, int num, int* posy, int* posx);

//Cette fonction re�oit une plaque de jeu, une direction valide ainsi que la position[py_0, px_0] de la case vide.Si le d�placement demand� est valide � ne d�placera pas la case vide en dehors des limites de la plaque � la fonction effectuera le d�placement de la case vide dans la direction voulue, va mettre � jour la position[py_0, px_0] de cette case et va renvoyer 1.  Sinon, elle renvoie 0 et la plaque restera inchang�e.
int changer_plaque(t_plaque pla, t_direction dir, int* py_0, int* px_0);

//Cette fonction commence par copier l��tat - solution dans la plaque re�ue en param�tre.Ensuite elle va effectuer � NBCOUPS � d�placements al�atoires de la case vide pour bien m�langer la plaque de d�part.Assurez - vous que la position[py_0, px_0] de la case vide a bien �t� initialis�e apr�s avoir m�lang�e la plaque.
void melanger_plaque(t_plaque pla, int* py_0, int* px_0);

//Cette fonction sera utile pour l�algorithme de recherche de la solution(2i�me partie du TP).Elle �value le � co�t minimal estim� � pour atteindre l��tat - solution � partir de la plaque re�ue en param�tre.Le � co�t minimal estim� � est simplement la somme des distances verticales / horizontales qui s�parent chaque num�ro(ou chaque case non - vide) de sa position finale d�sir�e(dans la plaque - solution).Voici la formule du co�t minimal estim� :
int  evaluer_plaque(const t_plaque pla);

//Fonction qui affiche simplement la plaque re�ue � l��cran, � DIM � valeurs par ligne.
void afficher_plaque(const t_plaque pla);
