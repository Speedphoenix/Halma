#include "Sous_programmes.h"

//sous programme pour trouver le pion d'un joueur à partir de ses coordonnées renvoie -1 s'il n'y a pas dez pion
int quel_pion(int x, int y, int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n])
{///si-victoire ; choix_deplacement
    int k, rep;

    rep = -1; //s'il y a pas de pion on renvoie -1
    k = joueur-1;

    for (int i=0;i<n;i++) //pour chaque pion
    {
        if (xpions[k][i]==x) //si le pion est à l'abscisse cherchée
        {
            if (ypions[k][i]==y) //si le pion est à l'ordonnée cherchée
            {
                rep = i;
            }
        }
    }
    return rep;
}

//sous programme pour trouver s'il y a un pion à partir des coordonnées: revoie 0 si'il n'y en a pas, positif s'il y a un pion ou si c'est à l'exterieur du damier
int si_pion(int x, int y, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n])
{///saut ; pion_possible ; mouvement
    int rep;
    rep = 0; //renvoie 0 s'il y en a pas

    if (x<1 || x>N || y<1 || y>N) //si c'est à l'exterieur du damier on renvoie 1
    {
        rep++;
    }
    else
    {
        for (int k=0;k<nb_j;k++) //pour chaque joueur
        {
            for(int i=0;i<n;i++) //pour chaque pion
            {
                if (xpions[k][i]==x) //si le pion est à l'abscisse cherchée
                {
                    if (ypions[k][i]==y) //si le pion est à l'ordonnée cherchée
                    {
                        rep++;
                    }
                }
            }
        }
    }
    return rep;
}

//donne si un saut est possible ou non :2 si c'est un saut, sinon 0
int saut(int x1, int y1, int x2, int y2, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///si_pion
{///mouvement
    int possible = 0, dx, dy, mx, my, s;

    dx = x1-x2; //prendre la difference de coordonnées
    dy = y1-y2;


    if ((dx==0 && dy==0) || dx==-1 || dx==1 || dy==1 || dy==-1) //si on fait pas un déplacement de 2 dans une direction
    {
        possible = 0; //pas possible
    }
    else if ((dx==2 || dx==-2 || dx==0) && (dy==0 || dy==-2 || dy==2)) //si on fait un déplacement de 2 dans une direction
    {
        mx = (x1+x2)/2; //prendre la position entre le départ et l'arrivée
        my = (y1+y2)/2;

        s = si_pion(mx, my, nb_j, n, xpions, ypions); //y a-t-il un pion entre la case de départ et celle d'arrivée?

        if (si_pion(x2, y2, nb_j, n, xpions, ypions)) //s'il y a un pion aux coordonnées d'arrivée
        {
            possible = 0; //pas possible
        }
        else if (s) //si non, s'il y a un pion au dessus duquel sauter
        {
            possible = 2; //possible
        }
        else //si non
        {
            possible = 0; //pas possible
        }
    }
    return possible;
}

// si un pion peut etre déplaçable renvoie une valeur positive sinon 0
int pion_possible(int choix, int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///si_pion
{///choix_deplacement
    int i, j, res, x, y = 0;

    res = 0;

    x = xpions[joueur-1][choix]; //pour ne pas avoir à répeter ça à chaque fois
    y = ypions[joueur-1][choix];

    //si un pas est possible
    for (i=-1;i<=1;i++) //pour chaque endroit directement autours du pion
    {
        for (j=-1;j<=1;j++)
        {
            if (si_pion(x+i, y+j, nb_j, n, xpions, ypions)==0) //s'il y a pas de pion là
            {
                res++;
            }
        }
    }

    //si un saut est possible
    if (res==0) //s'il y a des pions tout autours alors on a pas besoin de verifier qu'il y en ait un entre la case de départ et d'arrivée
    {
        for (i=-2;i<=2;i+=2)
        {
            for (j=-2;j<=2;j+=2)
            {
                if (si_pion(x+i, y+j, nb_j, n, xpions, ypions)==0) //s'il n'y a pas de pion là
                {
                    res++;
                }
            }
        }
    }

    return res;
}

//fin du fichier
