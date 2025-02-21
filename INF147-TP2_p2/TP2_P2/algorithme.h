#pragma once

#include "plaque.h"
#define MAXNOEUDS  1000     		// le nombre maximal de nœuds dans la liste

// type structuré qui représente un « nœud » de la liste (vous pouvez y ajouter d’autres champs!)
typedef struct {
    t_plaque    plaque;				// la plaque de jeu
    int         posx_0, posy_0;     // la position de la case vide
    int         cout;				// le « coût » de ce nœud, Cout(N) = P(N) + E(N)
    int         profondeur;		    // la profondeur (ou niveau) dans l’arbre de jeu
    int         parent;				// position du nœud-parent de ce noeud
    int         terminal;			// soit 0 (non-Terminal) ou 1 (Terminal)
    t_direction dir;				// la direction qui à été utilisée pour générer cette plaque
} t_noeud;


typedef t_noeud t_liste_noeuds[MAXNOEUDS];	// type-tableau pour la liste des noeuds
typedef int  t_tab_ptr_niveaux[MAXNOEUDS];	// type-tableau pour les niveaux



/*===========================================================================*/
/*
   Fonction    : initialiser_arbre()
   Description : Fonction qui reçoit la plaque de départ
                 (avec la position[py_0, px_0] de la case vide), et qui initialise
                 la structure de donnée pour la recherche. Il s’agit de copier
                 la plaque de départ dans le premier nœud de la liste, calculer
                 son coût et initialiser les autres champs de ce premier nœud.
                 La liste des pointeurs de niveaux est également initialisée
                 (premier niveau = 0, les autres à - 1).

   Paramètres  : - plaque_depart    : Plaque de départ
                 - py_0             : Position en y du 0
                 - px_0             : Position en x du 0

   Retour      : Aucun
*/
/*===========================================================================*/
void initialiser_arbre(const t_plaque plaque_depart, int py_0, int px_0,
    t_liste_noeuds liste_noeuds,
    t_tab_ptr_niveaux  niveaux);



/*===========================================================================*/
/*
   Fonction    : trouver_solution()
   Description : Fonction qui effectue une « passe » de la recherche jusqu'à
                 une profondeur de « max_cout ».  Elle reçoit la liste des nœuds
                 et des pointeurs de niveaux (déjà initialisées) ainsi que la
                 valeur de max_cout.  Si l’algorithme trouve la solution,
                 la fonction renvoie 1 ET renvoie la position du nœud-solution
                 dans la liste via le paramètre « *posfin ». Sinon, elle renvoie
                 simplement 0.

   Paramètres  : - liste_noeuds : Liste de type l_liste_noeuds qui contient
                                  les noeuds
                 - niveaux      : Liste de type t_tab_ptr_niveaux qui contient
                                  les pointeurs de niveaux
                 - max_cout     : Cout maximum qui est égale à la profondeur
                                  maximale de recherche
                 - posfin       : Position du noeuds-solution qui sera édité
                                  par référence

   Retour      : 1 si la solution est trouvée (En renvoie la position du noeuds-solution via *posfin)
*/
/*===========================================================================*/
int trouver_solution(t_liste_noeuds liste_noeuds,
    t_tab_ptr_niveaux  niveaux,
    int max_cout,
    int* posfin);


/*===========================================================================*/
/*
   Fonction    : afficher_solution()
   Description : Une fois la solution trouvée, cette fonction va afficher tous
                 les déplacements à faire de la plaque - départ jusqu'à la
                 plaque-solution pour résoudre le casse-tête (eg. afficher
                 la liste des valeurs « .dir » des parents du nœud-solution
                 dans l’ordre inverse). NOTE : vous devez donc absolument
                 utiliser une structure-pile pour retenir la liste des directions
                 à afficher.

   Paramètres  : - liste_noeuds : Liste de type l_liste_noeuds qui contient
                                  les noeuds
                 - posfin       : Position du noeuds-solution

   Retour      : 1 si la solution est trouvée (En renvoie la position du noeuds-solution via *posfin)
*/
/*===========================================================================*/
void afficher_solution(const t_liste_noeuds liste_noeuds, int posfin);