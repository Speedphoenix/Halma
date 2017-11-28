#include "Sous_programmes.h"

//sous programme pour l'emplacement du curseur
void gotoligcol(int y,int x) //merci aux professeurs d'info pour ce sous programme
{
    printf("\033[%d;%dH", (y), (x));
}

//affiche les pions
void afficher_pions(char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///x/ypions
{///afficher_damier
    for (int i=0;i<nb_j;i++) //pour chaque joueur
    {
        for (int j=0;j<n;j++) //pour chaque pion
        {
            gotoligcol(50-(3*ypions[i][j]), 6*xpions[i][j]-3); // simplifié: (47-(((3*ypions[i][j])-3)+1),((6*xpions[i][j])-6)+3)
            printf("%s%c%c%s",couleurp[i], pion1, pion2, RESET); //pion1 est le crochet ouvrant, pion2 le fermant
        }
    }

    gotoligcol(50,0); //pour ne pas que le curseur soit aux coordonnées d'un pion / tout en bas
}

//affiche le damier
void afficher_damier(char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///afficher_pions
{///afficher_deplacements ; choix_deplacement
    int i, j;
    char c;

    c=blanc;

    effacer_tout(); //vider l'écran
    gotoligcol(1, 1);

    //Les cases du damier sont 6 caractères en abscisse et 3 en ordonnée
    for(i=0;i<48;i++) //48 = 3*16
    {
        if((i%3)==0) //toutes les 3 lignes faut changer de couleur (pour cadrier)
        {
            if(c==blanc)
                c=noir;
            else
                c=blanc;
        }

        for(j=0;j<96;j++) //96 = 6*16
        {
            if((j%6)==0) //toutes les 6 colonnes faut changer le couleur (nouveau pions)
            {
                if(c==blanc)
                    c=noir;
                else
                    c=blanc;
            }

            printf("%c", c); //faut bien écrire la case non?
        }
        printf("\n");
    }

    printf(MAG); //pour écrire l'abscisse en bas
    for (i=1;i<=N;i++)
    {
        gotoligcol(49, 6*i-3);
        printf("%d",i);
    }
    printf(CYN); //pour écrire l'ordonnée à droite
    for (j=1;j<=N;j++)
    {
        gotoligcol(50-(3*j), 97);
        printf("%d",j);
    }
    printf(RESET); //remettre la couleur à 0

    afficher_maisons(nb_j, n, couleurp);

    afficher_pions(couleurp, nb_j, n, xpions, ypions);
}

//pour que le pion soit visible (selection)
void surligner_pion(int joueur, char *couleurp[4], int x, int y) ///nb_j ; x/ypions
{///afficher_deplacements

    gotoligcol(50-(3*y),6*x-4);
    printf("%s%c%s",couleurp[joueur-1], 123, RESET);
    gotoligcol(50-(3*y),6*x-1);
    printf("%s%c%s",couleurp[joueur-1], 125, RESET);

    gotoligcol(51-(3*y),6*x-3);
    printf("%s%c%s",couleurp[joueur-1], s_ligne, RESET);
    gotoligcol(51-(3*y),6*x-2);
    printf("%s%c%s",couleurp[joueur-1], s_ligne, RESET);

    gotoligcol(49-(3*y),6*x-3);
    printf("%s%c%s",couleurp[joueur-1], s_ligne, RESET);
    gotoligcol(49-(3*y),6*x-2);
    printf("%s%c%s",couleurp[joueur-1], s_ligne, RESET);
}

//pour afficher à quels endroits un pion peut aller
void afficher_deplacements(int joueur, int choix, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) /// afficher_damier ; surligner_pion ; mouvements
{/// choix_deplacement
    int deplacements[N][N]; //va contenir les déplacements possibles

    afficher_damier(couleurp, nb_j, n, xpions, ypions);

    surligner_pion(joueur, couleurp, xpions[joueur-1][choix], ypions[joueur-1][choix]);

    mouvements(deplacements, joueur, choix, nb_j, n, xpions, ypions, NULL);

    for (int i=0;i<N;i++) //on regarde pour chaque abscisse
    {
        for (int j=0;j<N;j++) // pour chaque ordonnée
        {
            if (deplacements[i][j]) //si on peut y arriver
            {
                gotoligcol(50-(3*(j+1)),6*(i+1)-3);
                printf("%s%c%c%s",CYN, pion3, pion3, RESET);
            }
        }
    }

    gotoligcol(50,0);
}

void surligner_menu(int y0, int x, int y)
{
    //pour effacer le curseur précédent
    gotoligcol(y0+1, x);
    printf("%c", noir);
    gotoligcol(y0-1, x);
    printf("%c", noir);

    gotoligcol(y0+1, x-1);
    printf("%c", noir);
    gotoligcol(y0-1, x-1);
    printf("%c", noir);

    gotoligcol(y0, x+1);
    printf("%c", noir);
    gotoligcol(y0, x-2);
    printf("%c", noir);


    printf(CYN);

    //pour mettre le nouveau curseur
    gotoligcol(y+1, x);
    printf("%c", s_ligne);
    gotoligcol(y-1, x);
    printf("%c", s_ligne);

    gotoligcol(y+1, x-1);
    printf("%c", s_ligne);
    gotoligcol(y-1, x-1);
    printf("%c", s_ligne);

    gotoligcol(y, x+1);
    printf("%c", 62);
    gotoligcol(y, x-2);
    printf("%c", 60);

    printf(RESET); //pour remettre un couleur normale
}

//pour déplacer avec les flèches: x0 et y0: là où y'avais le curseur précédent à effacer
void afficher_curseur(int joueur, int x0, int y0, int x, int y, char *couleurp[4])
{
    char c;
    if (x0%2==y0%2) //pour voir si la case est blanche ou noire
    {
        c = noir;
    }
    else c = blanc;

    //pour enlever l'ancien curseur
    gotoligcol(51-(3*y0),6*x0-5);
    printf("%c",c);
    gotoligcol(49-(3*y0),6*x0-5);
    printf("%c",c);

    gotoligcol(51-(3*y0),6*x0);
    printf("%c",c);
    gotoligcol(49-(3*y0),6*x0);
    printf("%c",c);


    //affichage du curseur
    gotoligcol(51-(3*y),6*x-5);
    printf("%s%c%s",couleurp[joueur-1], curseur, RESET);
    gotoligcol(49-(3*y),6*x-5);
    printf("%s%c%s",couleurp[joueur-1], curseur, RESET);

    gotoligcol(51-(3*y),6*x);
    printf("%s%c%s",couleurp[joueur-1], curseur, RESET);
    gotoligcol(49-(3*y),6*x);
    printf("%s%c%s",couleurp[joueur-1], curseur, RESET);

}

//pour afficher la boite de pause
void afficher_boite(void)
{
    for (int j=16;j<31;j++) //on fait une boite noire de 36 par 15
    {
        for (int i=31;i<67;i++)
        {
            gotoligcol(j, i);
            printf("%c", noir);
        }
    }
}

//pour que les maisons soient spéciales
void afficher_maisons(int nb_j, int n, char *couleurp[4])
{
    int x, y;
    for (int i=0;i<nb_j;i++) //pour chaque maison
    {
        for (int j=0;j<n;j++) //pour chaque case de la maison
        {
            x = xmaison(i, j, nb_j); //pour ne pas avoir à répéter à chaque fois
            y = ymaison(i, j, nb_j);

            gotoligcol(51-(3*y),6*x-4);
            printf("%s%c%s",couleurp[i], maison, RESET);
            gotoligcol(51-(3*y),6*x-1);
            printf("%s%c%s",couleurp[i], maison, RESET);

            gotoligcol(49-(3*y),6*x-4);
            printf("%s%c%s",couleurp[i], maison, RESET);
            gotoligcol(49-(3*y),6*x-1);
            printf("%s%c%s",couleurp[i], maison, RESET);
        }
    }
}

//pour faire clignoter le pion utilisé par l'IA il doit déjà y avoir un pion, il en aura plus après (case vide)
void clignoter(int joueur, int x, int y, int chemin[N][N], char *couleurp[4], int nombre, int fois)
{
    int x0, y0;
    char c;

    hex_coord(&x0, &y0, chemin[x-1][y-1]); //pour que x0 et y0 contiennent les coordonnées contenues dans chemin[x-1][y-1]

    x0++; //pour passer de coordonnées de (0 à 15) à (1 à 16)
    y0++;

    if (fois>255) //vu que c'est une fonction récursive, y'a la possibilité qu'elle tourne à l'infinit, et on n'aime pas ça
    {
        exit(EXIT_FAILURE);
    }

    fois++; //la fonction a été appellée une fois de plus

    if (x!=x0 && y!=y0) //si on est pas déjà arrivé à la fin de la récursivité
    {
        clignoter(joueur, x0, y0, chemin, couleurp, nombre, fois); //on l'appelle avec les coordonnées  pointées
    }
    else
    {
        nombre = nombre*2;
    }

    if (x%2==y%2) //pour avoir la bonne couleur de la case
    {
        c = noir;
    }
    else c = blanc;

    for (int i=0;i<nombre;i++) //faut faire clignoter 2 ou 4 fois
    {
        fflush(stdout); //est necessaire: sinon on saute le clignotement (a à voir avec l'attente)

        gotoligcol(50-(3*y),6*x-3); //effacer le pion
        printf("%c%c", c, c);

        usleep(200000); //pour l'attente entre les clignes, est en nanosecondes

        fflush(stdout); //necessaire

        gotoligcol(50-(3*y),6*x-3); //re-afficher le pion
        printf("%s%c%c%s",couleurp[joueur-1], pion1, pion2, RESET);

        usleep(200000); //pour l'attente entre les clignes, est en nanosecondes
    }

    fflush(stdout); //necessaire

    gotoligcol(50-(3*y),6*x-3);
    printf("%c%c",c, c);
}

//pour effacer l'écran
void effacer_tout(void)
{
    for (int i=0;i<55;i++)
    {
        gotoligcol(i, 0);
        for (int j=0;j<200;j++)
        {
            printf("%c", noir);
        }
    }
}

//fin du fichier
