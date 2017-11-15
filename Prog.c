#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>


int TailleGrille=0;
char **GrilleMots = NULL;
char **GrilleMotsBarres = NULL;
int NombreDeMots=0;
char **ListeMots=NULL;
int *ListeMotsBarres=NULL;


void JeuOrdi();

// Cette fonction lit deux fichiers (une grille de mots mêlés et une liste de mots à trouver) et les place dans deux tableaux
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
			char *pos = strchr(Buffer, '\n'); 
			if (pos) 
				*pos=0;
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
			GrilleMots[y][TailleGrille]=0;
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
	// Copie de la grille dans la grille des mots barrés
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
			if (Buffer[0]!=0)
				NombreDeMots++;
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
			ListeMotsBarres[iMot++]=0;
		}

		// On ferme le fichier
		fclose(fic2);
	} 
	return 0;
}

// Cette fonction va servir à afficher les tableaux utilisés dans la suite du programme
void AffichageGrille(char **Grille)
{
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
	for (int j=0; j<TailleGrille; j++)
		printf("%d", j%10);
	printf("\n");
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

		for (int k=0; k<LongueurMot; k++)
		{
			GrilleMotsBarres[y+SelonY*k][x+SelonX*k]='-';
		}
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

			JeuOrdi();
			for(int x=0; x<TailleGrille; x++)
			{
				for (int y=0; y<TailleGrille; y++)
				{
					if (GrilleMotsBarresSauv[x][y]!=GrilleMotsBarres[x][y])
					{
						printf("\nPERDU ! Try again....\n");
						return;
					}
				}
				printf("BRAVO !!! Vous avez gagné !\n");
			}
			printf("\n");
			return;
			
		}

		int sens, x, y, iMot=0;
		if (choix==1 || choix==2 || choix==3)
		{
			printf("Quel est le mot que vous avez vu?\n");
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
				printf("\nContinuez...\n");
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
		if (choix==1)
		{
			printf("\nVous avez choisi de jouer !\nVoici la grille :\n\n");
			LectureFichiers(FichierGrilleMots, FichierListeMots);
			AffichageGrille(GrilleMots);
			JeuJoueur();
		
		}
		
	
		if (choix==2)
		{	
			struct timeval time1, time2, time3; // au début de votre main
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
			
		}
		if (choix==3)
		{
			printf("Vous avez décider de quitter le jeu... A bientôt ;)\n");
			break;
		}
		printf("\n");
	}	
}



int main (int argc, char *argv[])
{
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

