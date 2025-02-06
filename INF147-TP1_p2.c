/*===========================================================================*/
/*  INF147 - Travail pratique (TP) #1 Partie 1. */
/*===========================================================================*/

/*
 Module		: TP1_p1.c
 Par		: Anthony Diotte
 Créé le    : 24 janvier 2025

Description :
Ce fichier contient la fonction main(), ainsi que lesfonctions nécessaires
au fonctionnement d'une machine distributrice. On y retrouve la gestion des
paiements, de l'inventaire et des prix des articles, ainsi que la prise de
commandes utilisateur. Certaines fonctions gèrent également l'affichage des
informations destinées à l'utilisateur.

*/

/*===========================================================================*/
//Permet de désactiver certains warnings du compilateur 
#define _CRT_SECURE_NO_WARNINGS 

// Librairies usuelles à inclure 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TEST_MODE 0 //1 = test mode on, 0 = test mode off
#define RUN_MODE 1  //1 = run mode on, 0 = run mode off
// Macro pour vider le buffer du terminal après l'utilisation de `scanf`.
#define FFLUSH() while (getchar() != '\n') {}

/* les dimensions de la machine-distributrice avec son nombre total d'items. */
#define NB_RANGEES     6      //le nombre de rangées  (doit être entre 2 et 6 max.) 
#define NB_COLONNES    6      //le nombre de colonnes (doit être entre 2 et 7 max.)
#define NB_ITEMS       (NB_RANGEES * NB_COLONNES)

// Macro convertissant le code utilisateur (lettre-chiffre) en index d'item.
#define NO_ITEM(lettre, chiffre) ((lettre - 'A') * NB_COLONNES + (chiffre - '0'))

/* Les 5 codes-capteur pour la gestion du capteur */
#define CODE_QUITTER                 0
#define CODE_BOUTON_COMMANDE         1
#define CODE_CAPTEUR_PIECE_25C      10
#define CODE_CAPTEUR_PIECE_1D       11
#define CODE_CAPTEUR_PIECE_2D       12
#define CODE_CAPTEUR_BILLET_5D      13
#define CHOIX_MAX                    5   //nb. maximal du choix-menu

/* Constante-tableau des valeurs des 3 pièces de monnaie */
#define NB_PIECES   3
const double VAL_PIECES[NB_PIECES] = { 0.25, 1.00, 2.00 };
#define BILLET_5D   5.00   //billets de 5 dollars

// Les quantités de pièces de monnaie en inventaire (réduire ces valeurs pour vos tests)
#define NB_PIECES_25C    1
#define NB_PIECES_1D     1
#define NB_PIECES_2D     1

/* Le tableau des pièces de monnaie disponibles (initialisation à la déclaration) */
int tab_monnaie[NB_PIECES] = { NB_PIECES_25C, NB_PIECES_1D, NB_PIECES_2D };


/* Constante-tableau des 5 prix possibles pour un item */
#define NB_PRIX     5
const double LISTE_PRIX[NB_PRIX] = { 2.00, 2.25, 2.50, 3.00, 3.50 };

#define INVENTAIRE_MAX               8   //quantité maximale d'un item en inventaire

/*===========================================================================*/
/*Prototype des fonctions*/
/*===========================================================================*/

/*===========================================================================*/
/*
fonction :		valider_commande()
Description :	Demande à l'utilisateur de choisir une action ou un capteur.
				Redemande une entrée valide si nécessaire..
				Les choix sont de 0 à 5
				1 : Choisir un item
				2,3,4,5 : Capteur de pièce de 0.25$, 1$, 2$, 5$
				0 : Quitter le programme

Paramètres :	Aucun
Retour :		Le choix du capteur (0 à 5)
*/
/*===========================================================================*/
unsigned int valider_commande(void);

/*===========================================================================*/
/*
fonction :		donner_code()
Description :	Cette fonction convertit le code utilisateur de 0 à 5 en code
				opération qui est soit 0, 1, 10, 11, 12 ou 13

Paramètres :	commande (Commande reçue par l'utilisateur)
Retour :		Le code correspondant à un capteur (0,1,10,11,12,13)
*/
/*===========================================================================*/
unsigned int donner_code(unsigned int commande);

/*===========================================================================*/
/*
fonction :		demander_item
Description :	Fonction qui demande à l'utilisateur quel item il désire. La
				fonction s'assure que le code existe dans le scénario étudié
				et redemande le code tant qu'il n'est pas bon.


Paramètres :	nb_items (Le nombre total d'items)
Retour :		L'index de l'item en question entre 0 et NB_ITEMS-1
*/
/*===========================================================================*/
unsigned int demander_item(unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		init_prix()
Description :	Fonction qui initialise aléatoirement le prix de tous les items
				dans la machine distributrice. Les valeurs sont mises dans un
				tableau qu'on passe en paramètres. Les valeurs de prix possible
				sont 2.00$, 2.25$, 2.50$, 3.00$ ou 3.50$.


Paramètres :	tab_prix (Un tableau de réel initialement vide)
				nb_items (Le nombre d'item contenu dans le tableau de prix)
Retour :		Aucun (On remplit le tableau en paramètre)
*/
/*===========================================================================*/
void init_prix(double tab_prix[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		init_inventaire
Description :	Fonction qui remplit le tableau d'inventaire aléatoirement avec
				des valeurs variant de 0 à INVENTAIRE_MAX

Paramètres :	tab_inventaire (Tableau d'inventaire initialement vide)
				nb_items (Nombre d'items différents dans l'inventaire)
Retour :		Aucun (On remplit le tableau d'inventaire)
*/
/*===========================================================================*/
void init_inventaire(int tab_inventaire[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		afficher_inventaire()
Description :	Fonction qui affiche l'inventaire des items. On affiche d'abord
				les codes des items, puis les quantités de chaque items.


Paramètres :	tab_inventaire (Tableau qui contient l'inventaire)
Retour :		nb_items (Le nombre d'items différents dans l'inventaire)
*/
/*===========================================================================*/
void afficher_inventaire(int tab_inventaire[], unsigned int nb_items);

/*===========================================================================*/
/*
fonction :		recolter_argent()
Description :	Fonction permettant de récolter l’argent nécessaire selon le 
				prix d'item reçu. La fonction boucle en faisant la saisie de 
				codes-capteurs pour voir si une nouvelle pièce ou billet a été 
				insérée. Selon la pièce de monnaie (ou un billet de 5$) reçue, 
				on ajoute le montant à un total accumulé, jusqu'à ce que ce 
				total soit égal ou supérieur au prix reçu.  Si l'utilisateur 
				sélectionne "1. Choisir un item", le système annule la commande 
				en cours et on quitte la boucle de saisie.  On retourne le total
				accumulé.

Paramètres :	prix:  Prix de l'item (réel). Un des 5 prix valides (voir les constantes)
				tab_monnaie: Tableau d’entiers indiquant le nombre de pièces de monnaie disponibles.

Retour :		le montant total reçu (réel), si la commande est annulée ce montant sera < prix.

*/
/*===========================================================================*/
double recolter_argent(double prix_item);


/*===========================================================================*/
/*
fonction :		recolter_argent()
Description :


Paramètres :
Retour :
*/
/*===========================================================================*/
int redonner_monnaie(double montant_a_retourner, int tab_monnaie[]);


/*===========================================================================*/
/*
fonction :		gerer_monnaie()
Description :


Paramètres :
Retour :
*/
/*===========================================================================*/
int gerer_monnaie(int item, double prix, int tab_monnaie);
/*===========================================================================*/

/*===========================================================================*/
/*Implémentation des fonctions privées*/
/*===========================================================================*/

//On demande d'entrer une valeur tant que la valeur est invalide
unsigned int valider_commande(void) {
	int commande = CHOIX_MAX + 1; //On initialise commande à une valeur invalide
	//Tant que commande ne prends pas une valeur valide, on redemande l'entrée
	while (commande < 0 || commande > CHOIX_MAX) {
		printf("Quel capteur voulez-vous simuler?\r\n");
		printf("1- Choisir un item\r\n");
		printf("2- 0.25$\r\n");
		printf("3- 1.00$\r\n");
		printf("4- 2.00$\r\n");
		printf("5- 5.00$\r\n");
		printf("0- Quitter\r\n");
		printf(">> ");
		scanf("%i", &commande);
		//printf("\r\n");
		FFLUSH();
	}
	//Lorsque commande prends une valeur valide, on sort de la boucle while()
	//et on retourne cette valeur
	return commande;
};

//On associe chaque commande avec un code
unsigned int donner_code(unsigned int commande) {
	//On associe chaque valeurs de commande à un code qu'on retourne
	switch (commande) {
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
		//Si un problème se produit et que commande prend une valeur invalide, on
		//on retourne une autre valeur invalide
	default:  return -1;
	}
};

//On demande quel item l'utilisateur désire tant qu'il est invalide
unsigned int demander_item(unsigned int nb_items) {
	char lettre = '0'; //on initialise à une valeur invalide
	char chiffre = '0';
	//Avec le nombre de colonnes et d'items, on trouve le nombre de chiffres possibles
	char nb_chiffres = (nb_items - 1) % NB_COLONNES;
	//Avec le nombre de colonnes et d'items, on trouve le nombre de lettres possibles
	char nb_lettres = (nb_items - 1) / NB_COLONNES;

	//Tant que le code entré est invalide, on le redemande
	while ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
		(chiffre < '0') || chiffre > '0' + nb_chiffres) {
		printf("Entrez le code de l'item:");
		scanf("%c%c", &lettre, &chiffre);
		printf("\r\n");
		lettre = toupper(lettre);
		FFLUSH();

		//Si la valeur entrée est invalide, on l'indique
		if ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
			(chiffre < '0') || chiffre > '0' + nb_chiffres) {
			printf("Item invalide \r\n");
		}
	}
	//On retourne l'index de l'item voulu à partir de son code lettre-chiffre
	return NO_ITEM(lettre, chiffre);
};

//On parcourt le tableau de prix et on y mets des valeurs de prix aléatoirement
void init_prix(double tab_prix[], unsigned int nb_items) {
	unsigned int indice_prix = 0; //Indice pour le tableau LISTE_PRIX
	//on parcourt le tableau de prix pour le remplir
	for (int i = 0; i < nb_items; i++) {
		//On trouve un entier aléatoire qu'on ramène de 0 à NB_PRIX
		indice_prix = rand() % NB_PRIX;
		tab_prix[i] = LISTE_PRIX[indice_prix];
	}
};

//On parcourt le tableau d'inventaire et on y mets des valeurs d'inventaire aléatoirement
void init_inventaire(int tab_inventaire[], unsigned int nb_items) {
	unsigned int nb_inventaire = 0; //Quantité en inventaire pour l'item courrant
	//On parcourt le tableau d'inventaire pour le remplir
	for (int i = 0; i < nb_items; i++) {
		//On trouve un entier aléatoire qu'on ramène de 0 à INVENTAIRE_MAX
		nb_inventaire = rand() % (INVENTAIRE_MAX + 1);
		//On attribue une valeur de 0 à INVENTAIRE_MAX à un item dans 
		// l'inventaire
		tab_inventaire[i] = nb_inventaire;
	}
};

//On affiche simplement l'inventaire
void afficher_inventaire(int tab_inventaire[], unsigned int nb_items) {
	char lettre;
	int chiffre;
	printf("ITEM : ");
	//On affiche tous les items
	for (int i = 0; i < nb_items; i++) {
		lettre = 'A' + (i / NB_COLONNES);
		chiffre = i % NB_COLONNES;
		printf("%c%d ", lettre, chiffre);
	}
	printf("\r\n");

	//On affiche l'inventaire de chaque items
	printf("INV. : ");
	for (int i = 0; i < nb_items; i++) {
		printf("%i  ", tab_inventaire[i]);
	}
	printf("\n");
};

double recolter_argent(double prix_item) {
	double cumul_argent = 0;
	int commande =0;
	double piece = 0;
	while ((commande != CODE_BOUTON_COMMANDE) && (cumul_argent < prix_item)) {
		commande = donner_code(valider_commande());
		switch (commande) {
		case CODE_CAPTEUR_PIECE_25C: case CODE_CAPTEUR_PIECE_1D: case CODE_CAPTEUR_PIECE_2D:
			piece = VAL_PIECES[commande-CODE_CAPTEUR_PIECE_25C];
			break;
		case CODE_CAPTEUR_BILLET_5D:
			piece = BILLET_5D;
			break;
		}

		cumul_argent += piece;
	}
	return cumul_argent;
}

int redonner_monnaie(double montant_a_retourner, int tab_monnaie[]) {
	int tab_nb_pieces[NB_PIECES] = { 0 };
	int pos_piece = NB_PIECES - 1;
	double montant = montant_a_retourner;
	int nb_pieces = 0;

	while (pos_piece >= 0 && montant > 0) {
		nb_pieces = montant / VAL_PIECES[pos_piece];

		nb_pieces = (nb_pieces > tab_monnaie[pos_piece]) ? tab_monnaie[pos_piece] : nb_pieces;
		
		if (nb_pieces > 0) {
			tab_nb_pieces[pos_piece] = nb_pieces;
			tab_monnaie[pos_piece] -= nb_pieces;
			montant -= nb_pieces * VAL_PIECES[pos_piece];
		}
		pos_piece--;
	}
	if (montant > 0) {
		for (int i = 0; i <= NB_PIECES; i++) {
			tab_monnaie[i] += tab_nb_pieces[i];
			tab_nb_pieces[i] = 0;
		}
		
		return 0; //Pas assez de monaie dans la machine
	}
	else {
		for (int i = (NB_PIECES-1); i >= 0; i--) {
			printf("nb de %0.2f : %d\r\n", VAL_PIECES[i], tab_nb_pieces[i]);
		}
		return 1; //Monnaie redonnée adéquatement
	}
}

int gerer_monnaie(int item, double prix, int tab_monnaie) {
	double montant_recu = recolter_argent(prix);
	double retour = 0;
	if (montant_recu < prix) {
		printf("Total: %0.2f$ / %0.2f$\r\n", montant_recu, prix);
		printf("Commande annulée\r\n");
		retour = montant_recu;
		printf("Re-voici votre argent :\r\n");
		redonner_monnaie(retour, &tab_monnaie);
		return 0;
	}
	else if (montant_recu >= prix) {
		retour = montant_recu - prix;
		printf("Re-voici votre argent :\r\n");
		redonner_monnaie(retour, &tab_monnaie);
		return 1;
	}
	
}

#if RUN_MODE

void main(void) {

	recolter_argent(3);
	gerer_monnaie(5, 3, &tab_monnaie);
}
#endif










#if TEST_MODE
/*===========================================================================*/
/*Fonction principale de test qui appelle l'execution des autres fonctions*/
/*===========================================================================*/
void main(void) {
	srand(time(NULL));
	int commande = -1;				//Commande utilisateur
	int code;						//Code correspondant à la commande utilisateur
	int no_item;		//Numéro de l'item correspondant à la commande utilisateur
	int tab_inventaire[NB_ITEMS];	//Tableau d'inventaire
	double tab_prix[NB_ITEMS];		//Tableau de prix
	char lettre_item;				//Lettre d'un item en question
	char chiffre_item;				//Chiffre d'un item en question

	printf("Main pour les tests unitaires de la Partie 1..\r\n\r\n");
	//On demande à l'utilisateur d'entrée des valeurs jusqu'à ce qu'il quitte
	while (commande != 0) {
		commande = valider_commande();
		code = donner_code(commande);
		printf("commande = %i, code = %i\r\n\r\n", commande, code);
	}
	//On initialise l'inventaire et les prix, puis on les affiches
	init_inventaire(&tab_inventaire, NB_ITEMS);
	init_prix(&tab_prix, NB_ITEMS);
	afficher_inventaire(&tab_inventaire, NB_ITEMS);
	printf("Voici la liste des prix:\r\n");
	for (int i = 0; i < NB_ITEMS; i++) {
		printf("%0.2f ", tab_prix[i]);
	}
	printf("\r\n");

	//On demande l'item voulu, puis on affiche toutes les informations sur
	//le processus et la transaction
	no_item = demander_item(NB_ITEMS);
	lettre_item = 'A' + (no_item / NB_COLONNES);
	chiffre_item = '0' + (no_item % NB_COLONNES);
	printf("Item no. %i: code[%c%c], inventaire = %i, prix = %0.2f", no_item,
		lettre_item, chiffre_item, tab_inventaire[no_item], tab_prix[no_item]);

}
/*===========================================================================*/
#endif