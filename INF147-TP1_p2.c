/*===========================================================================*/
/*  INF147 - Travail pratique (TP) #1 Partie 1. */
/*===========================================================================*/

/*
 Module		: TP1_p1.c
 Par		: Anthony Diotte
 Cr�� le    : 24 janvier 2025

Description :
Ce fichier contient la fonction main(), ainsi que lesfonctions n�cessaires
au fonctionnement d'une machine distributrice. On y retrouve la gestion des
paiements, de l'inventaire et des prix des articles, ainsi que la prise de
commandes utilisateur. Certaines fonctions g�rent �galement l'affichage des
informations destin�es � l'utilisateur.

*/

/*===========================================================================*/
// Permet de d�sactiver certains warnings du compilateur
#define _CRT_SECURE_NO_WARNINGS

// Librairies usuelles � inclure
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define TEST_MODE 1 // 1 = test mode on, 0 = test mode off

// Macro pour vider le buffer du terminal apr�s l'utilisation de `scanf`.
#define FFLUSH()              \
	while (getchar() != '\n') \
	{                         \
	}

/* les dimensions de la machine-distributrice avec son nombre total d'items. */
#define NB_RANGEES 10  // le nombre de rang�es  (doit �tre entre 2 et 6 max.)
#define NB_COLONNES 10 // le nombre de colonnes (doit �tre entre 2 et 7 max.)
#define NB_ITEMS (NB_RANGEES * NB_COLONNES)

// Macro convertissant le code utilisateur (lettre-chiffre) en index d'item.
#define NO_ITEM(lettre, chiffre) ((lettre - 'A') * NB_COLONNES + (chiffre - '0'))

// Macro permettant d'obtenir la lettre du code position (ex AX) en fonction de l'index lineaire (0 a NB_ITEMS - 1)
#define POSITION_A_LETTRE(position) ('A' + position / NB_COLONNES)

// Macro permettant d'obtenir le chiffre du code position (ex X1) en fonction de l'index lineaire (0 a NB_ITEMS - 1)
#define POSITION_A_CHIFFRE(position) ('0' + position % NB_COLONNES)

/* Les 5 codes-capteur pour la gestion du capteur */
#define CODE_QUITTER 0
#define CODE_BOUTON_COMMANDE 1
#define CODE_CAPTEUR_PIECE_25C 10
#define CODE_CAPTEUR_PIECE_1D 11
#define CODE_CAPTEUR_PIECE_2D 12
#define CODE_CAPTEUR_BILLET_5D 13
#define CHOIX_MAX 5 // nb. maximal du choix-menu

/* Constante-tableau des valeurs des 3 pi�ces de monnaie */
#define NB_PIECES 3
const double VAL_PIECES[NB_PIECES] = { 0.25, 1.00, 2.00 };
#define BILLET_5D 5.00 // billets de 5 dollars

// Les quantit�s de pi�ces de monnaie en inventaire (r�duire ces valeurs pour vos tests)
#define NB_PIECES_25C    5
#define NB_PIECES_1D     5
#define NB_PIECES_2D     5

/* Le tableau des pi�ces de monnaie disponibles (initialisation � la d�claration) */
int tab_monnaie[NB_PIECES] = { NB_PIECES_25C, NB_PIECES_1D, NB_PIECES_2D };

/* Constante-tableau des 5 prix possibles pour un item */
#define NB_PRIX 5
const double LISTE_PRIX[NB_PRIX] = { 2.00, 2.25, 2.50, 3.00, 3.50 };

#define INVENTAIRE_MAX 15 // quantit� maximale d'un item en inventaire

/*===========================================================================*/
/*Prototype des fonctions*/
/*===========================================================================*/

/*===========================================================================*/
/*
fonction :		valider_commande
Description :	Demande � l'utilisateur de choisir une action ou un capteur.
				Redemande une entr�e valide si n�cessaire..
				Les choix sont de 0 � 5
				1 : Choisir un item
				2,3,4,5 : Capteur de pi�ce de 0.25$, 1$, 2$, 5$
				0 : Quitter le programme

Param�tres :	Aucun
Retour :		Le choix du capteur (0 � 5)
*/
/*===========================================================================*/
unsigned int valider_commande(void);

/*===========================================================================*/
/*
fonction :		donner_code()
Description :	Cette fonction convertit le code utilisateur de 0 � 5 en code
				op�ration qui est soit 0, 1, 10, 11, 12 ou 13

Param�tres :	commande (Commande re�ue par l'utilisateur)
Retour :		Le code correspondant � un capteur (0,1,10,11,12,13)
*/
/*===========================================================================*/
unsigned int donner_code(unsigned int commande);

/*===========================================================================*/
/*
fonction :		demander_item
Description :	Fonction qui demande � l'utilisateur quel item il d�sire. La
				fonction s'assure que le code existe dans le sc�nario �tudi�
				et redemande le code tant qu'il n'est pas bon.


Param�tres :	nb_items (Le nombre total d'items)
Retour :		L'index de l'item en question entre 0 et NB_ITEMS-1
*/
/*===========================================================================*/
unsigned int demander_item(unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		init_prix()
Description :	Fonction qui initialise al�atoirement le prix de tous les items
				dans la machine distributrice. Les valeurs sont mises dans un
				tableau qu'on passe en param�tres. Les valeurs de prix possible
				sont 2.00$, 2.25$, 2.50$, 3.00$ ou 3.50$.


Param�tres :	tab_prix (Un tableau de r�el initialement vide)
				nb_items (Le nombre d'item contenu dans le tableau de prix)
Retour :		Aucun (On remplit le tableau en param�tre)
*/
/*===========================================================================*/
void init_prix(double tab_prix[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		init_inventaire
Description :	Fonction qui remplit le tableau d'inventaire al�atoirement avec
				des valeurs variant de 0 � INVENTAIRE_MAX

Param�tres :	tab_inventaire (Tableau d'inventaire initialement vide)
				nb_items (Nombre d'items diff�rents dans l'inventaire)
Retour :		Aucun (On remplit le tableau d'inventaire)
*/
/*===========================================================================*/
void init_inventaire(int tab_inventaire[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		afficher_inventaire
Description :	Fonction qui affiche l'inventaire des items. On affiche d'abord
				les codes des items, puis les quantit�s de chaque items.


Param�tres :	tab_inventaire (Tableau qui contient l'inventaire)
Retour :		nb_items (Le nombre d'items diff�rents dans l'inventaire)
*/
/*===========================================================================*/
void afficher_inventaire(int tab_inventaire[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		recolter_argent()
Description :	Fonction permettant de r�colter l�argent n�cessaire selon le 
				prix d'item re�u. La fonction boucle en faisant la saisie de 
				codes-capteurs pour voir si une nouvelle pi�ce ou billet a �t� 
				ins�r�e. Selon la pi�ce de monnaie (ou un billet de 5$) re�ue, 
				on ajoute le montant � un total accumul�, jusqu'� ce que ce 
				total soit �gal ou sup�rieur au prix re�u.  Si l'utilisateur 
				s�lectionne "1. Choisir un item", le syst�me annule la commande 
				en cours et on quitte la boucle de saisie.  On retourne le total
				accumul�.

Param�tres :	prix:			Prix de l'item (r�el). Un des 5 prix valides 
								(voir les constantes)
				tab_monnaie:	Tableau d�entiers indiquant le nombre de pi�ces 
								de monnaie disponibles.

Retour :		Le montant total re�u (r�el), si la commande est annul�e 
				ce montant sera < prix.

*/
/*===========================================================================*/
double recolter_argent(double prix_item);


/*===========================================================================*/
/*
fonction :		Fonction permettant de redonner la monnaie n�cessaire pour 
				combler un � montant � re�u. On utilise ici l�algorithme 
				classique de distribution de monnaie.



Param�tres :	montant:		Le montant de monnaie � rembourser (r�el).
				tab_monnaie:	Tableau d�entiers indiquant le nombre de pi�ces 
								de monnaie disponibles.

Retour :		1 si la monnaie a �t� redonn�e, 0 sinon (pas assez de pi�ces dans la machine).
*/
/*===========================================================================*/
int redonner_monnaie(double montant_a_retourner, int tab_monnaie[]);


/*===========================================================================*/
/*
fonction :		gerer_monnaie()
Description :	Fonction g�rant la logique pour la gestion de l�argent et la 
				monnaie. Re�oit une position-item et un prix. On imprime ces 
				deux valeurs � l��cran. On appelle � recolter_argent() � et si 
				cette fonction retourne un montant insuffisant, on consid�re 
				que l�achat a �t� annul�e et on rembourse l'utilisateur (en 
				affichant un message d�annulation). Si le montant retourn� 
				par � recolter_argent() � est suffisant, on consid�re l�achat 
				comme complet pour l�instant.  Si le montant retourn� est plus 
				grand que le prix de l�item on tente de redonner la monnaie de 
				cette diff�rence.  Si la fonction de distribution de monnaie a 
				�chou� (manque de monnaie), on consid�re l�achat comme annul� 
				et on redonne le montant du prix au complet en monnaie � 
				l�utilisateur (en affichant un message de remboursement et un 
				message demandant de payer avec la monnaie exacte). Si l�achat 
				est complet la fonction retourne 1 et si l�achat a �t� annul�, 0.

Param�tres :	item:			Position-tableau de l'item (entre 0 et NB_ITEMS-1)
				prix:			Prix de l'item � payer (un des 5 prix valides, 
								voir les constantes)
				tab_monnaie:	Tableau d�entiers indiquant le nombre de pi�ces 
								de monnaie disponibles.

Retour: 1 si l�achat s'est effectu� avec succ�s, 0 sinon.
*/
/*===========================================================================*/
int gerer_monnaie(int item, double prix, int tab_monnaie);


/*===========================================================================*/
/*
fonction :		gerer_commande()
Description :	


Param�tres :	

Retour: rien
*/
/*===========================================================================*/
void gerer_commande(int tab_inventaire[], double tab_prix[], int nb_items, int tab_monnaie[]);


/*===========================================================================*/
/*
fonction :		afficher_machine()
Description :	Fonction permettant d'afficher l'inventaire et les prix des items 
				sous forme visuelle. Affiche un tableau de � nb_ran � par 
				� nb_col � et montre les informations suivantes : No. item 
				(de "A0" � "F5" pour un mod�le 6x6), la quantit� disponible 
				ainsi que le prix. NOTE : Si la quantit� disponible d�un item 
				est 0, on affiche "vide". S�il y a un seul item de disponible 
				on affiche � 1 item �, mais il y en a plusieurs c�est � X items �


Param�tres :	tab_inventaire: Tableau d�entiers des quantit�s des items.
				tab_prix:       Tableau de r�els des prix des items.
				nb_ran:         Le nombre de rang�es de la machine (entier).
				nb_col:         Le nombre de colonnes de la machine (entier).


Retour: rien
*/
/*===========================================================================*/
void afficher_machine(int tab_inventaire[], double tab_prix[], unsigned int nb_ran, unsigned int nb_col);



/*===========================================================================*/


/*===========================================================================*/
/*
fonction :		afficher_machine
Description :	Fonction permettant d'afficher l'inventaire et les prix des items sous forme visuelle.
				Affiche un tableau de « nb_ran » par « nb_col » et montre les informations suivantes :
				No. item (de "A0" à "F5" pour un modèle 6x6), la quantité disponible ainsi que le prix.
				NOTE : Si la quantité disponible d’un item est 0, on affiche "vide". S’il y a un seul
				item de disponible on affiche « 1 item », mais il y en a plusieurs c’est « X items »

Important: on assume que la taille des deux tableaux reçus est de (nb_ran * nb_col).

Param�tres :	- tab_inventaire: Tableau d’entiers des quantités des items.
				- tab_prix: Tableau de réels des prix des items.
				- nb_ran: Le nombre de rangées de la machine (entier).
				- nb_col: Le nombre de colonnes de la machine (entier).

Retour :		Rien (void)
*/
/*===========================================================================*/
void afficher_machine(int tab_inventaire[], int tab_prix[], unsigned int nb_ran, unsigned int nb_col);

int gerer_monnaie(int item, double prix, int tab_monnaie[]);
/*===========================================================================*/

/*===========================================================================*/
/*Definition du main
/*===========================================================================*/
#ifndef TEST_MODE

void main(void)
{
}
#endif

/*===========================================================================*/
/*Definition des fonctions privees*/
/*===========================================================================*/

// On demande d'entrer une valeur tant que la valeur est invalide
unsigned int valider_commande(void)
{
	int commande = CHOIX_MAX + 1; // On initialise commande � une valeur invalide
	// Tant que commande ne prends pas une valeur valide, on redemande l'entr�e
	while (commande < 0 || commande > CHOIX_MAX)
	{
		printf("Quel capteur voulez-vous simuler?\r\n");
		printf("1- Choisir un item\r\n");
		printf("2- 0.25$\r\n");
		printf("3- 1.00$\r\n");
		printf("4- 2.00$\r\n");
		printf("5- 5.00$\r\n");
		printf("0- Quitter\r\n");
		printf(">> ");
		scanf("%i", &commande);
		// printf("\r\n");
		FFLUSH();
	}
	// Lorsque commande prends une valeur valide, on sort de la boucle while()
	// et on retourne cette valeur
	return commande;
};

// On associe chaque commande avec un code
unsigned int donner_code(unsigned int commande)
{
	// On associe chaque valeurs de commande � un code qu'on retourne
	switch (commande)
	{
	case 1:
		return CODE_BOUTON_COMMANDE;

	case 2:
		return CODE_CAPTEUR_PIECE_25C;

	case 3:
		return CODE_CAPTEUR_PIECE_1D;

	case 4:
		return CODE_CAPTEUR_PIECE_2D;

	case 5:
		return CODE_CAPTEUR_BILLET_5D;

	case 0:
		return CODE_QUITTER;
		// Si un probl�me se produit et que commande prend une valeur invalide, on
		// on retourne une autre valeur invalide
	default:
		return -1;
	}
};

// On demande quel item l'utilisateur d�sire tant qu'il est invalide
unsigned int demander_item(unsigned int nb_items)
{
	char lettre = '0'; // on initialise � une valeur invalide
	char chiffre = '0';
	// Avec le nombre de colonnes et d'items, on trouve le nombre de chiffres possibles
	char nb_chiffres = (nb_items - 1) % NB_COLONNES;
	// Avec le nombre de colonnes et d'items, on trouve le nombre de lettres possibles
	char nb_lettres = (nb_items - 1) / NB_COLONNES;

	// Tant que le code entr� est invalide, on le redemande
	while ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
		(chiffre < '0') || chiffre > '0' + nb_chiffres)
	{
		printf("Entrez le code de l'item:");
		scanf("%c%c", &lettre, &chiffre);
		printf("\r\n");
		lettre = toupper(lettre);
		FFLUSH();

		// Si la valeur entr�e est invalide, on l'indique
		if ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
			(chiffre < '0') || chiffre > '0' + nb_chiffres)
		{
			printf("Item invalide \r\n");
		}
	}
	// On retourne l'index de l'item voulu � partir de son code lettre-chiffre
	return NO_ITEM(lettre, chiffre);
};

// On parcourt le tableau de prix et on y mets des valeurs de prix al�atoirement
void init_prix(double tab_prix[], unsigned int nb_items)
{
	unsigned int indice_prix = 0; // Indice pour le tableau LISTE_PRIX
	// on parcourt le tableau de prix pour le remplir
	for (int i = 0; i < nb_items; i++)
	{
		// On trouve un entier al�atoire qu'on ram�ne de 0 � NB_PRIX
		indice_prix = rand() % NB_PRIX;
		tab_prix[i] = LISTE_PRIX[indice_prix];
	}
};

// On parcourt le tableau d'inventaire et on y mets des valeurs d'inventaire al�atoirement
void init_inventaire(int tab_inventaire[], unsigned int nb_items)
{
	unsigned int nb_inventaire = 0; // Quantit� en inventaire pour l'item courrant
	// On parcourt le tableau d'inventaire pour le remplir
	for (int i = 0; i < nb_items; i++)
	{
		// On trouve un entier al�atoire qu'on ram�ne de 0 � INVENTAIRE_MAX
		nb_inventaire = rand() % (INVENTAIRE_MAX + 1);
		// On attribue une valeur de 0 � INVENTAIRE_MAX � un item dans
		//  l'inventaire
		tab_inventaire[i] = nb_inventaire;
	}
};

// On affiche simplement l'inventaire
void afficher_inventaire(int tab_inventaire[], unsigned int nb_items)
{
	char lettre;
	int chiffre;
	printf("ITEM : ");
	// On affiche tous les items
	for (int i = 0; i < nb_items; i++)
	{
		lettre = 'A' + (i / NB_COLONNES);
		chiffre = i % NB_COLONNES;
		printf("%c%d ", lettre, chiffre);
	}
	printf("\r\n");

	// On affiche l'inventaire de chaque items
	printf("INV. : ");
	for (int i = 0; i < nb_items; i++)
	{
		printf("%i  ", tab_inventaire[i]);
	}
	printf("\n");
};

double recolter_argent(double prix_item) {
	double cumul_argent = 0;  // Variable accumulant l'argent ins�r�
	int commande = 0;         // Variable stockant le code de la commande utilisateur
	double piece = 0;         // Montant correspondant � la pi�ce ins�r�e

	// Boucle jusqu'� ce que l'utilisateur valide la commande ou que l'argent ins�r� soit suffisant
	while ((commande != CODE_BOUTON_COMMANDE) && (cumul_argent < prix_item)) {
		commande = donner_code(valider_commande());  // Obtient le code de la commande ou de la pi�ce ins�r�e

		// V�rification du type de pi�ce/billet ins�r�
		switch (commande) {
		case CODE_CAPTEUR_PIECE_25C:
		case CODE_CAPTEUR_PIECE_1D:
		case CODE_CAPTEUR_PIECE_2D:
			// Calcule la valeur de la pi�ce en fonction du code re�u
			piece = VAL_PIECES[commande - CODE_CAPTEUR_PIECE_25C];
			break;
		case CODE_CAPTEUR_BILLET_5D:
			// D�tecte un billet de 5$
			piece = BILLET_5D;
			break;
		}

		cumul_argent += piece;  // Ajoute la valeur de la pi�ce au montant total accumul�
	}

	return cumul_argent;  // Retourne le montant total collect�
}

int redonner_monnaie(double montant_a_retourner, int tab_monnaie[]) {
	int tab_nb_pieces[NB_PIECES] = { 0 };  // Tableau pour stocker le nombre de pi�ces � rendre
	int pos_piece = NB_PIECES - 1;       // Position actuelle dans le tableau de pi�ces (part du plus grand)
	double montant = montant_a_retourner; // Copie du montant � rendre pour modification
	int nb_pieces = 0;                   // Nombre de pi�ces utilis�es pour une d�nomination donn�e


	// Boucle pour distribuer la monnaie en utilisant les plus grandes pi�ces disponibles
    while (pos_piece >= 0 && montant > 0) {
        nb_pieces = montant / VAL_PIECES[pos_piece];  // Calcule le nombre de pi�ces n�cessaires

        // V�rifie si on a assez de pi�ces disponibles, sinon prend le maximum possible
        nb_pieces = (nb_pieces > tab_monnaie[pos_piece]) ? tab_monnaie[pos_piece] : nb_pieces;

        if (nb_pieces > 0) {
            tab_nb_pieces[pos_piece] = nb_pieces;  // Stocke le nombre de pi�ces utilis�es
            tab_monnaie[pos_piece] -= nb_pieces;  // Met � jour le stock de pi�ces
            montant -= nb_pieces * VAL_PIECES[pos_piece];  // R�duit le montant restant � rendre
        }
        pos_piece--;  // Passe � la plus petite d�nomination suivante
    }
	// V�rifie si le montant � rendre a �t� totalement couvert
	if (montant > 0) {
		// Si on ne peut pas rendre le montant exact, on remet les pi�ces utilis�es
		for (int i = 0; i <= NB_PIECES; i++) {
			tab_monnaie[i] += tab_nb_pieces[i];  // Restaure le stock initial
			tab_nb_pieces[i] = 0;  // R�initialise le tableau des pi�ces utilis�es
		}
		return 0; // �chec : pas assez de monnaie disponible
	}
	else {
		// Affichage du nombre de pi�ces utilis�es pour rendre la monnaie
		for (int i = (NB_PIECES - 1); i >= 0; i--) {
			printf("nb de %0.2f : %d\r\n", VAL_PIECES[i], tab_nb_pieces[i]);
		}
		return 1; // Succ�s : monnaie redonn�e correctement
	}
}

int gerer_monnaie(int item, double prix, int tab_monnaie[]) {
	double montant_recu = recolter_argent(prix);
	double retour = 0;
	//Impression de l'ent�te
	printf("-------------------------\n");
	printf("Vous voulez l'item : %i\n", item);
	printf("Veuillez ins�rer: %d$ SVP\n", prix);
	printf("-------------------------\n");

	//Gestion de l'annulation de la commande s'il n'y a pas assez d'argent ou si
	//on annule volontairement la transaction
	if (montant_recu < prix) {
		printf("Total: %0.2f$ / %0.2f$\r\n", montant_recu, prix);
		printf("Commande annulée\r\n");
		retour = montant_recu;
		printf("Re-voici votre argent :\r\n");
		redonner_monnaie(retour, tab_monnaie);
		return 0;
	}
	//Gestion du retour de l'argent apr�s une transaction r�ussie
	else {
		retour = montant_recu - prix;
		printf("Re-voici votre argent :\r\n");
		//Si l'achat r�ussi, mais qu'on ne peut pas redonner la monnaie, on annule 
		//la transaction
		if (!redonner_monnaie(retour, tab_monnaie)) {
			printf("Impossible de rendre la monnaie exacte. Commande annul�e.\r\n");
			printf("Re-voici votre argent :\r\n");
			redonner_monnaie(montant_recu, tab_monnaie);
			return 0;
		}
		//Si l'achat réussi et qu'on peut redonner la monnaie, on finalise
		//la transaction
		else {
			printf("Achat complet.\r\n");
			return 1;
		}
	}
}

void gerer_commande(int tab_inventaire[], double tab_prix[], int nb_items, int tab_monnaie[]) {

	int position_item = demander_item(nb_items);

	if (tab_inventaire[position_item] <= 0) {
		printf("L�item [%c%c] n�est plus disponible", POSITION_A_LETTRE(position_item), POSITION_A_CHIFFRE(position_item));
	}
	else if (gerer_monnaie(position_item, tab_prix[position_item], tab_monnaie)) {
		tab_inventaire[position_item] -= 1;
	}


}

void afficher_machine(int tab_inventaire[], double tab_prix[], unsigned int nb_ran, unsigned int nb_col) {
	printf("\r\n");
	char string[15];
	int position = 0;

	for (int rang = 0; rang < nb_ran; rang++) {
		for (int ligne = 0; ligne < 4; ligne++) {
			for (int col = 0; col < nb_col; col++) {
				position = (rang * nb_col) + col;
				switch (ligne) {
				case 0:
					sprintf(string, "---------");
					break;
				case 1:
					sprintf(string, "|   %c%c   ", POSITION_A_LETTRE(position), POSITION_A_CHIFFRE(position));
					break;
				case 2:
					if (tab_inventaire[position] <= 0) {
						strcpy(string, "|  vide  ");
					}
					else if (tab_inventaire[position] == 1) {
						strcpy(string, "| 1 item ");
					}
					else if (tab_inventaire[position] < 10 && tab_inventaire[position] > 1) {
						sprintf(string, "| %d items", tab_inventaire[position]);
					}
					else if (tab_inventaire[position] >= 10) {
						sprintf(string, "|%d items", tab_inventaire[position]);
					};
					break;
				case 3:
					sprintf(string, "| %.2f$  ", tab_prix[position]);
					break;
				default:
					strcpy(string, "| erreur ");
					break;

				}

				if (col == nb_col - 1 && ligne != 0) {
					printf("%s|", string);
				}
				else {
					printf("%s", string);
				}

			}
			printf("\n");

		}
	}
	for (int col = 0; col < nb_col; col++) {
		printf("---------");
	}

	printf("\r\n");
}

#if RUN_MODE

void main(void) {

	//recolter_argent(3);
	gerer_monnaie(5, 3, &tab_monnaie);
}

void gerer_commande(int tab_inventaire[], double tab_prix[], int nb_items, int tab_monnaie[]) {

	int position_item = demander_item(nb_items);

	if (tab_inventaire[position_item] <= 0) {
		printf("L’item [%c%c] n’est plus disponible", POSITION_A_LETTRE(position_item), POSITION_A_CHIFFRE(position_item));
	}
	else if (gerer_monnaie(position_item, tab_prix[position_item], tab_monnaie)) {
		tab_inventaire[position_item] -= 1;
	}


}

void afficher_machine(int tab_inventaire[], double tab_prix[], unsigned int nb_ran, unsigned int nb_col) {
	printf("\r\n");
	char string[15];
	int position = 0;

	for (int rang = 0; rang < nb_ran; rang++) {
		for (int ligne = 0; ligne < 4; ligne++) {
			for (int col = 0; col < nb_col; col++) {
				position = (rang * nb_col) + col;
				switch (ligne) {
				case 0:
					sprintf(string, "---------");
					break;
				case 1:
					sprintf(string, "|   %c%c   ", POSITION_A_LETTRE(position), POSITION_A_CHIFFRE(position));
					break;
				case 2:
					if (tab_inventaire[position] <= 0) {
						strcpy(string, "|  vide  ");
					}
					else if (tab_inventaire[position] == 1) {
						strcpy(string, "| 1 item ");
					}
					else if (tab_inventaire[position] < 10 && tab_inventaire[position] > 1) {
						sprintf(string, "| %d items", tab_inventaire[position]);
					}
					else if (tab_inventaire[position] >= 10) {
						sprintf(string, "|%d items", tab_inventaire[position]);
					};
					break;
				case 3:
					sprintf(string, "| %.2f$  ", tab_prix[position]);
					break;
				default:
					strcpy(string, "| erreur ");
					break;

				}

				if (col == nb_col - 1 && ligne != 0) {
					printf("%s|", string);
				}
				else {
					printf("%s", string);
				}

			}
			printf("\n");

		}
	}
	for (int col = 0; col < nb_col; col++) {
		printf("---------");
	}

	printf("\r\n");
}



/*===========================================================================*/
/*Definition du main test */
/*===========================================================================*/
#if TEST_MODE
void main(void)
{
	srand(time(NULL));
	int commande = -1;			  // Commande utilisateur
	int code;					  // Code correspondant � la commande utilisateur
	int no_item;				  // Num�ro de l'item correspondant � la commande utilisateur
	int tab_inventaire[NB_ITEMS]; // Tableau d'inventaire
	double tab_prix[NB_ITEMS];	  // Tableau de prix
	char lettre_item;			  // Lettre d'un item en question
	char chiffre_item;			  // Chiffre d'un item en question

	printf("Main pour les tests unitaires de la Partie 1..\r\n\r\n");
	// On demande � l'utilisateur d'entr�e des valeurs jusqu'� ce qu'il quitte
	while (commande != 0)
	{
		commande = valider_commande();
		code = donner_code(commande);
		printf("commande = %i, code = %i\r\n\r\n", commande, code);
	}
	// On initialise l'inventaire et les prix, puis on les affiches
	init_inventaire(tab_inventaire, NB_ITEMS);
	init_prix(tab_prix, NB_ITEMS);
	afficher_inventaire(tab_inventaire, NB_ITEMS);
	printf("Voici la liste des prix:\r\n");

	afficher_machine(tab_inventaire, tab_prix, NB_RANGEES, NB_COLONNES);
	for (int i = 0; i < NB_ITEMS; i++)
	{
		printf("%0.2f ", tab_prix[i]);
	}
	printf("\r\n");

	// On demande l'item voulu, puis on affiche toutes les informations sur
	// le processus et la transaction
	no_item = demander_item(NB_ITEMS);
	lettre_item = 'A' + (no_item / NB_COLONNES);
	chiffre_item = '0' + (no_item % NB_COLONNES);
	printf("Item no. %i: code[%c%c], inventaire = %i, prix = %0.2f", no_item,
		lettre_item, chiffre_item, tab_inventaire[no_item], tab_prix[no_item]);
}
#endif
/*===========================================================================*/
