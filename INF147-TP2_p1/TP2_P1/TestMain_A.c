#include "plaque.h"
#include <conio.h>

// Définition des codes des touches direction et de la touche ESC
typedef enum
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_RIGHT = 77,
	KEY_LEFT = 75,
	KEY_ESC = 27
} t_codes_touches;

int main(void)
{
	int py0; // Position verticale de la case vide
	int px0; // Position horizontale de la case vide

	int posy; // Position verticale trouvée par trouver_numero()
	int posx; // Position horizontale trouvée par trouver_numero()

	t_codes_touches touche; // Variable pour stocker la touche pressée

	t_plaque plaque_test; // Déclaration de la plaque de jeu

	int cout = 0;	// Coût de l'état actuel de la plaque
	int changement; // Indicateur de changement après un déplacement

	srand(time(NULL));


	// Copie de la solution dans plaque_test comme plaque de depart
	memcpy(plaque_test, SOLUTION, sizeof(t_plaque));
	py0 = DIM - 1;
	px0 = 

	printf("Plaque solution\r\n");
	afficher_plaque(plaque_test);

	// Mélange de la plaque en effectuant des déplacements aléatoires
	melanger_plaque(plaque_test, &py0, &px0);

	printf("Plaque melangee\r\n");
	afficher_plaque(plaque_test);
	printf("Cout: %d\r\n", evaluer_plaque(plaque_test));

	// Boucle infinie pour la gestion des déplacements de l'utilisateur
	while (1)
	{
		touche = 0; // Réinitialisation de la touche pressée
		if (_kbhit())
		{					   // Vérifie si une touche a été pressée
			touche = _getch(); // Lecture de la touche pressée
			if (touche == 0 || touche == 224)
			{					   // Vérifie si la touche est un ASCII etendu
				touche = _getch(); // Lecture du deuxième code de la touche
				switch (touche)
				{
				case KEY_LEFT: // Déplacement vers la gauche
					changement = changer_plaque(plaque_test, GAUCHE, &py0, &px0);
					break;
				case KEY_RIGHT: // Déplacement vers la droite
					changement = changer_plaque(plaque_test, DROITE, &py0, &px0);
					break;
				case KEY_UP: // Déplacement vers le haut
					changement = changer_plaque(plaque_test, HAUT, &py0, &px0);
					break;
				case KEY_DOWN: // Déplacement vers le bas
					changement = changer_plaque(plaque_test, BAS, &py0, &px0);
					break;
				default:
					changement = 0; // Aucun déplacement effectué
					break;
				}

				// Si un déplacement a été effectué avec succès
				if (changement)
				{
					system("cls");											 // Efface l'écran
					afficher_plaque(plaque_test);							 // Affiche la plaque mise à jour
					printf("Cout: %d\r\n", evaluer_plaque(plaque_test));	 // Affiche le coût actuel
					printf("CASE VIDE est a la position [%d,%d]", py0, px0); // Indique la nouvelle position de la case vide
				}
				else
				{
					printf("\r\nERREUR DIRECTION"); // Affichage d'une erreur si le déplacement est invalide
				}
			}
			else
			{
				// Vérifie si l'utilisateur a appuyé sur Échap pour quitter
				if (touche == KEY_ESC)
					break; // Quitte la boucle
			}
		}
	}

	// Vérification de la position de la case vide avec trouver_numero()
	trouver_numero(plaque_test, 0, &posy, &posx);
	printf("\r\nEt Selon trouver_numero() a [%d,%d]", posy, posx);

	system("pause");
	return 1; // Fin du programme
}