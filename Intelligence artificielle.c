#include "Sous_programmes.h"

//RESTE À BLINDER POUR NE PAS BLOQUER L'ENNEMI DANS SA MAISON
//POUR 2 JOUEURS QUE TOUS LES PIONS FINISSENT DANS LA MAISON, FIXER LES PIONS ARRIVÉS À LA MAISON ENNEMIE

//mets dans fix[n] les numéros des pions à ne pas boujer et renvoie le nombre de pions fixes
int fixe(int joueur, int nb_j, int n, int fix[n], int xpions[nb_j][n], int ypions[nb_j][n])
{
    int i, tant_q = 0, x, rep = -1; //le tant_q est nécessaire, j'arrive pas à faire une boucle qui peut s'arreter au premier tour sinon
    i = 0; //on doit faire un do__while, peut pas avec un for...

    for (int j=0;j<n;j++)   //pour mettre toutes les cases de fix à -1
    {                       //discutablement inutile
        fix[j] = -1;
    }

    do //on regarde chaque emplacement de la maison adverse jusqu'à ce qu'il n'y ait pllus de pion allié dedans
    {
        x = quel_pion(17-xmaison(joueur-1, i, nb_j), 17-ymaison(joueur-1, i, nb_j), joueur, nb_j, n, xpions, ypions);
        //maintenant x contient -1 s'il y a pas de pion, le numéo du pion si non

        if (x!=-1) //s'il y a un pion (du bon joueur)
        {
            fix[i] = x; //on y met le numéro du pion
        }
        if(x==-1 || i==n-1)
        {
            rep = i-1; //on garde le nombre de pions qui sont dans la maison ennemie (pour ne pas avoir après à parcourir l'integralité du tableau)
            tant_q = 1;
        }
        i++;
    } while(tant_q==0);
    return rep;
}

//renvoie 0 si le deplacement est vers l'avant, 1 s'il est sur le coté immédiat, 2 s'il est en arrière 3 s'il ne faut juste pas le faire
int direction(int x1, int y1, int x2, int y2, int joueur, int nb_j)
{
    int dx, dy, rep;

    x1 = cotex(joueur-1, x1, nb_j); //pour mettre tout en perspective du joueur 1 (en bas à droite)
    y1 = cotey(joueur-1, y1, nb_j);
    x2 = cotex(joueur-1, x2, nb_j);
    y2 = cotey(joueur-1, y2, nb_j);

    dx = x2-x1; //on pred la difference (vecteur)
    dy = y2-y1;

    //pour ne pas que les pions se bloquent aux bords des maisons
    if ((nb_j==2 && ((x2==16 && y2==11) || (x2==11 && y2==16))) || (nb_j==4 && ((x2==16 && y2==12) || (x2==12 && y2==16))))
    {
        rep = 3;
    }
    else if (x2>=x1 && y2>=y1) //si le déplacement est vers l'avant
    {
        rep = 0;
    }
    else if (((dx==2 || dx==1) && (dy==-2 || dy==-1)) || ((dx==-2 || dx==-1) && (dy==2 || dy==1))) //si le déplacement est sur le côté immédiat
    {
        if (nb_j==2 && ((x1==16 && y1==11) || (x1==11 && y1==16))) //pour débloquer du bords des maisons (discutablement inutile)
        {
            rep = 0;
        }
        else if(nb_j==4 && ((x1==16 && y1==12) || (x1==12 && y1==16))) //pour débloquer du bords des maisons (discutablement inutile)
        {
            rep = 0;
        }
        else //ce qui compte
        {
            rep = 1;
        }
    }
    else //vers l'arrière
    {
        rep = 2;
    }

    return rep;
}

//dettermine la distance de chaque pion à la maison adverse et le met dans avancement[n]
void avance_p(int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], float avancement[n])
{
    float x, y; //pour ne pas à avoir à répéter plusieures fois la racine

    for (int i=0;i<n;i++) //pour chaque pion
    {
        x = 17-cotex(joueur-1, xpions[joueur-1][i], nb_j); //en perspective la distance sur un axe à l'arrivée
        y = 17-cotey(joueur-1, ypions[joueur-1][i], nb_j);

        avancement[i] = sqrt(x*x+y*y); //racine de (a²+b²)
    }
}

//que pour un partie joueur vs IA
void mirroir(int joueur, int nb_j, int n, int prev[4], int xpions[nb_j][n], int ypions[nb_j][n], int chemin[N][N])
{
    //prev[4] contient les coordonnées du déplacement précédent (du joueur)
    int possible[N][N];
    int choix, defaud = 1;

    prev[0] = cotex(joueur-1, prev[0], nb_j); //on met tout du côté de l'IA
    prev[1] = cotey(joueur-1, prev[1], nb_j);
    prev[2] = cotex(joueur-1, prev[2], nb_j);
    prev[3] = cotey(joueur-1, prev[3], nb_j);

    choix = quel_pion(prev[0], prev[1], joueur, nb_j, n, xpions, ypions); //on regarde le pion qu'y si trouve

    if (choix!=-1) //s'il y a un pion
    {
        mouvements(possible, joueur, choix, nb_j, n, xpions, ypions, chemin); //on regarde tous les mouvements possibles

        if (possible[prev[2]-1][prev[3]-1]) //si le déplacement mirroir est possible
        {
            xpions[joueur-1][choix] = prev[2];
            ypions[joueur-1][choix] = prev[3];

            defaud = 0;
        }
    }
    if (defaud) //si le déplacement mirroir n'était pas possible on passe au plan b (ia basique)
    {
        facile(joueur, nb_j, n, xpions, ypions, prev, chemin);
    }
}

//RESTE À BLINDER POUR NE PAS BLOQUER L'ENNEMI DANS SA BASE
//reste à faire en sorte que pour une même case d'arrivée le pion le plus reculé soit pris
//pour l'intelligence artificielle basique
void facile(int joueur, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int prev[4], int chemin[N][N])
{
    float avancement[n];
    int arrivee[N][N], deplacement[N][N], direc[N][N], relatif[N][N], fix[n];
    int i, j, k, v, bestx, besty, choix, f, bouj; //i, j, k servent pour les boulcles for: k le numero du pion, i et j l'abscisse et l'ordonnée
    //f et bouj servent pour fixer les pions déjà dans la base adverse

    int chemin_t[n][N][N]; //on enregistre les chemin de chaque pion

    zero_tab(arrivee, -1); //initialiser les différents tableaux
    zero_tab(direc, 4);
    zero_tab(relatif, -1);

    avance_p(joueur, nb_j, n, xpions, ypions, avancement); // maintenant chaque case de avancement a la distance à la maison adverse

    f = fixe(joueur, nb_j, n, fix, xpions, ypions); //f contient le nombre de pions dans la base ennemie

    for (k=0;k<n;k++) //pour chaque pion
    {
        bouj = 1;
//        if (f!=-1) //si il y a des pions au bon endroit dans la base ennemie
//        {
//            for(i=0;i<=f+1;i++) //on regaerde pour chaque emplacement de la maison si le pion k y est
//            {
//                if (fix[i]==k)
//                bouj = 0;
//            }
//        }
        if (bouj) //si le pion n'est pas fixé dans la base ennemie
        {
            mouvements(deplacement, joueur, k, nb_j, n, xpions, ypions, chemin_t[k]); //on regarde tous les depllacements possibles du pion
            for (i=0;i<N;i++) //pour chaque coordonnée
            {
                for (j=0;j<N; j++)
                {
                    if (deplacement[i][j] != 0) // si le pion peut y arriver
                    {
                        v = direction(xpions[joueur-1][k], ypions[joueur-1][k], i+1, j+1, joueur, nb_j); //on regarde dans quelle direction le pion irait

                        if (v<direc[i][j]) //si le déplacement précédent (un autre pion) est moins bon, alors on remplace par le nouveau pionn
                        {//                 à noter que le le tableau a été initialisé à 4 partout, et que le pire des déplacement vaut 3
                            arrivee[i][j] = k;
                            direc[i][j] = v;
                        }
                    }
                }
            }
        }
    }

    //maintenant:
    //les cases de arrivee contiennent le numéro du meilleur pion qui peut y arriver
    //les cases de direc contiennent la direction dans laquelle se déplace la pion (3 s'il y a pas de pions pour y aller, 2 si c'est en arriere, 1 sur le cote (immediat) 0si c'est en avant)


    //pour que les coordonnées soient relativent au joueur 1
    for (i=0;i<N;i++)
    {
        for (j=0;j<N;j++)
        {
            relatif[cotex(joueur-1, i+1, nb_j)-1][cotey(joueur-1, j+1, nb_j)-1] = arrivee[i][j];
        }
    }

    bestx = 0; ///
    besty = 0; ///le problème est (peut-être) là
    choix = 0; ///

    //choix de l'emplacement d'arrivée
    for (i=0;i<N;i++)
    {
        for (j=0;j<N;j++) //pour chaque coordonnée
        {
            if (relatif[i][j]!=-1) //si un pion peut y arrier
            {
                if (direc[cotex(joueur-1, bestx+1, nb_j)-1][cotey(joueur-1, besty+1, nb_j)-1]>=direc[cotex(joueur-1, i+1, nb_j)-1][cotey(joueur-1, j+1, nb_j)-1])
                { //si la nouvelle direction est meilleure

                    if ((i+j)>(bestx+besty)) //pour voir si ce déplacement est meilleur que le précédent
                    {
                        bestx = i; //on remplace par le nouveau
                        besty = j;
                    }
                }
            }
        }
    }

    choix = relatif[bestx][besty];

    bestx = cotex(joueur-1, bestx+1, nb_j);
    besty = cotey(joueur-1, besty+1, nb_j);

    for (i=0;i<N;i++) //pour chaque case de chemin
    {
        for (j=0;j<N;j++) //on garde le chemin du bon pion
        {
            chemin[i][j] = chemin_t[choix][i][j];
        }
    }

    prev[0] = xpions[joueur-1][choix]; //on garde les coordonnées de départ et d'arrivée
    prev[1] = ypions[joueur-1][choix];
    prev[2] = bestx;
    prev[3] = besty;

    xpions[joueur-1][choix] = bestx; //on remplace les coordonnées des pions par les nouvelles
    ypions[joueur-1][choix] = besty;

}

// fin du fichier
