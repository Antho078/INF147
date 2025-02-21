#include "plaque.h"

/*===========================================================================*/
/* Déclaration des fonctions privées */
/*===========================================================================*/

/*===========================================================================*/
/*
   Fonction    : print_tableau_limite()
   Description : Affiche une ligne de séparation pour encadrer la plaque.
				 Cette fonction est utilisée avant et après l'affichage
				 du contenu de la plaque pour améliorer la lisibilité.

   Paramètres  : - dim : La dimension de la plaque (nb colonnes = nb lignes)

   Retour      : Aucun (affichage seulement)
*/
/*===========================================================================*/
static void print_tableau_limite(int dim);

/*===========================================================================*/
/* Définition des fonctions publiques */
/*===========================================================================*/
void trouver_numero(const t_plaque pla, int num, int* posy, int* posx)
{
	// Boucle qui parcourt la plaque à l'horizontale (i), puis à la vertical (j)
	for (int indice_horiz = 0; indice_horiz < DIM; indice_horiz++)
	{
		for (int indice_vert = 0; indice_vert < DIM; indice_vert++)
		{
			// Si la case en cours d'analyse contient la valeur recherchée, on renvoie la position en paramètre
			if (pla[indice_vert][indice_horiz] == num)
			{
				(*posx) = indice_horiz;
				(*posy) = indice_vert;
				return;
			}
		}
	}
}

/*===========================================================================*/
int changer_plaque(t_plaque pla, t_direction dir, int* py_0, int* px_0)
{

	// On analyse les cas qu'il est possible de recevoir
	switch (dir)
	{
	case HAUT:
		/*On s'assure de ne pas être à la position limite supérieure en Y
		x | x | x
		o | o | o
		o | o | o */
		if (*py_0)
		{
			// On mets la valeur visée par le zéro à la place du 0
			pla[(*py_0)][(*px_0)] = pla[(*py_0) - 1][(*px_0)];

			// On mets le zéro à la place visée par le zéro
			pla[(*py_0) - 1][(*px_0)] = VIDE;

			// On mets à jour la position du 0
			(*py_0)--;
			return 1;
		}
		break;
	case GAUCHE:
		/*On s'assure de ne pas être à la position limite gauche en X
		x | o | o
		x | o | o
		x | o | o */
		if (*px_0)
		{
			// On mets la valeur visée par le zéro à la place du 0
			pla[(*py_0)][(*px_0)] = pla[(*py_0)][(*px_0) - 1];

			// On mets le zéro à la place visée par le zéro
			pla[(*py_0)][(*px_0) - 1] = VIDE;

			// On mets à jour la position du 0
			(*px_0)--;
			return 1;
		}
		break;
	case BAS:
		/*On s'assure de ne pas être à la position limite inférieure en Y
		o | o | o
		o | o | o
		x | x | x */
		if (*py_0 < (DIM - 1))
		{
			// On mets la valeur visée par le zéro à la place du 0
			pla[(*py_0)][(*px_0)] = pla[(*py_0) + 1][(*px_0)];

			// On mets le zéro à la place visée par le zéro
			pla[(*py_0) + 1][(*px_0)] = VIDE;

			// On mets à jour la position du 0
			(*py_0)++;
			return 1;
		}
		break;
	case DROITE:
		/*On s'assure de ne pas être à la position limite droite en X
		o | o | x
		o | o | x
		o | o | x  */
		if (*px_0 < (DIM - 1))
		{
			// On mets la valeur visée par le zéro à la place du 0
			pla[(*py_0)][(*px_0)] = pla[(*py_0)][(*px_0) + 1];

			// On mets le zéro à la place visée par le zéro
			pla[(*py_0)][(*px_0) + 1] = VIDE;

			// On mets à jour la position du 0
			(*px_0)++;
			return 1;
		}
		break;
	}
	return 0;
}

/*===========================================================================*/
void melanger_plaque(t_plaque pla, int* py_0, int* px_0)
{
	int deplacement_aleatoire = 0;
	// Boucle qui parcourt la plaque à l'horizontale (i), puis à la vertical (j)
	// pour mettre la solution dans le tableau pla
	for (int indice_horiz = 0; indice_horiz < DIM; indice_horiz++)
	{
		for (int indice_vert = 0; indice_vert < DIM; indice_vert++)
		{
			// On mets toutes les valeurs de la solution dans pla
			pla[indice_vert][indice_horiz] = SOLUTION[indice_vert][indice_horiz];
		}
	}
	//	trouver_numero(pla, VIDE, py_0, px_0);
	for (int i = 0; i < NBCOUPS; i++)
	{
		deplacement_aleatoire = rand() % 4;
		changer_plaque(pla, deplacement_aleatoire, py_0, px_0);
	}
}

/*===========================================================================*/
int evaluer_plaque(const t_plaque pla)
{
	int pos_x_attendue;
	int pos_y_attendue;
	int pos_x_actuelle;
	int pos_y_actuelle;
	int cout_total = 0;
	// Boucle qui évalue le coût total en fonction de la distance de chaque numéro par rapport à sa position correcte
	for (int rangee = 1; rangee < DIM; rangee++)
	{
		for (int colonne = 1; colonne < DIM; colonne++) {
			pos_x_attendue = colonne; // Position attendue du numero en x
			pos_y_attendue = rangee;  // Position attendue du numero en y

			// Positions actuelles du numero en x,y
			if (SOLUTION[rangee][colonne]) {
				trouver_numero(pla, SOLUTION[rangee][colonne], &pos_y_actuelle, &pos_x_actuelle);
				// Calcul du coût basé sur la distance de Manhattan
				cout_total += abs(pos_x_actuelle - pos_x_attendue) + abs(pos_y_actuelle - pos_y_attendue);
			}


		}
	}

	return cout_total;
}

/*===========================================================================*/
void afficher_plaque(const t_plaque pla)
{
	const int buff = 10;
	char num_str[10];
	int num;

	// Affichage des bordures supérieures du tableau
	print_tableau_limite(DIM);

	// Parcours et affichage des valeurs de la plaque
	for (int rang = 0; rang < DIM; rang++)
	{
		printf("|");
		for (int col = 0; col < DIM; col++)
		{
			num = pla[rang][col];
			if (num)
			{
				// Conversion du numéro en string avec formatage
				sprintf_s(num_str, buff, "%2d |", num);
			}
			else
			{
				// Affichage d'un espace vide pour la case vide
				strcpy_s(num_str, buff, "   |");
			}

			printf("%s", num_str);
		}
		printf("\r\n");
	}

	// Affichage des bordures inférieures du tableau
	print_tableau_limite(DIM);
}

/*===========================================================================*/
/* Définition des fonctions privees */
/*===========================================================================*/
static void print_tableau_limite(int dim)
{
	// Affichage de la bordure limitrophe du tableau
	printf("*");
	for (int col = 0; col < 4 * dim - 1; col++)
	{
		printf("-");
	}
	printf("*\n");
}
