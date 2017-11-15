#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

// Cette fonction permet de lire deux fichiers passés en paramètre au "main" (que l'on repasse en paramètre à cette fonction) et de mettre leur contenu dans deux tableaux.
int LectureFichiers(char *FichierGrilleMots, char *FichierListeMots);

// Cette fonction permet d'afficher une grille de mots. L'affichage comprend également les coordoonées x et y.
void AffichageGrille(char **Grille);

// Cette fonction permet d'afficher une liste de mots.
void AffichageMots(char **ListeMots);

/* 
Cette fonction permet de vérifier si un mot (le mot d'indice iMot de la liste de mots) se trouve aux coordonnées x et y de la grille de mots.
  Si SelonX = 1 et SelonY = 0, on effectue la vérification sur la ligne de gauche à droite.
  Si SelonX = -1 0 et SelonY = 0, on effectue la vérification sur la ligne de droite à gauche.
  Si SelonX = 0 et SelonY = 1, on effectue la vérification sur la colonne de haut à bas.
  Si SelonX = 0 et SelonY = -1, on effectue la vérification sur la colonne de bas à haut.
  Si SelonX = 1 et SelonY = 1, on effectue la vérification sur la diagonale de droite de haut en bas.
  Si SelonX = 1 et SelonY = -1, on effectue la vérification sur la diagonale de gauche de bas en haut.
  Si SelonX = -1 et SelonY = -1, on effectue la vérification sur diagonale de droite de bas en haut.
  Si SelonX = -1 et SelonY = 1, on effectue la vérification sur la diagonale de gauche de haut en bas.
*/
int VerificationMot(int x, int y, int SelonX, int SelonY, char **GrilleMots, char **GrilleMotsBarres, int iMot);

// L'appel à cette fonction lance le jeu dans lequel l'utilisateur va joueur.
void JeuJoueur();

// L'appel à cette fonction lance le jeu dans lequel l'ordinateur va jouer seul.
void JeuOrdi();

// Cette fonction va lancer un menu qui va permettre à l'utilisateur de choisir s'il veut jouer ou s'il veut laisser l'ordinateur jouer.
void Interface(char *FichierGrilleMots, char *FichierListeMots);


