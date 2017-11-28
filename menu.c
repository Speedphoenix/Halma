#include "Sous_programmes.h"

//Quelle langue, attension agrandir la fenetre
int debut(int *langue)
{
    int skip = 0; //skip si l'utilisateur demande au début, pour sauter les trucs longs

    texte(1, 0, 0, NULL); //41) Quelle langue?\n1: Français\n2: English\n3: Italiano\n4: Русский (credits to Artemii Lazovskii)\n0: Utiliser les valeurs par défaut et passer au choix du jeu

    fflush(stdin);
    scanf("%d", langue); //choix de la langue

    if (*langue<1 || *langue>4) //si il choisit pas une langue possible
    {
        if (*langue==0) //s'il demande à skip
        {
            skip = 1;
        }
        *langue = 1;
        texte(2, 0, 0, NULL); //42) La langue a été mise par défaut en français
    }

    texte(1, 0, *langue, NULL); //1) ATENTION! Agrandir la fenêtre! plein écran recommandé
    return skip; //on renvoie pour savoir s'il faut skipper d'autres trucs ou pas
}

//Voulez vous voir les règles du jeu? et les touches
void presentation(int langue)
{
    int regle;
    //menu
    texte(2, 0, langue, NULL); //2) Voulez-vous voir les règles du jeu? (0 - non / 1 - oui)

    fflush(stdin);
    scanf("%d",&regle);

    if (regle)
    {
        texte(3, 0, langue, NULL); //3) présentation et règles du jeu
    }

    texte(22, 0, langue, NULL); //22) Utilisez les flèches du clavier pour déplacer le curseur, ENTRÉE pour valider le choix et P pour mettre le jeu en pause
}

//choix du nombre de maisons
void choix_jeu1(int langue, int *nb_j)
{
    int key, position, y0, x;

    effacer_tout(); //pour vider l'écran

    x = 8; //on garde cette position

    gotoligcol(1, 2);
    texte(33, 0, langue, NULL); //33) Utilisez les flèches du clavier pour déplacer le curseur et ENTRÉE pour selectionner

    gotoligcol(6, x-6);
    texte(34, 0, langue, NULL); //34) Combien de maisons?

    gotoligcol(9, x);
    texte(35, 2, langue, NULL); //35) %d Maisons

    gotoligcol(11, x);
    texte(35, 4, langue, NULL); //35) %d Maisons

    y0 = position = 9;
    //y0 est la position précédente du curseur (à effacer)

    do //pour déplacer le curseur
    {
        surligner_menu(y0, x-2, position);
        y0 = position; //pour prendre la position précédente du curseur

        key = getkey(); //prise d'une touche du clavier

        if (key==1 && position!=9) //pour aller vers le haut
        {
            position -= 2;
        }
        else if (key==3 && position!=11) //pour aller vers le bas
        {
            position += 2;
        }
    }while (key!=5); //appuye sur enter

    if (position==9) //deux joueurs
    {
        *nb_j = 2;
    }
    else //quatre joueurs
    {
        *nb_j = 4;
    }
}

//choix de la valeur d'ia de chaque joueur
void choix_jeu2(int langue, int nb_j, int intel[nb_j])
{
    int key, position, y0, x, i;
    char *couleurp[4] = {RED, BLU, GRN, YEL};

    effacer_tout(); //pour vider l'écran

    x = 8; //on garde cette position

    gotoligcol(1, 2);
    texte(33, 0, langue, NULL); //33) Utilisez les flèches du clavier pour déplacer le curseur et ENTRÉE pour selectionner

    gotoligcol(6, x-6);
    texte(36, 0, langue, NULL); //36) Choisissez pour chaque maison si vous voullez une Intelligence Artificielle ou un Joueur

    for (i=0;i<nb_j;i++) //pour afficher la possibilité de chaque maison
    {
        gotoligcol(9+(2*i), x);
        texte(37, i+1, langue, couleurp); //37 Maison n°%d
        gotoligcol(9+(2*i), x-3);
        if(intel[i]==JOUEUR) //si c'est un joueur ou une IA (par défaud)
        {
            texte(38, i+1, langue, couleurp); //37) PL
        }
        else if(intel[i]==FACILE)
        {
            texte(39, i+1, langue, couleurp); //39) IA
        }
    }

    gotoligcol(9+(2*nb_j), x);
    texte(40, 0, langue, NULL); //40) Confirmer

    y0 = position = 9; //la position la plus haute

    do //tant qu'il n'est pas sur "continuer"
    {
        do //tant qu'il n'appuye pas sur enter
        {
            surligner_menu(y0, x-2, position);
            y0 = position; //pour garder la position précédente

            key = getkey(); //lecture de la touche au clavier

            if (key==1 && position!=9) //vers le haut
            {
                position = position-2;
            }
            else if (key==3 && position!=(9+2*nb_j)) //vers le bas
            {
                position += 2;
            }
        }while (key!=5);  //tant qu'il n'appuye pas sur enter

        if (position!=(9+2*nb_j)) //si il a choisi une des maisons
        {
            i = (position-9)/2; //le numéro de la maison sélectionnée
            gotoligcol(position, x-3); //pour écrire la valeur d'IA

            if(intel[i]==JOUEUR) //on inverse la valeur d'IA
            {
                intel[i] = FACILE;

                texte(39, i+1, langue, couleurp); //39) IA
            }
            else if(intel[i]==FACILE)
            {
                intel[i] = JOUEUR;

                texte(38, i+1, langue, couleurp); //38) PL
            }
        }

    } while (position!=(9+2*nb_j)); //tant que la position n'est pas "continuer"
}

//pour choisir quel fichier de sauvgarde ouvrir. Renvoie 0 si nouvelle partie
int choix_sauv(int langue)
{
    int key, position, y0, x, rep;

    effacer_tout(); //pour vider l'écran

    x = 8; //on garde cette position

    gotoligcol(1, 2);
    texte(33, 0, langue, NULL); //33) Utilisez les flèches du clavier pour déplacer le curseur et ENTRÉE pour selectionner

    gotoligcol(6, x-6);
    texte(23, 1, langue, NULL); //23) Voulez vous charger une partie sauvegardée?

    for (int i=1;i<=3;i++) //pour afficher la possibilité de chaque sauvegarde
    {
        gotoligcol(7+(2*i), x);
        texte(41, i, langue, NULL); //41) Partie %d
    }

    gotoligcol(15, x);
    texte(42, 0, langue, NULL); //42) Nouvelle partie

    y0 = position = 9;
    //y0 est la position précédente du curseur (à effacer)

    do //pour déplacer le curseur
    {
        surligner_menu(y0, x-2, position);
        y0 = position; //pour prendre la position précédente du curseur

        key = getkey(); //prise d'une touche du clavier

        if (key==1 && position!=9) //pour aller vers le haut
        {
            position -= 2;
        }
        else if (key==3 && position!=15) //pour aller vers le bas
        {
            position += 2;
        }
    }while (key!=5); //appuye sur enter

    rep = (position-7)/2;

    if (rep==4)
    {
        rep = 0;
    }

    return rep;
}

//voulez vous prendre la partie enregistrée? appel du choix_jeu si non
void menu(int langue, int *nb_j, int *n, int *joueur, int xpions2[2][19], int ypions2[2][19], int xpions4[4][13], int ypions4[4][13], int intel2[2], int intel4[4])
{
    int enregist;

    FILE* fic = NULL;

    printf("\e[?25l"); //pour cacher le curseur pour le reste du jeu
    fflush(stdout);

    getch(); //parcequ'un enter a été appuyé avant ça

    enregist = choix_sauv(langue);

    if (enregist==0) //si il ne charge pas la partie enregistrée
    {
        choix_jeu1(langue, nb_j); //choix du nombre de maisons

        if (*nb_j==2)
        {
            choix_jeu2(langue, *nb_j, intel2); //choix des valeurs d'IA
        }
        else
        {
            choix_jeu2(langue, *nb_j, intel4); //choix des valeurs d'IA
        }

        *n = 25-3*(*nb_j); //le nombre de pions

        effacer_tout(); //vider l'écran

        if (*nb_j==2)
        {
            reset_pions(*nb_j, *n, xpions2, ypions2);
        }
        else if(*nb_j==4)
        {
            reset_pions(*nb_j, *n, xpions4, ypions4);
        }
        *joueur = 1; //au tour du joueur 1

    }
    else //si il decide de charger la partie enregistrée
    {
        switch (enregist)//ouverture du fichier
        {
            case 1:
            fic = fopen ("Partie1.txt", "r"); //ouverture en lecture
            break;
            case 2:
            fic = fopen ("Partie2.txt", "r"); //ouverture en lecture
            break;
            case 3:
            fic = fopen ("Partie3.txt", "r"); //ouverture en lecture
            break;
        }// accès en lecture au fichier et récupération de l’entier qui s’y trouve

        if (fic==NULL) exit(EXIT_FAILURE); //si on ne parvient pas à ouvrir le fichier le jeu s'arrete et renvoie une erreur


        ouvrir_partie1(joueur, nb_j, fic); //ouvrir à qui le tour et le nombre de joueurs

        *n = 25-3*(*nb_j); //le nombre de pions

        if (*nb_j==2) //ouvrir les ias et les coordonnées des pions
        {
            ouvrir_partie2(*nb_j, *n, xpions2, ypions2, intel2, fic);
        }
        else if (*nb_j==4)
        {
            ouvrir_partie2(*nb_j, *n, xpions4, ypions4, intel4, fic);
        }

        //fermeture du fichier
        fclose(fic);
    }
}

//fin du fichier
