#include "Jeu.h"

int TailleGrille=0; // Taille de la grille (elle sera initialisée dans LectureFichiers)
char **GrilleMots = NULL; // Grille de mots mélés (sous forme de liste de chaines qui sera initialisée dans LectureFichiers)
char **GrilleMotsBarres = NULL; // Grille de mots barrés (trouvés) (Elle sera initialisée dans LectureFichiers)
int NombreDeMots=0; // Nombre de mots à rechercher (il sera initialisé dans LectureFichiers)
char **ListeMots=NULL; // Liste des mots à rechercher (elle sera initialisée dans LectureFichiers)
int *ListeMotsBarres=NULL; // Liste des mots barrés (1=trouvé, 0=non trouvé) (elle sera initialisée dans LectureFichiers)

int LectureFichiers(char *FichierGrilleMots, char *FichierListeMots)
{
	// Lecture de la grille
	FILE *fic1=fopen(FichierGrilleMots,"r");
	if (fic1 == NULL)
   		printf("Le fichier 'Grille' est vide\n");
	else
	{
		char Buffer[100];
		// Détermine la taille de la grille
		if (fgets(Buffer, sizeof(Buffer), fic1)==NULL)
			printf("Problème de lecture de la 'Grille'\n");
		else
		{
			// On supprime l'éventuel '\n' terminateur
			char *pos = strchr(Buffer, '\n');  // Recherche d'un éventuel '\n'
			if (pos) // Le caractère '\n' a été trouvé
				*pos=0; // On remplace le '\n' par un 0-terminateur
			// On mémorise la taille de la grille
			TailleGrille = strlen(Buffer);
		}
		// Allocation de mémoire pour la grille
		GrilleMots = malloc(TailleGrille*sizeof(char *));
		// Lecture de la grille
		for (int y=0; y<TailleGrille; y++)
		{
			// On alloue la mémoire pour cette ligne
			GrilleMots[y]=malloc(TailleGrille+1);
			// On recopie la ligne dans la grille
			strncpy(GrilleMots[y], Buffer, TailleGrille);
			GrilleMots[y][TailleGrille]=0; // On met un 0 pour terminer la ligne et simplifier les affichages
			// Lecture de la ligne suivante
			if (fgets(Buffer, sizeof(Buffer), fic1)==NULL)
			{
				printf("Il manque des informations dans la grille !\n");
				break;
			}
		}
	}

	// Allocation de la grille des mots barres
	GrilleMotsBarres = malloc(TailleGrille*sizeof(char *));
	// Copie de la grille dans la grille des mots barrés : dans cette grille, on va barrer les mots que l'on va trouver dans la suite du programme
	for (int i=0; i<TailleGrille; i++)
	{
		// Allocation d'une ligne
		GrilleMotsBarres[i]=malloc(TailleGrille+1);
		// Copie de la ligne
		strcpy(GrilleMotsBarres[i], GrilleMots[i]);
	}

	// Lecture de la liste de mots
	FILE *fic2=fopen(FichierListeMots,"r");
	if (fic2 == NULL)
   		printf("Le fichier 'Liste de Mots' est vide\n");
	else
	{
		// On compte le nombre de mots dans le fichier
		NombreDeMots = 0;
		char Buffer[100];
		while (fgets(Buffer, sizeof(Buffer), fic2)!=NULL)
		{
			// On supprime l'éventuel '\n' terminateur
			char *pos = strchr(Buffer, '\n'); 
			if (pos) 
				*pos=0;
			if (Buffer[0]!=0) // ligne vide ?
				NombreDeMots++; // Non !
		}

		// On se repositionne au début du fichier
		fseek(fic2, 0, SEEK_SET);

		// On alloue un tableau pour le nombre de mot et les mots barres
		ListeMots = (char **)malloc(NombreDeMots*sizeof(char *));
		ListeMotsBarres = (int *)malloc(NombreDeMots*sizeof(int));

		// On lit les mots
		int iMot=0;
		while (fgets(Buffer, sizeof(Buffer), fic2)!=NULL)
		{
			// On supprime l'éventuel '\n' terminateur
			char *pos = strchr(Buffer, '\n'); 
			if (pos) 
				*pos=0;
			// On ignore les lignes vides
			if (Buffer[0]==0) continue;
			// On alloue la mémoire pour conserver ce mot
			char *s = malloc(strlen(Buffer)+1); // +1 pour le 0 terminateur
			// On copie le mot
			strcpy(s, Buffer);
			// et on le met dans le tableau
			ListeMots[iMot] = s;
			// On marque le mot comme non barré et on passe au suivant
			ListeMotsBarres[iMot++]=0; // Pour barrer le mot, on mettra la valeur 1
		}

		// On ferme le fichier
		fclose(fic2);
	} 
	return 0;
}

void AffichageGrille(char **Grille)
{
	// On affiche la coordonnée x de la grille
	printf("    ");
	for (int j=0; j<TailleGrille; j++)
	{
		if (j<10)
			printf(" ");
		else
			printf("%d", j/10);
	}
	printf("\n");
	printf("    ");
	// On affiche la coordonnée y de la grille
	for (int j=0; j<TailleGrille; j++)
		printf("%d", j%10);
	printf("\n");
	// On affiche la grille
	for (int i=0; i<TailleGrille; i++)
	{
		printf("%2d  ", i);
		printf("%s\n", Grille[i]);
	}
}

void AffichageMots(char **ListeMots)
{
	for(int i=0; i<NombreDeMots; i++)
		if (ListeMotsBarres[i]==0)
			printf("%d: %s\n", i, ListeMots[i]);
}


int VerificationMot(int x, int y, int SelonX, int SelonY, char **GrilleMots, char **GrilleMotsBarres, int iMot)
{
	char *MotEnCours = ListeMots[iMot];
	long LongueurMot=strlen(MotEnCours);

	// Le mot tient-il ? Non
	if (SelonX>0 && x+LongueurMot > TailleGrille)
		return 0;

	if (SelonX<0 && x-LongueurMot+1 < 0)
		return 0;

	if (SelonY>0 && y+LongueurMot > TailleGrille)
		return 0;

	if (SelonY<0 && y-LongueurMot+1 < 0)
		return 0;

	// Le mot est-il ici ?
	int bTrouve=1; // On part du principe que oui
	for (int k=0; k<LongueurMot; k++)
	{
		if (GrilleMots[y+SelonY*k][x+SelonX*k]!=MotEnCours[k]) 
		{  
			bTrouve=0;
			break;
		} // Finalement, non, il n'y est pas
	}
	
	if (bTrouve)
	{
		if (SelonX>0 && SelonY==0)
			printf("mot trouvé sur la ligne de gauche à droite : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX<0 && SelonY==0)
			printf("mot trouvé sur la ligne de droite à gauche : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX==0 && SelonY>0)
			printf("mot trouvé sur la colonne de haut en bas : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX==0 && SelonY<0)
			printf("mot trouvé sur la colonne de bas en haut : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX>0 && SelonY>0)
			printf("mot trouvé sur diagonale droite de haut en bas : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX>0 && SelonY<0)
			printf("mot trouvé sur diagonale gauche de haut en bas : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX<0 && SelonY<0)
			printf("mot trouvé sur diagonale droite de bas en haut : %s x = %d y = %d \n", MotEnCours, x, y);
		else if (SelonX<0 && SelonY>0)
			printf("mot trouvé sur diagonale gauche de bas en haut : %s x = %d y = %d \n", MotEnCours, x, y);

		// On barre le mot trouvé
		for (int k=0; k<LongueurMot; k++)
			GrilleMotsBarres[y+SelonY*k][x+SelonX*k]='-'; 
		ListeMotsBarres[iMot]=1;
	}
	return bTrouve;
}

void JeuJoueur()
{
	while(1)
	{
		int choix = 0;
		printf("\nVous allez devoir donner les mots que vous pensez voir, ainsi que leur position\n");
		printf("\nAttention ! Les indices x et y commencent à 0...\n");
		printf("\nLa liste de mot est :\n");
		AffichageMots(ListeMots);
		printf("\nOù avez-vous repéré le mot ?\nTapez 1 s'il est sur une ligne; tapez 2 s'il est sur une colonne; tapez 3 s'il est sur une diagonale; tapez 4 si vous pensez avoir trouver tous les mots...\n");
		scanf("%d", &choix);
		
		if (choix == 4)
		{	
			// Sauvegarde de la liste des mots barrés pour comparaison ultérieure
			char **GrilleMotsBarresSauv = malloc(TailleGrille * sizeof(char *));
			for (int i=0; i<TailleGrille; i++)
			{
				GrilleMotsBarresSauv[i] = malloc(TailleGrille+1);
				strcpy(GrilleMotsBarresSauv[i], GrilleMotsBarres[i]);
			}

			// On fait jouer l'ordinateur pour comparer le résultat 
			JeuOrdi();
			for(int x=0; x<TailleGrille; x++)
			{
				for (int y=0; y<TailleGrille; y++)
				{
					if (GrilleMotsBarresSauv[x][y]!=GrilleMotsBarres[x][y]) // Le résultat n'est pas le même ?
					{
						printf("\nPERDU ! Try again....\n\n");
						return;
					}
				}
				printf("BRAVO !!! Vous avez gagné !\n");
			}
			printf("\n");
			return; // L'utilisateur a terminé de jouer: on ne passe pas à la suite
			
		}

		// Si l'utilisateur a trouvé un mot, on lui demande des informations sur ce mot
		int sens, x, y, iMot=0;
		if (choix==1 || choix==2 || choix==3)
		{
			printf("Quel est le mot que vous avez vu? Entrez son numéro:\n");
			scanf("%d", &iMot);
			printf("Le mot est-il dans le bon sens ou dans le sens inverse? Tapez 1 s'il est dans le bon sens; 2 sinon.\n");
			scanf("%d", &sens);
			printf("Quelle est la coordonnée de départ x du mot?\n");
			scanf("%d", &x);
			printf("Quelle est la coordonnée de départ y du mot?\n");
			scanf("%d", &y);
		}
		switch(choix)
		{
			case 1:
			{
				if (sens == 1)	
					VerificationMot(x, y, 1, 0, GrilleMots, GrilleMotsBarres, iMot);
				if (sens == 2)
					VerificationMot(x, y, -1, 0, GrilleMots, GrilleMotsBarres, iMot);
				printf("\nLa grille, après le jeu, est :\n");
				AffichageGrille(GrilleMotsBarres);
				printf("\n");
				printf("Continuez...\n");
				break;
			}		
			case 2:
			{
				if (sens == 1)	
					VerificationMot(x, y, 0, 1, GrilleMots, GrilleMotsBarres, iMot);
				if (sens == 2)
					VerificationMot(x, y, 0, -1, GrilleMots, GrilleMotsBarres, iMot);
				printf("\nLa grille, après le jeu, est :\n");
				AffichageGrille(GrilleMotsBarres);
				printf("\n");
				printf("\nContinuez...\n");
				break;
			}
			case 3:
			{
				int diagonale;
				printf("Votre mot est-il sur la diagonale droite ou la diagonale gauche?\nTapez 1 pour droite; 2 pour gauche\n");
				scanf("%d", &diagonale);
				if (sens == 1 && diagonale == 1)	
					VerificationMot(x, y, 1, 1, GrilleMots, GrilleMotsBarres, iMot);
				if (sens == 1 && diagonale == 2)
					VerificationMot(x, y, -1, 1, GrilleMots, GrilleMotsBarres, iMot);
				if (sens == 2 && diagonale == 1)	
					VerificationMot(x, y, -1, -1, GrilleMots, GrilleMotsBarres, iMot);
				if (sens == 2 && diagonale == 2)
					VerificationMot(x, y, 1, -1, GrilleMots, GrilleMotsBarres, iMot);
				printf("\nLa grille, après le jeu, est :\n");
				AffichageGrille(GrilleMotsBarres);
				printf("\n");
				printf("\nContinuez...\n");
				break;
			}
			default:
			{
				printf("Commande inconnue !\n");
				break;
			}
		}
	}
}


void JeuOrdi()
{
    /*cas à traiter : mot sur une ligne dans le bon sens
                      mot sur une ligne dans le mauvais sens
                      mot sur une colonne dans le bon sens
                      mot sur une colonne dans le mauvais sens
                      mot sur une diagonale gauche-droite dans le bon sens
             	      mot sur une diagonale gauche-droite dans le mauvais sens
                      mot sur une diagonale droite-gauche dans le bon sens
             	      mot sur une diagonale droite-gauche dans le mauvais sens*/
	printf("\nLa liste de mot est :\n\n");
		AffichageMots(ListeMots);
	for (int iMot=0; iMot<NombreDeMots; iMot++)
	{   
		// Pour chaque mot :    
		// On parcours tout le tableau à la recherche des mots
		for (int x=0; x<TailleGrille; x++)
		{
			for (int y=0; y<TailleGrille; y++)
			{
				// Le mot est-il sur la ligne de gauche à droite ?
				VerificationMot(x, y, 1, 0, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la ligne de droite à gauche ?
				VerificationMot(x, y, -1, 0, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la colonne de haut en bas ?
				VerificationMot(x, y, 0, 1, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la colonne de bas en haut ?
				VerificationMot(x, y, 0, -1, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la diagonale de droite de haut en bas ?
				VerificationMot(x, y, 1, 1, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la diagonale de gauche de bas en haut ?
				VerificationMot(x, y, 1, -1, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la diagonale de droite de bas en haut ?
				VerificationMot(x, y, -1, -1, GrilleMots, GrilleMotsBarres, iMot);
				// Le mot est-il sur la diagonale de gauche de haut en bas ?
				VerificationMot(x, y, -1, 1, GrilleMots, GrilleMotsBarres, iMot);
			}
		}
	}
	// On affiche la grille en fin de jeu
	printf("\nLa grille, après le jeu, est :\n");
	AffichageGrille(GrilleMotsBarres);

	// Affichage du mot mystère
	printf("\nLes lettres du mot mystère sont :\n");
	for(int x=0; x<TailleGrille; x++)
		for (int y=0; y<TailleGrille; y++)
			if (GrilleMotsBarres[x][y]!='-')
				printf("%c", GrilleMotsBarres[x][y]);
	printf("\n");

} 					


void Interface(char *FichierGrilleMots, char *FichierListeMots)
{
	while (1)
	{
		int choix = 0;
		printf("Bienvenue !\nTapez 1 pour jouer, tapez 2 pour faire jouer l'ordinateur, tapez 3 pour quitter le jeu \n");
		scanf("%d", &choix);
		switch(choix)
		{
			case 1:
			{
				printf("\nVous avez choisi de jouer !\nVoici la grille :\n\n");
				LectureFichiers(FichierGrilleMots, FichierListeMots);
				AffichageGrille(GrilleMots);
				JeuJoueur();
				break;
		
			}
			case 2:
			{	
				struct timeval time1, time2, time3; 
				gettimeofday(&time1, NULL); // avant initialisation des données
				printf("\nVous avez choisi de faire jouer l'ordinateur.\nVoici la grille avec laquelle il va jouer :\n\n");
				LectureFichiers(FichierGrilleMots, FichierListeMots);
				AffichageGrille(GrilleMots);
				printf("\n");
				gettimeofday(&time2, NULL); // après initialisation des données
				JeuOrdi();
				printf("Mots Restants :\n");
				AffichageMots(ListeMots);
				gettimeofday(&time3, NULL); // en fin de processus de recherche
				printf("\nInitialisation time : %f ms\n", (float)((time2.tv_sec - time1.tv_sec) + (time2.tv_usec - time1.tv_usec) / 1000.f));
				printf("Process time : %f ms\n", (float)((time3.tv_sec - time2.tv_sec) + (time3.tv_usec - time2.tv_usec) / 1000.f));
				break;
			
			}
			case 3:
			{
				printf("Vous avez décider de quitter le jeu... A bientôt ;)\n");
				return;
			}
			printf("\n");
		}
	}	
}
