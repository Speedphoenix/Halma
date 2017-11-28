#include "Sous_programmes.h"

//definit le coin du damier en fonction du joueur (abscisse)
int cotex(int i, int x, int nb_j)
{
    int r = 0;

    if (nb_j==2) //si il y a deux joueurs le joueur 2 est dans le coin opposé
    {
        if (i==0)
        {
            r = x;
        }
        if (i==1)
        {
            r = (N+1)-x;
        }
    }
    else if (nb_j==4) //si il y a 4 joueurs chaque coin peut différer
    {
        if (i==0 || i==3)
        {
            r = x;
        }
        if (i==1 || i==2)
        {
            r = (N+1)-x;
        }
    }

    return r;
}

//definit le coin du damier en fonction du joueur (ordonnée)
int cotey(int i, int y, int nb_j)
{
    int r = 0;

    if (nb_j==2) //si il y a deux joueurs le joueur 2 est dans le coin opposé
    {
        if (i==0)
        {
            r = y;
        }
        if (i==1)
        {
            r = (N+1)-y;
        }
    }
    else if (nb_j==4) //si il y a 4 joueurs chaque coin peut différer
    {
        if (i==0 || i==1)
        {
            r = y;
        }
        if (i==2 || i==3)
        {
            r = (N+1)-y;
        }
    }

    return r;
}

//entre le numéro d'un pion et renvoie les coordonnées initiales
int xmaison(int i, int pion, int nb_j)
{
    //i est le numéro du joueur (de 0 à 1 ou 3)
    int x;
    switch (pion) // contient les abscisses de départ
    {
        case 0:
        x = 1;
        break;
        case 1:
        x = 2;
        break;
        case 2:
        x = 1;
        break;
        case 3:
        x = 3;
        break;
        case 4:
        x = 1;
        break;
        case 5:
        x = 2;
        break;
        case 6:
        x = 4;
        break;
        case 7:
        x = 1;
        break;
        case 8:
        x = 3;
        break;
        case 9:
        x = 2;
        break;
        case 10:
        x = 3;
        break;
        case 11:
        x = 2;
        break;
        case 12:
        x = 4;
        break;
        case 13:
        x = 3;
        break;
        case 14:
        x = 4;
        break;
        case 15:
        x = 5;
        break;
        case 16:
        x = 1;
        break;
        case 17:
        x = 2;
        break;
        case 18:
        x = 5;
        break;
        default:
        x = 0;
        break;
    }

    x = cotex(i, x, nb_j); //pour l'avoir du bon coté

    return x;
}

//entre le numéro d'un pion et renvoie les coordonnées initiales
int ymaison(int i, int pion, int nb_j)
{
    //i est le numéro du joueur (de 0 à 1 ou 3)
    int y;
    switch (pion) // contient les ordonnées de départ
    {
        case 0:
        y = 1;
        break;
        case 1:
        y = 1;
        break;
        case 2:
        y = 2;
        break;
        case 3:
        y = 1;
        break;
        case 4:
        y = 3;
        break;
        case 5:
        y = 2;
        break;
        case 6:
        y = 1;
        break;
        case 7:
        y = 4;
        break;
        case 8:
        y = 2;
        break;
        case 9:
        y = 3;
        break;
        case 10:
        y = 3;
        break;
        case 11:
        y = 4;
        break;
        case 12:
        y = 2;
        break;
        case 13:
        y = 4;
        break;
        case 14:
        y = 3;
        break;
        case 15:
        y = 1;
        break;
        case 16:
        y = 5;
        break;
        case 17:
        y = 5;
        break;
        case 18:
        y = 2;
        break;
        default:
        y = 0;
        break;
    }

    y = cotey(i, y, nb_j); //pour l'avoir du bon coté

    return y;
}

//pour mettre chaque pion à sa position initiale
void reset_pions(int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n])
{
    for (int i=0;i<nb_j;i++) //pour chaque joueur
    {
        for (int j=0;j<n;j++) //pour chaque pion
        {
            xpions[i][j] = xmaison(i, j, nb_j); //on met la position initiale dans la maison
            ypions[i][j] = ymaison(i, j, nb_j);
        }
    }
}

//fin du fichier
