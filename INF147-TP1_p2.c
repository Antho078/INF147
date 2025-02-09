/*===========================================================================*/
/*  INF147 - Travail pratique (TP) #1 Partie 2. */
/*===========================================================================*/

/*
 Module		: INF147-TP1_p2.c
 Par		: Anthony Diotte et Olivier Cotnoir
 Cree le    : 24 janvier 2025

Description :
Ce fichier contient la fonction main(), ainsi que les fonctions necessaires
au fonctionnement d'une machine distributrice. On y retrouve la gestion des
paiements, de l'inventaire et des prix des articles, ainsi que la prise de
commandes utilisateur. Certaines fonctions gerent egalement l'affichage des
informations destinees a l'utilisateur.

*/

/*===========================================================================*/
// Permet de desactiver certains warnings du compilateur
#define _CRT_SECURE_NO_WARNINGS

// Librairies usuelles a inclure
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

// Macro pour vider le buffer du terminal apres l'utilisation de `scanf`.
#define FFLUSH()              \
	while (getchar() != '\n') \
	{                         \
	}

/* les dimensions de la machine-distributrice avec son nombre total d'items. */
#define NB_RANGEES 6  // le nombre de rangees  (doit etre entre 2 et 6 max.)
#define NB_COLONNES 6 // le nombre de colonnes (doit etre entre 2 et 7 max.)
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

/* Constante-tableau des valeurs des 3 pieces de monnaie */
#define NB_PIECES 3
const double VAL_PIECES[NB_PIECES] = {0.25, 1.00, 2.00};
#define BILLET_5D 5.00 // billets de 5 dollars

// Les quantites de pieces de monnaie en inventaire (reduire ces valeurs pour vos tests)
#define NB_PIECES_25C 1
#define NB_PIECES_1D 1
#define NB_PIECES_2D 1

/* Le tableau des pieces de monnaie disponibles (initialisation a la declaration) */
int tab_monnaie[NB_PIECES] = {NB_PIECES_25C, NB_PIECES_1D, NB_PIECES_2D};

/* Constante-tableau des 5 prix possibles pour un item */
#define NB_PRIX 5
const double LISTE_PRIX[NB_PRIX] = {2.00, 2.25, 2.50, 3.00, 3.50};

#define INVENTAIRE_MAX 8 // quantite maximale d'un item en inventaire

/*===========================================================================*/
/*Prototype des fonctions*/
/*===========================================================================*/

/*===========================================================================*/
/*
fonction :		valider_commande()
Description :	Demande a l'utilisateur de choisir une action ou un capteur.
				Redemande une entree valide si necessaire ...
				Les choix sont de 0 a 5
				1 : Choisir un item
				2,3,4,5 : Capteur de piece de 0.25$, 1$, 2$, 5$
				0 : Quitter le programme

Parametres :	Aucun
Retour :		Le choix du capteur (0 a 5)
*/
/*===========================================================================*/
int valider_commande(void);

/*===========================================================================*/
/*
fonction :		donner_code()
Description :	Cette fonction convertit le code utilisateur de 0 a 5 en code
				operation qui est soit 0, 1, 10, 11, 12 ou 13

Parametres :	commande (Commande recue par l'utilisateur)
Retour :		Le code correspondant a un capteur (0,1,10,11,12,13)
*/
/*===========================================================================*/
int donner_code(int commande);

/*===========================================================================*/
/*
fonction :		demander_item()
Description :	Fonction qui demande a l'utilisateur quel item il desire. La
				fonction s'assure que le code existe dans le scenario etudie
				et redemande le code tant qu'il n'est pas bon.


Parametres :	nb_items (Le nombre total d'items)
Retour :		L'index de l'item en question entre 0 et NB_ITEMS-1
*/
/*===========================================================================*/
int demander_item(int nb_items);

/*===========================================================================*/
/*
fonction :		init_prix()
Description :	Fonction qui initialise aleatoirement le prix de tous les items
				dans la machine distributrice. Les valeurs sont mises dans un
				tableau qu'on passe en parametres. Les valeurs de prix possible
				sont 2.00$, 2.25$, 2.50$, 3.00$ ou 3.50$.


Parametres :	tab_prix (Un tableau de reel initialement vide)
				nb_items (Le nombre d'item contenu dans le tableau de prix)
Retour :		Aucun (On remplit le tableau en parametre)
*/
/*===========================================================================*/
void init_prix(double tab_prix[], int nb_items);

/*===========================================================================*/
/*
fonction :		init_inventaire()
Description :	Fonction qui remplit le tableau d'inventaire aleatoirement avec
				des valeurs variant de 0 a INVENTAIRE_MAX

Parametres :	tab_inventaire (Tableau d'inventaire initialement vide)
				nb_items (Nombre d'items differents dans l'inventaire)
Retour :		Aucun (On remplit le tableau d'inventaire)
*/
/*===========================================================================*/
void init_inventaire(int tab_inventaire[], int nb_items);

/*===========================================================================*/
/*
fonction :		afficher_inventaire()
Description :	Fonction qui affiche l'inventaire des items. On affiche d'abord
				les codes des items, puis les quantites de chaque items.


Parametres :	tab_inventaire (Tableau qui contient l'inventaire)
Retour :		nb_items (Le nombre d'items differents dans l'inventaire)
*/
/*===========================================================================*/
void afficher_inventaire(int tab_inventaire[], int tab_monnaie[], int nb_items);

/*===========================================================================*/
/*
fonction :		recolter_argent()
Description :	Fonction permettant de recolter l'argent necessaire selon le
				prix d'item recu. La fonction boucle en faisant la saisie de
				codes-capteurs pour voir si une nouvelle piece ou billet a ete
				inseree. Selon la piece de monnaie (ou un billet de 5$) recue,
				on ajoute le montant a un total accumule, jusqu'a ce que ce
				total soit egal ou superieur au prix recu.  Si l'utilisateur
				selectionne "1. Choisir un item", le systeme annule la commande
				en cours et on quitte la boucle de saisie.  On retourne le total
				accumule.

Parametres :	prix:			Prix de l'item (reel). Un des 5 prix valides
								(voir les constantes)
				tab_monnaie:	Tableau d'entiers indiquant le nombre de pieces
								de monnaie disponibles.

Retour :		Le montant total recu (reel), si la commande est annulee
				ce montant sera < prix.

*/
/*===========================================================================*/
double recolter_argent(double prix_item, int tab_monnaie[]);

/*===========================================================================*/
/*
fonction :		Fonction permettant de redonner la monnaie necessaire pour
				combler un montant recu. On utilise ici l'algorithme
				classique de distribution de monnaie.



Parametres :	montant:		Le montant de monnaie a rembourser (reel).
				tab_monnaie:	Tableau d'entiers indiquant le nombre de pieces
								de monnaie disponibles.

Retour :		1 si la monnaie a ete redonnee, 0 sinon (pas assez de pieces dans la machine).
*/
/*===========================================================================*/
int redonner_monnaie(double montant_a_retourner, int tab_monnaie[]);

/*===========================================================================*/
/*
fonction :		gerer_monnaie()
Description :	Fonction gerant la logique pour la gestion de l'argent et la
				monnaie. Recoit une position-item et un prix. On imprime ces
				deux valeurs a l'ecran. On appelle recolter_argent() et si
				cette fonction retourne un montant insuffisant, on considere
				que l'achat a ete annulee et on rembourse l'utilisateur (en
				affichant un message d'annulation). Si le montant retourne
				par recolter_argent() est suffisant, on considere l'achat
				comme complet pour l'instant.  Si le montant retourne est plus
				grand que le prix de l'item on tente de redonner la monnaie de
				cette difference.  Si la fonction de distribution de monnaie a
				echoue (manque de monnaie), on considere l'achat comme annule
				et on redonne le montant du prix au complet en monnaie a
				l'utilisateur (en affichant un message de remboursement et un
				message demandant de payer avec la monnaie exacte). Si l'achat
				est complet la fonction retourne 1 et si l'achat a ete annule, 0.

Parametres :	item:			Position-tableau de l'item (entre 0 et NB_ITEMS-1)
				prix:			Prix de l'item a payer (un des 5 prix valides,
								voir les constantes)
				tab_monnaie:	Tableau d'entiers indiquant le nombre de pieces
								de monnaie disponibles.

Retour: 1 si l'achat s'est effectue avec succes, 0 sinon.
*/
/*===========================================================================*/
int gerer_monnaie(int item, double prix, int tab_monnaie[]);

/*===========================================================================*/
/*
fonction :		gerer_commande()
Description :	Fonction permettant la gestion d'une nouvelle commande. La
				fonction appelle « demander_item() » et vérifie si l'item est
				disponible. Si l’item demandé n’est pas disponible, on affiche
				un message informatif tel que : « l’item [B2] n’est plus
				disponible » (la position-tableau est convertie en code "A0").
				Sinon, elle effectue la gestion de la monnaie et si l’achat de
				l’item est bien complété l'inventaire disponible de l’item
				choisi sera réduit de 1 dans le tableau d’inventaire.

Paramètres :	tab_inventaire: Tableau d’entiers contenant la quantité disponible
								de chaque item.
				tab_prix:       Tableau de réels contenant le prix de chaque item.
				nb_items:       Taille des 2 tableaux (entier).
				tab_monnaie:    Tableau d’entiers du nombre de pièces de monnaie
								disponibles.


Retour: rien
*/
/*===========================================================================*/
void gerer_commande(int tab_inventaire[], double tab_prix[], int nb_items, int tab_monnaie[]);

/*===========================================================================*/
/*
fonction :		afficher_machine()
Description :	Fonction permettant d'afficher l'inventaire et les prix des items
				sous forme visuelle. Affiche un tableau de nb_ran par
				nb_col et montre les informations suivantes : No. item
				(de "A0" a "F5" pour un modele 6x6), la quantite disponible
				ainsi que le prix. NOTE : Si la quantite disponible d'un item
				est 0, on affiche "vide". S'il y a un seul item de disponible
				on affiche 1 item , mais il y en a plusieurs c'est X items


Parametres :	tab_inventaire: Tableau d'entiers des quantites des items.
				tab_prix:       Tableau de reels des prix des items.
				nb_ran:         Le nombre de rangees de la machine (entier).
				nb_col:         Le nombre de colonnes de la machine (entier).


Retour: rien
*/
/*===========================================================================*/
void afficher_machine(int tab_inventaire[], double tab_prix[], int nb_ran, int nb_col);

/*===========================================================================*/

/*===========================================================================*/
/*
fonction :		afficher_machine()
Description :	Fonction permettant d'afficher l'inventaire et les prix des items
				sous forme visuelle. Affiche un tableau de « nb_ran » par
				« nb_col » et montre les informations suivantes : No. item
				(de "A0" à "F5" pour un modèle 6x6), la quantité disponible
				ainsi que le prix. NOTE : Si la quantité disponible d’un item
				est 0, on affiche "vide". S’il y a un seul item de disponible
				on affiche « 1 item », mais il y en a plusieurs c’est « X items »


Paramètres :	tab_inventaire: Tableau d’entiers des quantités des items.
				tab_prix:       Tableau de réels des prix des items.
				nb_ran:         Le nombre de rangées de la machine (entier).
				nb_col:         Le nombre de colonnes de la machine (entier).


Retour: rien
*/
/*===========================================================================*/
void afficher_machine(int tab_inventaire[], double tab_prix[], int nb_ran, int nb_col);

/*===========================================================================*/
/*
fonction :		gerer_monnaie()
Description :	Fonction gérant la logique pour la gestion de l’argent et la
				monnaie. Reçoit une position-item et un prix. On imprime ces
				deux valeurs à l’écran. On appelle « recolter_argent() » et si
				cette fonction retourne un montant insuffisant, on considère
				que l’achat a été annulée et on rembourse l'utilisateur (en
				affichant un message d’annulation). Si le montant retourné
				par « recolter_argent() » est suffisant, on considère l’achat
				comme complet pour l’instant.  Si le montant retourné est plus
				grand que le prix de l’item on tente de redonner la monnaie de
				cette différence.  Si la fonction de distribution de monnaie a
				échoué (manque de monnaie), on considère l’achat comme annulé
				et on redonne le montant du prix au complet en monnaie à
				l’utilisateur (en affichant un message de remboursement et un
				message demandant de payer avec la monnaie exacte). Si l’achat
				est complet la fonction retourne 1 et si l’achat a été annulé, 0.

Paramètres :	item:			Position-tableau de l'item (entre 0 et NB_ITEMS-1)
				prix:			Prix de l'item à payer (un des 5 prix valides,
								voir les constantes)
				tab_monnaie:	Tableau d’entiers indiquant le nombre de pièces
								de monnaie disponibles.

Retour: 1 si l’achat s'est effectué avec succès, 0 sinon.
*/
/*===========================================================================*/
int gerer_monnaie(int item, double prix, int tab_monnaie[]);

/*===========================================================================*/
/*
fonction :		total_monnaie()
Description :	Fonction calculant le montant total de la monnaie dans la machine.
				Elle recoit tab_monnaie[] ainsi que sa taille et retourne un double
				representant la valeur monetaire totale. Pour ce faire, elle multiplie
				le nombre de chaque piece dans tab_monnaie[] par la valeur de la piece et
				additionne les valeurs resultantes pour chaque type de piece.

Paramètres :	tab_monnaie:	Tableau d’entiers indiquant le nombre de pièces
								de monnaie disponibles.

				nb_items:		Nombre d'items differents dans la machine.
								Correspond a la taille du tableau.

Retour: Un double representant la valeur totale de la monnaie dans la machine.
*/
/*===========================================================================*/
double total_monnaie(int tab_monnaie[], int nb_items);

/*===========================================================================*/
/*Definition du main
/*===========================================================================*/

void main(void)
{
	srand(time(NULL));
	int commande = -1;			  // Commande utilisateur
	int code;					  // Code correspondant a la commande utilisateur
	int tab_inventaire[NB_ITEMS]; // Tableau d'inventaire
	double tab_prix[NB_ITEMS];	  // Tableau de prix

	init_inventaire(tab_inventaire, NB_ITEMS); // On initialise le tableau d'inventaire avec des quantites aleatoires
	init_prix(tab_prix, NB_ITEMS);			   // On initialise le tableau des prix avec des prix aleatoires

	while (1)
	{
		afficher_machine(tab_inventaire, tab_prix, NB_RANGEES, NB_COLONNES);	// On affiche l'interface de la machine sous forme de tableau
		afficher_inventaire(tab_inventaire, tab_monnaie, NB_ITEMS);				// On affiche l'inventaire de maniere lineaire

		if (total_monnaie(tab_monnaie, NB_PIECES) < 5)							// Si le total de la valeur de la monnaie est inferieur a 5$,
																			    // on demande de payer en monnaie exacte
		{
			printf("Veuillez payer monnaie exacte\r\n");
		}

		// On demande a l'utilisateur d'entree une commande tant qu'il n'appuie pas sur 1
		do
		{
			code = donner_code(valider_commande());
			if (code == 0)
			{
				printf("Au revoir :(\r\n"); // On quitte si l'utilisateur appuie sur 0
				return;
			}
			else if (code > 1)
			{
				printf("Faites une commande en premier !\r\n"); // Dans tout autre cas, on redemande la saisie
			}
		} while (code != 1);


		gerer_commande(tab_inventaire, tab_prix, NB_ITEMS, tab_monnaie); // Une fois la commande validee, on fait la gestion de cette commande, en passant tous les tableaux.

		system("pause"); // Une fois la commande traitee, on attend une entree de l'utilisateur avant d'effacer l'ecran et de recommencer
		system("cls");
	}
}

/*===========================================================================*/
/*Definition des fonctions privees*/
/*===========================================================================*/

// On demande d'entrer une valeur tant que la valeur est invalide
int valider_commande(void)
{
	int commande = CHOIX_MAX + 1; // On initialise commande a une valeur invalide
	// Tant que commande ne prends pas une valeur valide, on redemande l'entree
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
int donner_code(int commande)
{
	// On associe chaque valeurs de commande a un code qu'on retourne
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
		// Si un probleme se produit et que commande prend une valeur invalide, on
		// on retourne une autre valeur invalide
	default:
		return -1;
	}
};

// On demande quel item l'utilisateur desire tant qu'il est invalide
int demander_item(int nb_items)
{
	char lettre = '0'; // on initialise a une valeur invalide
	char chiffre = '0';
	// Avec le nombre de colonnes et d'items, on trouve le nombre de chiffres possibles
	char nb_chiffres = (nb_items - 1) % NB_COLONNES;
	// Avec le nombre de colonnes et d'items, on trouve le nombre de lettres possibles
	char nb_lettres = (nb_items - 1) / NB_COLONNES;

	// Tant que le code entre est invalide, on le redemande
	while ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
		   (chiffre < '0') || chiffre > '0' + nb_chiffres)
	{
		printf("Entrez le code de l'item:");
		scanf("%c%c", &lettre, &chiffre);
		printf("\r\n");
		lettre = toupper(lettre);
		FFLUSH();

		// Si la valeur entree est invalide, on l'indique
		if ((lettre < 'A') || (lettre > 'A' + nb_lettres) ||
			(chiffre < '0') || chiffre > '0' + nb_chiffres)
		{
			printf("Item invalide \r\n");
		}
	}
	// On retourne l'index de l'item voulu a partir de son code lettre-chiffre
	return NO_ITEM(lettre, chiffre);
};

// On parcourt le tableau de prix et on y mets des valeurs de prix aleatoirement
void init_prix(double tab_prix[], int nb_items)
{
	int indice_prix = 0; // Indice pour le tableau LISTE_PRIX
	// on parcourt le tableau de prix pour le remplir
	for (int i = 0; i < nb_items; i++)
	{
		// On trouve un entier aleatoire qu'on ramene de 0 a NB_PRIX
		indice_prix = rand() % NB_PRIX;
		tab_prix[i] = LISTE_PRIX[indice_prix];
	}
};

// On parcourt le tableau d'inventaire et on y mets des valeurs d'inventaire aleatoirement
void init_inventaire(int tab_inventaire[], int nb_items)
{
	int nb_inventaire = 0; // Quantite en inventaire pour l'item courrant
	// On parcourt le tableau d'inventaire pour le remplir
	for (int i = 0; i < nb_items; i++)
	{
		// On trouve un entier aleatoire qu'on ramene de 0 a INVENTAIRE_MAX
		nb_inventaire = rand() % (INVENTAIRE_MAX + 1);
		// On attribue une valeur de 0 a INVENTAIRE_MAX a un item dans
		//  l'inventaire
		tab_inventaire[i] = nb_inventaire;
	}
};

// On affiche simplement l'inventaire
void afficher_inventaire(int tab_inventaire[], int tab_monnaie[], int nb_items)
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
		printf("%02i ", tab_inventaire[i]);
	}
	printf("\n");

	// On affiche le nombe de pièces de monnaie
	printf("Monnaie: ");
	for (int i = 0; i < NB_PIECES; i++)
	{
		printf(" %i x %0.2f$ ", tab_monnaie[i], VAL_PIECES[i]);
	}
	printf("\r\n");
};

double recolter_argent(double prix_item, int tab_monnaie[])
{
	double cumul_argent = 0; // Variable accumulant l'argent insere
	int commande = 0;		 // Variable stockant le code de la commande utilisateur
	double piece = 0;		 // Montant correspondant a la piece inseree

	// Boucle jusqu'a ce que l'utilisateur valide la commande ou que l'argent insere soit suffisant
	while ((commande != CODE_BOUTON_COMMANDE) && (cumul_argent < prix_item))
	{
		commande = donner_code(valider_commande()); // Obtient le code de la commande ou de la piece inseree

		// Verification du type de piece/billet insere
		switch (commande)
		{
		case CODE_CAPTEUR_PIECE_25C:
		case CODE_CAPTEUR_PIECE_1D:
		case CODE_CAPTEUR_PIECE_2D:
			// Calcule la valeur de la piece en fonction du code recu
			piece = VAL_PIECES[commande - CODE_CAPTEUR_PIECE_25C];
			tab_monnaie[commande - CODE_CAPTEUR_PIECE_25C] += 1;
			break;
		case CODE_CAPTEUR_BILLET_5D:
			// Detecte un billet de 5$
			piece = BILLET_5D;
			break;
		default:
			piece = 0;
			break;

		}

		cumul_argent += piece; // Ajoute la valeur de la piece au montant total accumule

		printf("\r\nTotal: %.2f$/%.2f$\r\n", cumul_argent, prix_item); // On affiche le montant cumule en cours
	}

	return cumul_argent; // Retourne le montant total collecte
}

int redonner_monnaie(double montant_a_retourner, int tab_monnaie[])
{
	int tab_nb_pieces[NB_PIECES] = {0};	  // Tableau pour stocker le nombre de pieces a rendre
	int pos_piece = NB_PIECES - 1;		  // Position actuelle dans le tableau de pieces (part du plus grand)
	double montant = montant_a_retourner; // Copie du montant a rendre pour modification
	int nb_pieces = 0;					  // Nombre de pieces utilisees pour une denomination donnee

	// Boucle pour distribuer la monnaie en utilisant les plus grandes pieces disponibles
	while (pos_piece >= 0 && montant > 0)
	{
		nb_pieces = montant / VAL_PIECES[pos_piece]; // Calcule le nombre de pieces necessaires

		// Verifie si on a assez de pieces disponibles, sinon prend le maximum possible
		nb_pieces = (nb_pieces > tab_monnaie[pos_piece]) ? tab_monnaie[pos_piece] : nb_pieces;

		if (nb_pieces > 0)
		{
			tab_nb_pieces[pos_piece] = nb_pieces;		  // Stocke le nombre de pieces utilisees
			tab_monnaie[pos_piece] -= nb_pieces;		  // Met a jour le stock de pieces
			montant -= nb_pieces * VAL_PIECES[pos_piece]; // Reduit le montant restant a rendre
		}
		pos_piece--; // Passe a la plus petite denomination suivante
	}
	// Verifie si le montant a rendre a ete totalement couvert
	if (montant > 0)
	{
		// Si on ne peut pas rendre le montant exact, on remet les pieces utilisees
		for (int i = 0; i < NB_PIECES; i++)
		{
			tab_monnaie[i] += tab_nb_pieces[i]; // Restaure le stock initial
			tab_nb_pieces[i] = 0;				// Reinitialise le tableau des pieces utilisees
		}
		return 0; // echec : pas assez de monnaie disponible
	}
	else
	{
		// Affichage du nombre de pieces utilisees pour rendre la monnaie
		for (int i = (NB_PIECES - 1); i >= 0; i--)
		{
			printf("nb de %0.2f : %d\r\n", VAL_PIECES[i], tab_nb_pieces[i]);
		}
		return 1; // Succes : monnaie redonnee correctement
	}
}

int gerer_monnaie(int item, double prix, int tab_monnaie[])
{

	// Impression de l'entete
	printf("-------------------------\n");
	printf("Vous voulez l'item : %i\n", item);
	printf("Veuillez inserer: %.2f$ SVP\n", prix);
	printf("-------------------------\n");

	double montant_recu = recolter_argent(prix, tab_monnaie);
	double retour = 0;

	// Gestion de l'annulation de la commande s'il n'y a pas assez d'argent ou si
	// on annule volontairement la transaction
	if (montant_recu < prix)
	{
		printf("Commande annulee\r\n");
		retour = montant_recu;
		printf("Re-voici votre argent :\r\n");
		redonner_monnaie(retour, tab_monnaie);
		return 0;
	}
	// Gestion du retour de l'argent apres une transaction reussie
	else
	{
		retour = montant_recu - prix;
		printf("Re-voici votre argent :\r\n");
		// Si l'achat reussi, mais qu'on ne peut pas redonner la monnaie, on annule
		// la transaction
		if (!redonner_monnaie(retour, tab_monnaie))
		{
			printf("Impossible de rendre la monnaie exacte. Commande annulee.\r\n");
			printf("Re-voici votre argent :\r\n");
			redonner_monnaie(montant_recu, tab_monnaie);
			return 0;
		}
		// Si l'achat réussi et qu'on peut redonner la monnaie, on finalise
		// la transaction
		else
		{
			printf("Achat complet.\r\n");
			return 1;
		}
	}
}

void gerer_commande(int tab_inventaire[], double tab_prix[], int nb_items, int tab_monnaie[])
{

	int position_item = demander_item(nb_items); // On demande et valide le choix de l'item 

	// Si cet item n'est plus disponible, on l'indique 
	if (tab_inventaire[position_item] <= 0) 
	{
		printf("L'item [%c%c] n'est plus disponible\r\n", POSITION_A_LETTRE(position_item), POSITION_A_CHIFFRE(position_item));
	}
	// Si l'item est toujours diponible, on essaie de faire la transaction avec gerer_monaie(), si la transaction est valide, on decremente l'inventaire.
	else if (gerer_monnaie(position_item, tab_prix[position_item], tab_monnaie)) 
	{
		tab_inventaire[position_item] -= 1;
	}
}

double total_monnaie(int tab_monnaie[], int nb_pieces)
{
	double total = 0;
	// On itere a travers tab_monnaie[] pour trouver la valeur monetaire totale de la monnaie
	for (int i = 0; i < nb_pieces; i++)
	{
		total += tab_monnaie[i] * VAL_PIECES[i]; // Le total est incremente du nombre de piece * la valeur piece
	}
	return total;
}

void afficher_machine(int tab_inventaire[], double tab_prix[], int nb_ran, int nb_col)
{
	printf("\r\n"); // Saut de ligne pour l'affichage
	char string[15]; // Tableau pour stocker les chaines de caracteres a afficher
	int position = 0; // Variable pour calculer la position de l'element dans les tableaux

	// Boucle sur les rangees
	for (int rang = 0; rang < nb_ran; rang++)
	{
		// Boucle pour afficher chaque ligne d'une rangee (4 lignes)
		for (int ligne = 0; ligne < 4; ligne++)
		{
			// Boucle sur les colonnes
			for (int col = 0; col < nb_col; col++)
			{
				position = (rang * nb_col) + col; // Calcul de la position lineaire dans le tableau

				// Choix du contenu de la ligne affichee
				switch (ligne)
				{
				case 0:
					sprintf(string, "---------"); // Ligne horizontale separatrice
					break;
				case 1:
					sprintf(string, "|   %c%c   ", POSITION_A_LETTRE(position), POSITION_A_CHIFFRE(position)); // Code alphanumerique de l'item
					break;
				case 2:
					// Affichage de l'etat de l'inventaire
					if (tab_inventaire[position] <= 0)
					{
						strcpy(string, "|  vide  "); // Case vide
					}
					else if (tab_inventaire[position] == 1)
					{
						strcpy(string, "| 1 item "); // Un seul article
					}
					else if (tab_inventaire[position] < 10 && tab_inventaire[position] > 1)
					{
						sprintf(string, "| %d items", tab_inventaire[position]); // Moins de 10 articles
					}
					else if (tab_inventaire[position] >= 10)
					{
						sprintf(string, "|%d items", tab_inventaire[position]); // 10 articles ou plus
					}
					break;
				case 3:
					sprintf(string, "| %.2f$  ", tab_prix[position]); // Affichage du prix avec deux decimales
					break;
				default:
					strcpy(string, "| erreur "); // Gestion d'erreur
					break;
				}

				// Affichage de la derniere colonne avec une barre verticale pour fermer le tableau
				if (col == nb_col - 1 && ligne != 0)
				{
					printf("%s|", string);
				}
				else
				{
					printf("%s", string);
				}
			}
			printf("\n"); // Passage a la ligne suivante
		}
	}

	// Affichage de la derniere ligne horizontale
	for (int col = 0; col < nb_col; col++)
	{
		printf("---------");
	}

	printf("\r\n"); // Saut de ligne final
}
