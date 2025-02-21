#pragma once

#include "plaque.h"
#define MAXNOEUDS  1000     		// le nombre maximal de n�uds dans la liste

// type structur� qui repr�sente un � n�ud � de la liste (vous pouvez y ajouter d�autres champs!)
typedef struct {
    t_plaque    plaque;				// la plaque de jeu
    int         posx_0, posy_0;     // la position de la case vide
    int         cout;				// le � co�t � de ce n�ud, Cout(N) = P(N) + E(N)
    int         profondeur;		    // la profondeur (ou niveau) dans l�arbre de jeu
    int         parent;				// position du n�ud-parent de ce noeud
    int         terminal;			// soit 0 (non-Terminal) ou 1 (Terminal)
    t_direction dir;				// la direction qui � �t� utilis�e pour g�n�rer cette plaque
} t_noeud;


typedef t_noeud t_liste_noeuds[MAXNOEUDS];	// type-tableau pour la liste des noeuds
typedef int  t_tab_ptr_niveaux[MAXNOEUDS];	// type-tableau pour les niveaux



/*===========================================================================*/
/*
   Fonction    : initialiser_arbre()
   Description : Fonction qui re�oit la plaque de d�part
                 (avec la position[py_0, px_0] de la case vide), et qui initialise
                 la structure de donn�e pour la recherche. Il s�agit de copier
                 la plaque de d�part dans le premier n�ud de la liste, calculer
                 son co�t et initialiser les autres champs de ce premier n�ud.
                 La liste des pointeurs de niveaux est �galement initialis�e
                 (premier niveau = 0, les autres � - 1).

   Param�tres  : - plaque_depart    : Plaque de d�part
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
   Description : Fonction qui effectue une � passe � de la recherche jusqu'�
                 une profondeur de � max_cout �.  Elle re�oit la liste des n�uds
                 et des pointeurs de niveaux (d�j� initialis�es) ainsi que la
                 valeur de max_cout.  Si l�algorithme trouve la solution,
                 la fonction renvoie 1 ET renvoie la position du n�ud-solution
                 dans la liste via le param�tre � *posfin �. Sinon, elle renvoie
                 simplement 0.

   Param�tres  : - liste_noeuds : Liste de type l_liste_noeuds qui contient
                                  les noeuds
                 - niveaux      : Liste de type t_tab_ptr_niveaux qui contient
                                  les pointeurs de niveaux
                 - max_cout     : Cout maximum qui est �gale � la profondeur
                                  maximale de recherche
                 - posfin       : Position du noeuds-solution qui sera �dit�
                                  par r�f�rence

   Retour      : 1 si la solution est trouv�e (En renvoie la position du noeuds-solution via *posfin)
*/
/*===========================================================================*/
int trouver_solution(t_liste_noeuds liste_noeuds,
    t_tab_ptr_niveaux  niveaux,
    int max_cout,
    int* posfin);


/*===========================================================================*/
/*
   Fonction    : afficher_solution()
   Description : Une fois la solution trouv�e, cette fonction va afficher tous
                 les d�placements � faire de la plaque - d�part jusqu'� la
                 plaque-solution pour r�soudre le casse-t�te (eg. afficher
                 la liste des valeurs � .dir � des parents du n�ud-solution
                 dans l�ordre inverse). NOTE : vous devez donc absolument
                 utiliser une structure-pile pour retenir la liste des directions
                 � afficher.

   Param�tres  : - liste_noeuds : Liste de type l_liste_noeuds qui contient
                                  les noeuds
                 - posfin       : Position du noeuds-solution

   Retour      : 1 si la solution est trouv�e (En renvoie la position du noeuds-solution via *posfin)
*/
/*===========================================================================*/
void afficher_solution(const t_liste_noeuds liste_noeuds, int posfin);