
#ifndef SOUS_PROGRAMMES_H_INCLUDED
#define SOUS_PROGRAMMES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <termios.h> // pour getch et kbhit

#include <unistd.h> //pour kbhit
#include <fcntl.h>

#define N 16 //la taille d'un côté de tableau/damier 16*16
#define CLIGNE 2 //le nombre de clignotements à chaque fois (fois deux pour la case de départ)

#define noir 32
#define blanc 124 //219 sur windows, soit 47 soit 124 sur linux
#define pion1 91 //91   60  64  123
#define pion2 93 //93   62      125
#define pion3 47 // pour les possibilités de déplacement
#define curseur 126
#define s_ligne 45
#define maison 34 //34 42

#define label1 91 // pour fic_goto
#define label2 93
#define label3 40
#define label4 125

#define JOUEUR 0 //pour les valeurs d'IA
#define FACILE 1
#define MIRROIR 2

/// pris de http://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

///sous programmes du menu///

//pour choisir la langue
int debut(int *langue);

//presentation des regles/des touches à utiliser
void presentation(int langue);

//choix du nobre de joueur/ charger la partie enregistrée
void menu(int langue, int *nb_j, int *n, int *joueur, int xpions2[2][19], int ypions2[2][19], int xpions4[4][13], int ypions4[4][13], int intel2[2], int intel4[4]);


///sous programmes pour l'emplacement initial des pions///

//sous programmes pour que la maison soit du bon côté
int cotex(int i, int x, int nb_j);
int cotey(int i, int y, int nb_j);

//entre le numéro d'un pion et renvoie les coordonnées initiales
int xmaison(int i, int pion, int nb_j);
int ymaison(int i, int pion, int nb_j);

//pour la position initiale des pions
void reset_pions(int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

///sous programmes de blindage///

//sous programme pour trouver le pion d'un joueur à partir de ses coordonnées renvoie -1 s'il n'y a pas dez pion
int quel_pion(int x, int y, int n, int joueur, int nb_j, int xpions[nb_j][n], int ypions[nb_j][n]);

//sous programme pour trouver s'il y a un pion à partir des coordonnées: revoie 0 si'il n'y en a pas, 1 s'il y a un pion ou si c'est à l'exterieur du damier
int si_pion(int x, int y, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

//donne si un saut est possible ou non :2 si c'est un saut, 0 sinon
int saut(int x1, int y1, int x2, int y2, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

// si un pion peut etre déplaçable renvoie une valeur positive sinon 0
int pion_possible(int choix, int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);


///sous programmes d'affichage///

//sous programme pour l'emplacement du curseur
void gotoligcol(int y,int x);

//affiche les pions
void afficher_pions(char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

//affiche le damier
void afficher_damier(char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

//pour afficher à quels endroits un pion peut aller
void afficher_deplacements(int joueur, int choix, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

//pour déplacer avec les flèches: x0 et y0: là où y'avais le curseur précédent à effacer
void afficher_curseur(int joueur, int x0, int y0, int x, int y, char *couleurp[4]);

//pour que le pion soit visible (selection)
void surligner_pion(int joueur, char *couleurp[4], int x, int y);

//pour montrer où est la selection de l'utilisateur dans le menu
void surligner_menu(int y0, int x, int y);

//pour afficher la boite de pause
void afficher_boite(void);

//pour que les maisons soient spéciales
void afficher_maisons(int nb_j, int n, char *couleurp[4]);

//pour faire clignoter le pion utilisé par l'IA il doit déjà y avoir un pion, il en aura toujours un après
void clignoter(int joueur, int x, int y, int chemin[N][N], char *couleurp[4], int nombre, int fois);

//pour vider l'écran
void effacer_tout(void);


///sous programmes qui donnent quoi écrire:
void texte(int num, int joueur, int langue, char *couleurp[4]);

//pour se rendre au bon label dans un fichier
int fic_goto(FILE *fic, int num, int type, int depart);


///sous programme de choix des déplacements

/* renvoie 1 si une touche a été appuyée, 0 sinon */ ///pris depuis campus.ece.fr MERCI À https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int kbhit(void);

/* reads from keypress, doesn't echo */ /// pris depuis http://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux
char getch(void);

// renvoie 1 si l'utilisateur a choisi vers le haut, 2 vers la gauche, 3 vers le bas, 4 vers la droite et 5 pour selectioner
int getkey(void);

//pour selectionner un pion et le déplacer avec le curseur
void choix_deplacement(int joueur, int *choix, int *xdestination, int *ydestination, int langue, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]);


///sous programmes pour faire tous les déplacements d'un coup
//pour initialiser un tableau
void zero_tab(int tab[N][N], int zero);

//pur DRY
void coordonnees_dry(int *x, int *y, int joueur, int choix, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]);

//change les valeurs d'un tableau: 0 si le pion ne peut pas y aller, 1 s'il peut y aller avec un pas, 2 s'il peut y aller avec un ou plusieurs sauts
void mouvements(int deplacements[N][N],int joueur, int choix, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int chemin[N][N]);

//pour passer d'un nombre entre 0 et 255 à deux coordonnées
void hex_coord(int *x, int *y, int hex);

//pour passer de deux coordonnées à un nombre entre 0 et 255
int coord_hex(int x, int y);

/// enregistrer la partie, mettre en pause la partie, tout ce qui est raport avec des fichiers séparés

//pour enregistrer une partie
int enregistrer_partie(int joueur, int nb_j, int n, int langue, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]);

//pour lire le nombre de joueurs et le tour (à quel joueur)
void ouvrir_partie1(int *joueur, int *nb_j, FILE* fic);

//pour lire la valeur d'IA de chaque joueur (joueur ou Niveau de l'ia) et les coordonnées de chaque pion
void ouvrir_partie2(int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j], FILE* fic);

//quand la partie est en pause
void pause_game(int joueur, int langue, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]);

//arrete le programme et met en pause si on a appuyé sur p, ne fait rien sinon
int pause_entre(int joueur, int langue, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]);

///intelligence artificielle

//donne le pion le plus proche de la base adverse
void avance_p(int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], float avancement[n]);

//IA mirroir (fait basique s'il n'y a pas de possibilités
void mirroir(int joueur, int nb_j, int n, int prev[4], int xpions[nb_j][n], int ypions[nb_j][n], int chemin[N][N]);

//IA basique
void facile(int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int prev[4], int chemin[N][N]);

//pour savoir dans quelle direction va un déplacement
int direction(int x1, int y1, int x2, int y2, int joueur, int nb_j);

//pour ne plus toucher aux pions qui sont dans la maison advesre
int fixe(int joueur, int nb_j, int n, int fix[n], int xpions[nb_j][n], int ypions[nb_j][n]);


#endif
