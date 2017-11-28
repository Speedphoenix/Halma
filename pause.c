#include "Sous_programmes.h"

///chapitre 10 dans le poli

//pour enregistrer une partie, renvoie 1 si il ne s'est rien passé, 2 si la partie a été enregistrée
int enregistrer_partie(int joueur, int nb_j, int n, int langue, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]) /// nb_j ; x/ypions
{
    //declaration des ressources
    int i, j, sauv, y0, position, x, key, rep = 2;

    FILE* fic = NULL;

    ///pour choisir quel fichier de sauvegarde utiliser

    x = 37; //position juste à gauche du texte

    y0 = position = 20; //la position tout en haut
    //y0 est la position précédente du curseur (pour l'effacer quand on affiche la nouvelle)

    afficher_boite(); //on fait une boite noire de 36 par 15

    gotoligcol(18, 40);
    texte(43, 0, langue, NULL); //43) Quelle sauvegarde?

    for (i=1;i<=3;i++) //pour afficher la possibilité de chaque sauvegarde
    {
        gotoligcol(18+(2*i), x);
        texte(41, i, langue, NULL); //41) Partie %d
    }

    gotoligcol(26, x);
    texte(44, 0, langue, NULL); //44) Annuler

    do
    {
        surligner_menu(y0, x-2, position); //afficher le curseur
        y0 = position;

        key = getkey(); //lecture sans enter d'une touche du clavier

        if (key==1 && position!=20) //vers le haut
        {
            position -= 2;
        }
        else if (key==3 && position!=26) //vers le bas
        {
            position += 2;
        }
    }while (key!=5); //tant qu'il appuye pas sur enter

    sauv = (position-18)/2;

    ///fin du choix du fichier de sauvegarde

    if (sauv==4)
    {
        rep = 1;
    }
    else
    {
        switch (sauv)//ouverture du fichier
        {
            case 1:
            fic = fopen ("Partie1.txt", "w"); //ouverture en écriture
            break;
            case 2:
            fic = fopen ("Partie2.txt", "w"); //ouverture en écriture
            break;
            case 3:
            fic = fopen ("Partie3.txt", "w"); //ouverture en écriture
            break;
        }// accès en lecture au fichier et récupération de l’entier qui s’y trouve


        if (fic==NULL) exit(EXIT_FAILURE); //si on ne parvient pas à ouvrir le fichier le jeu s'arrete et renvoie une erreur

        fprintf(fic, "%d,", nb_j); //écrire le nombre de joueurs
        fprintf(fic, "%d,\n", joueur); //à qui le tour? (on veut reprendre le tour du J2 si c'était le tour du J2 lors de la sauvegarde)

        for (i=0;i<nb_j;i++)
        {
            fprintf(fic, "%d,", intel[i]); //pour avoir pour chaque joueur si c'est un joueur ou une IA (et son niveau)
        }
        fprintf(fic, "\n");

        for (i=0;i<nb_j;i++)
        {
            for (j=0;j<n;j++)
            {
                fprintf(fic, "%d,%d,", xpions[i][j], ypions[i][j]); //on écrit les coordonnées de chaque pion de chaque joueur
            }
            fprintf(fic, "\n");
        }

        //fermeture du fichier
        fclose(fic);
    }

    return rep;
}

//pour lire le nombre de joueurs et le tour (à quel joueur)
void ouvrir_partie1(int *joueur, int *nb_j, FILE* fic) ///nb_j ; x/ypions
{

    fscanf(fic, "%d,", nb_j); //lire le nombre de joueurs
    fscanf(fic, "%d,", joueur); //lire le tour de quel joueur?

}

//pour lire la valeur d'IA de chaque joueur (joueur ou Niveau de l'ia) et les coordonnées de chaque pion
void ouvrir_partie2(int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j], FILE* fic)
{
    int i, j;

    fseek(fic, 5, SEEK_SET); //le premier est nb_j, le deuxième une virgule, le troisième le numéro du joueur, le quatrième une virgule et le 5ème un \n

    for (i=0;i<nb_j;i++)
    {
        fscanf(fic, "%d,", (intel + i)); //pour chaque joueur si c'est une IA ou un humain...
    }

    for (i=0;i<nb_j;i++) //lire les coordonnées de chaque pion
    {
        for (j=0;j<n;j++)
        {
            fscanf(fic, "%d,", &xpions[i][j]);
            fscanf(fic, "%d,", &ypions[i][j]);
        }
        fscanf(fic, "\n");
    }
}

void pause_game(int joueur, int langue, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j]) ///enregistrer_partie
{///choix deplacement
    int reprise = 1, val; //pour avoir les valeurs des choix etc...
    int x, position, y0, key; //pour le menu/déplacement du curseur et affichage

    y0 = position = 20; //la position tout en haut
    //y0 est la position précédente du curseur (pour l'effacer quand on affiche la nouvelle

    do
    {
        x = 37; //position juste à gauche du texte

        afficher_boite(); //on fait une boite noire de 36 par 15

        if (reprise==2) //si l'utilisateur a décidé d'enregistrer ça passe par là
        {
            gotoligcol(29, 37);
            texte(27, joueur, langue, NULL); //27) ***Partie enregistrée***
        }

        gotoligcol(18, 43);
        texte(24, joueur, langue, NULL); //24) ***Pause***

        gotoligcol(20, x);
        texte(25, joueur, langue, NULL); //25)  - Reprendre la partie

        gotoligcol(22, x);
        texte(26, joueur, langue, NULL); //26)  - Sauvegarder la partie

        gotoligcol(24, x);
        texte(28, joueur, langue, NULL); //28)  - Quitter le jeu

        reprise = 1; //discutablement inutile

        do
        {
            surligner_menu(y0, x-2, position); //afficher le curseur
            y0 = position;

            key = getkey(); //lecture sans enter d'une touche du clavier

            if (key==1 && position!=20) //vers le haut
            {
                position -= 2;
            }
            else if (key==3 && position!=24) //vers le bas
            {
                position += 2;
            }
        }while (key!=5); //tant qu'il appuye pas sur enter

        val = (position-18)/2; //on voit en quelle position le curseur est

        switch (val)
        {
            case 1: //reprendre le jeu
            reprise = 0;
            break;

            case 2: //enregistrer la partie
            reprise = enregistrer_partie(joueur, nb_j, n, langue, xpions, ypions, intel);
            break;

            case 3: //quitter le jeu
            gotoligcol(51,0);
            exit(0);
            break;

            default: //discutablement inutile
            reprise = 1;
        }
    } while (reprise!=0); //reprise est 0 si on demande à reprendre
}

//arrete le programme et met en pause si on a appuyé sur p, ne fait rien sinon
int pause_entre(int joueur, int langue, int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j])
{
    int rep;
    rep = kbhit();

    if (rep) //kbhit revoie 1 si une touche du clavier a été appuyée, 0 sinon
    {
        if (getch()=='p') //si il a appuyé sur p
        {
            pause_game(joueur, langue, nb_j, n, xpions, ypions, intel); //mettre en pause
        }
    }
    fflush(stdin); //pour enlever toutes les touches de trop qui ont été appuyées

    return rep;
}

//fin du fichier
