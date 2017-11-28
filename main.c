#include "Sous_programmes.h"

// cd Documents/Programmes/C/Halma_Linux/bin/Release/
// . /Halma_Linux

///             ***INTELLIGENCE ARTIFICIELLE***
///nombre magique

//sous programme de verification de victoire, renvoie le numéro du joueur qui a gagné (0 si personne n'a encore gagné)
int si_victoire(int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n]) ///nb_j ; x/ymaison ; quel_pion
{
    int gagnant = 0, quant; //gagnant est le return, quant est le compte de pions qui sont dans la maison adverse

    for (int i=0;i<nb_j;i++) //test pour chaque joueur
    {
        quant = 0;
        for (int j=0;j<n;j++) // test de chaque case d'arrivée
        {
            if (quel_pion(17-xmaison(i, j, nb_j), 17-ymaison(i, j, nb_j), i+1, nb_j, n, xpions, ypions)!=-1) // s'il y a un pion du joueur dans chaque case
            {
                quant++;
            }
        }
        if (quant==n) //si tous les pions sont dans la maison adverse
        {
            gagnant = i+1;
        }
    }

    return gagnant;
}

//sous programme du tour d'un joueur
void action_de_joueur(int joueur, int langue, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int sauv[4], int intel[nb_j]) /// afficher_damier ; choix_deplacement ; nouvelles_coordonnees
{
    int choix; //le numéro du pion selectioné
    int xdestination, ydestination; //les coordonnées finales du pion

    texte(9, joueur, langue, couleurp); //9) c'est au joueur x de jouer

    //choix du pion par le joueur et choix du déplacement
    choix_deplacement(joueur, &choix, &xdestination, &ydestination, langue, couleurp, nb_j, n, xpions, ypions, intel);
    //maintenant choix contient le numéro du pion selectioné et x/ydestination les coordonnées d'arrivée

    //pour renvoyer l'information sur le déplacement fait (pour l'IA mirroir) en perspective du joueur 1 (en bas à gauche)
    sauv[0] = cotex(joueur-1, xpions[joueur-1][choix], nb_j);
    sauv[1] = cotey(joueur-1, ypions[joueur-1][choix], nb_j);
    sauv[2] = cotex(joueur-1, xdestination, nb_j);
    sauv[3] = cotey(joueur-1, ydestination, nb_j);

    //changement des coordonnées du pion déplacé (faut bien non?)
    xpions[joueur-1][choix] = xdestination;
    ypions[joueur-1][choix] = ydestination;
}

//le jeu joueur contre joueur (contre joueur contre joueur)
int jeu(int joueur, int langue, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j], int skip)
{
    //intel contient pour chaque joueur si c'est un joueur ou une IA (et son niveau)
    //joueur contient le joueur initial (1 si on a pris une partie sauvegardée)

    int victoire = 0; //le return, contiendra le numéro duu joueur gagnant, 0 jusque là
    int prev[4], chemin[N][N]; //prev est pour garder les mouvement lors des tours (IA mirroir), chemin contient les coordonnées de plusieurs sauts (clignotement)

    afficher_damier(couleurp, nb_j, n, xpions, ypions); //affichage du damier initial

    while (victoire==0) // le jeu?
    {

        if (intel[joueur-1]==JOUEUR) //si c'est un humain
        {
            afficher_damier(couleurp, nb_j, n, xpions, ypions);//on affiche une première fois pour faire le choix

            action_de_joueur(joueur, langue, couleurp, nb_j, n, xpions, ypions, prev, intel);
        }

        else //si c'est une IA
        {
            //voit si l'utilisateur a appuyé sur p et met en pause si oui
            if (pause_entre(joueur, langue, nb_j, n, xpions, ypions, intel))
            afficher_damier(couleurp, nb_j, n, xpions, ypions); //pour effacer la boite de pause


            if (intel[joueur-1]==FACILE)
            {
                facile(joueur, nb_j, n, xpions, ypions, prev, chemin);
            }
            else if(intel[joueur-1]==MIRROIR)
            {
                mirroir(joueur, nb_j, n, prev, xpions, ypions, chemin);
            }

            if (skip!=1)
            {
                clignoter(joueur, prev[2], prev[3], chemin, couleurp, CLIGNE, 0); //pour faire clignoter le déplacement
            }

            //voit si l'utilisateur a appuyé sur p et met en pause si oui
            if (pause_entre(joueur, langue, nb_j, n, xpions, ypions, intel))
            afficher_damier(couleurp, nb_j, n, xpions, ypions); //pour effacer la boite de pause

        }

        afficher_damier(couleurp, nb_j, n, xpions, ypions); //affichage du damier après le tour

        texte(19, joueur, langue, couleurp); //19) ***FIN DU TOUR***

        usleep (500000); //pour mettre une petite pause entre les tours. Est en nanosecondes

        victoire = si_victoire(nb_j, n, xpions, ypions); //verification de la victoire

        if (joueur==nb_j) //pour passer au tour suivant
        {
            joueur = 1;
        }
        else if (joueur<nb_j)
        {
            joueur++;
        }

        fflush(stdout); //on sait jamais
    }
    return victoire;
}

//le main
int main()
{
    int victoire, joueur, partie, langue, n, nb_j, skip;
    char *couleurp[4] = {RED, BLU, GRN, YEL}; //contient les couleurs de chaque joueur

    int xpions2[2][19], ypions2[2][19], intel2[2] = {0}; //coordonnées des pions, niveau de l'ia/joueur
    int xpions4[4][13], ypions4[4][13], intel4[4] = {0};

    partie = 1;
    joueur = 0;
    skip = 0;

    skip = debut(&langue); //quelle langue, agrangir la fenêtre etc...

    do
    {
        victoire = 0; //on met victoire à 0 maintenant si jamais on rejoue

        if (skip!=1)
        {
            presentation(langue);// presentation, regles etc
            usleep(1000000);
        }

        usleep(1000000); //en nanosecondes

        menu(langue, &nb_j, &n, &joueur, xpions2, ypions2, xpions4, ypions4, intel2, intel4); //choix du nombre de joueurs et de la partie enregistrée


        ///DEBUT DU JEU
        if (nb_j==2) //jeu à 2 joueurs
        {
            victoire = jeu(joueur, langue, couleurp, nb_j, n, xpions2, ypions2, intel2, skip);
        }
        else if(nb_j==4) //jeu à 4 joueurs
        {
            victoire = jeu(joueur, langue, couleurp, nb_j, n, xpions4, ypions4, intel4, skip);
        }
        ///FIN DU JEU


        effacer_tout(); //pour "effacer" l'écran

        printf("\e[?25h"); //pour re-afficher le curseur
        fflush(stdout); //necessaire

        gotoligcol(0,0);
        texte(5, victoire, langue, couleurp); //5) Le joueur x a gagné

        texte(6, joueur, langue, NULL); //6) Voulez vous rejouer?


        fflush(stdin);
        scanf("%d",&partie); //savoir s'il faut rejouer?

    } while (partie);

    printf("\n\n\n");

    texte(7, joueur, langue, NULL); //7) merci à Artemii Lazovskii pour la traduction en russe

    return 0;
}

//fin du fichier
