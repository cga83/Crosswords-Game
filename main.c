#include "Jeu.h"

int main (int argc, char *argv[])
{
	// Pour lancer le programme, il faut taper ./Jeu nomDuFichierGrille nomDuFichierListe (3 arguments nécessaires !)
	if (argc!=3)
	{
		printf("Mauvais nombre d'argument. Pour executer le programme veuillez taper : ./Jeu FichierGrille FichierListe\nPar exemple, ./Jeu Grille1.txt Liste1.txt\n");
		return 0;
	}
	char *FichierGrilleMots = argv[1];
	char *FichierListeMots = argv[2];
	Interface(FichierGrilleMots, FichierListeMots);
	return 1;
}

