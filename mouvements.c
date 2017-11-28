#include "Sous_programmes.h"

//pour passer d'un nombre entre 0 et 255 à deux coordonnées
void hex_coord(int *x, int *y, int hex)
{
    *y = hex%N;
    *x = (hex-(*y))/N;
}

//pour passer de deux coordonnées à un nombre entre 0 et 255
int coord_hex(int x, int y)
{
    int hex;
    hex = (N*x)+y;
    return hex;
}

//pour initialiser un tableau à la valeur zero
void zero_tab(int tab[N][N], int zero)
{
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<N;j++)
        {
            tab[i][j] = zero;
        }
    }
}

//pur DRY, remplacer x et y par les coordonnées d'un pion
void coordonnees_dry(int *x, int *y, int joueur, int choix, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///x/ypions ; nb_j
{///mouvements ; choix_deplacement
    *x = xpions[joueur-1][choix];
    *y = ypions[joueur-1][choix];
}

//pur DRY, si chemin!=NULL on stocke dedans x et y
void sauv_chemin(int chemin[N][N], int x0, int y0, int x, int y)
{
    if (chemin!=NULL) //chemin n'existe que dans l'utilisation de l'IA
    {
        chemin[x0][y0] = coord_hex(x, y);
    }
}

//pour voir quelles cases peuvent avoir un saut, DRY pour ne pas à avoir à écrire tout ça dans le sous programme principal
int autre_saut(int *x,int *y,int deplacements[N][N],int deja[N][N])
{
    int res;
    res = 0;

    //on regarde chaque case, si on peut y arriver avec un saut, et si ce test a déjà été fait
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<N;j++)
        {
            if (deplacements[i][j]==2 && deja[i][j]==0)
            {
                *x = i+1; //on stocke les coordonnées du saut multiple possible
                *y = j+1;
                res = 1;
            }
        }
    }

    return res; //on renvoie si il y a une possibilité ou pas
}

//change les valeurs d'un tableau: 0 si le pion ne peut pas y aller, 1 s'il peut y aller avec un pas, 2 s'il peut y aller avec un ou plusieurs sauts
void mouvements(int deplacements[N][N], int joueur, int choix, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int chemin[N][N]) ///coordonnees_dry ; si_pion ; saut ;
{///afficher_deplacements ; choix_deplacement ; les IA
    int i,j, x, y, res;
    int deja[N][N]; //pour voir si un saut a déjà été fait à partir de là

    res = 0;

    zero_tab(deplacements, 0);
    zero_tab(deja, 0);

    if (chemin!=NULL) //si chemin existe (que pour les IA)
    {
        zero_tab(chemin, -1);
    }

    coordonnees_dry(&x, &y, joueur, choix, nb_j, n, xpions, ypions); //pour travailler avec x et y au lieu de x/ypions[joueur-1][choix]

    if (chemin!=NULL)
    {
        chemin[x-1][y-1] = coord_hex(x-1, y-1); //la case de départ se pointe elle même
    }


    //si un pas est  possible
    for (i=-1;i<=1;i++)
    {
        for (j=-1;j<=1;j++)
        {
            if (si_pion(x+i, y+j, nb_j, n, xpions, ypions)==0) //si y'a pas de pion alors le pas est possible
            {
                deplacements[x+i-1][y+j-1] = 1; //on peut

                sauv_chemin(chemin, x+i-1, y+j-1, x-1, y-1); //on garde le chemin (si chemin!=NULL)
            }
        }
    }

    //si un saut est possible

    for (i=-2;i<=2;i+=2)
    {
        for (j=-2;j<=2;j+=2)
        {
            if (saut(x, y, x+i, y+j, nb_j, n, xpions, ypions)==2)
            {
                deplacements[x+i-1][y+j-1] = 2; //c'est possible

                sauv_chemin(chemin, x+i-1, y+j-1, x-1, y-1); //on garde le chemin

                res = 2;
            }
        }
    }

    if (res) //pas besion d'ajouter quoi que ce soit si aucun saut n'est possible
    {
        deja[x-1][y-1] = 1; //on a déjà fait un saut à partir du pion de départ
        res = autre_saut(&x, &y, deplacements, deja); //on prend les coordonnées d'un endroit d'où on peut faire un autre saut
        while (res) //tant qu'il y a des sauts possibles
        {
            for (i=-2;i<=2;i+=2) //on regarde si un saut est possible...
            {
                for (j=-2;j<=2;j+=2)
                {
                    if (saut(x, y, x+i, y+j, nb_j, n, xpions, ypions)==2)
                    {
                        deplacements[x+i-1][y+j-1] = 2; //le deplacemnt est possible

                        if (chemin!=NULL) //chemin n'existe que pour les IA
                        {
                            if (chemin[x+i-1][y+j-1]==-1) //pour ne pas écraser le chemin précédent (et faire une récursion infinie)
                            {
                                sauv_chemin(chemin, x+i-1, y+j-1, x-1, y-1); //on garde le chemin
                            }
                        }
                    }
                }
            }
            res = autre_saut(&x, &y, deplacements, deja); //on regarde s'il reste des sauts multiples à faire
            deja[x-1][y-1] = 1; //on a déjà fait des sauts multiples à partir de là
        }
    }
}

//fin du fichier
