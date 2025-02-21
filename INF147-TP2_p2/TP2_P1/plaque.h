#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DIM 3       // la largeur de la plaque (3x3 cases en tout)
#define VIDE 0      // la valeur de la case vide
#define NBCOUPS 200 // le nombre de coups à faire pour mélanger la plaque initiale

/* Type énuméré pour les 4 déplacements possibles */
typedef enum
{
   NULLE = -1,
   HAUT,
   GAUCHE,
   BAS,
   DROITE
} t_direction;
/* Type-tableau pour la plaque de jeu */
typedef int t_plaque[DIM][DIM];
/* Constante qui illustre la plaque-solution à trouver */
static const t_plaque SOLUTION = {{1, 2, 3}, {4, 5, 6}, {7, 8, VIDE}};

/*===========================================================================*/
/* Déclaration des fonctions publiques */
/*===========================================================================*/

/*===========================================================================*/
/*
   Fonction    : trouver_numero()
   Description : Recherche la position (ligne et colonne) du numéro « num »
                 dans la plaque de jeu pla. Met à jour les références posy et
                 posx avec les indices de la ligne et de la colonne où se
                 trouve ce numéro.

   Paramètres  : - pla  : La plaque dans laquelle on cherche le numéro
                 - num  : Le numéro recherché
                 - posy : Pointeur pour stocker la position verticale
                 - posx : Pointeur pour stocker la position horizontale

   Retour      : Aucun (les positions sont mises à jour via les pointeurs)
*/
/*===========================================================================*/
void trouver_numero(const t_plaque pla, int num, int *posy, int *posx);

/*===========================================================================*/
/*
   Fonction    : changer_plaque()
   Description : Déplace la case vide dans la direction indiquée si le
                 déplacement est valide.

   Paramètres  : - pla  : La plaque de jeu
                 - dir  : La direction du déplacement
                 - py_0 : Pointeur vers la position verticale de la case vide
                 - px_0 : Pointeur vers la position horizontale de la case vide

   Retour      : - 1 si le déplacement est valide et effectué
                 - 0 sinon (aucun changement dans la plaque)
*/
/*===========================================================================*/
int changer_plaque(t_plaque pla, t_direction dir, int *py_0, int *px_0);

/*===========================================================================*/
/*
   Fonction    : melanger_plaque()
   Description : Initialise la plaque en copiant l’état-solution, puis effectue
                 un nombre défini de déplacements aléatoires de la case vide
                 pour mélanger la plaque.

   Paramètres  : - pla  : La plaque de jeu à mélanger
                 - py_0 : Pointeur vers la position verticale de la case vide
                 - px_0 : Pointeur vers la position horizontale de la case vide

   Retour      : Aucun (modifie la plaque en place)
*/
/*===========================================================================*/
void melanger_plaque(t_plaque pla, int *py_0, int *px_0);

/*===========================================================================*/
/*
   Fonction    : evaluer_plaque()
   Description : Calcule le coût minimal estimé pour atteindre l’état-solution
                 à partir de la plaque reçue en paramètre. Le coût est basé
                 sur la somme des distances verticales et horizontales entre
                 chaque numéro et sa position finale désirée.

   Paramètres  : - pla : La plaque de jeu à évaluer

   Retour      : - Le coût minimal estimé
*/
/*===========================================================================*/
int evaluer_plaque(const t_plaque pla);

/*===========================================================================*/
/*
   Fonction    : afficher_plaque()
   Description : Affiche la plaque reçue à l’écran, avec « DIM » valeurs par ligne.

   Paramètres  : - pla : La plaque à afficher

   Retour      : Aucun (affichage sur la sortie standard)
*/
/*===========================================================================*/
void afficher_plaque(const t_plaque pla);
