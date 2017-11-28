#include "Sous_programmes.h"

///CES DEUX SOUS PROGRAMMES FONT PARTIE DE WINDOWS.H (pray for linux)
/* reads from keypress, doesn't echo */ /// pris depuis http://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux
char getch(void)
{
    struct termios oldattr, newattr;
    char ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* renvoie 1 si une touche a été appuyée, 0 sinon */ ///pris depuis campus.ece.fr  MERCI À https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// renvoie 1 si l'utilisateur a choisi vers le haut, 2 vers la gauche, 3 vers le bas, 4 vers la droite, 5 pour selectioner et 6 pour p
int getkey(void)
{
    int rep;
    char lis;
    do
    {
        lis = getch(); //on prend la touche clavier
        switch (lis)
        {
            case'A': //flèche du haut
            rep = 1;
            break;
            case 'D': //flèche de gauche
            rep = 2;
            break;
            case 'B': //flèche du bas
            rep = 3;
            break;
            case 'C': //flèche de droite
            rep = 4;
            break;
            case '\n': //enter
            rep = 5;
            break;
            case 'p': //p (pour mettre en pause
            rep = 6;
            break;
            default:
            rep = 0;
            break;
        }
    }while (rep==0); //on ne s'arrete pas tant que l'utilisateur n'a pas entré un touche correcte

    return rep;
}

//pour selectionner un pion et le déplacer avec le curseur
void choix_deplacement(int joueur, int *choix, int *xdestination, int *ydestination, int langue, char *couleurp[4], int nb_j, int n, int xpions[nb_j][n], int ypions[nb_j][n], int intel[nb_j])
{///nb_j ; pause_game ; afficher_damier ; quel_pion ; pion_possible ; coordonnees_dry ; afficher_deplacements ; mouvements ;
    int xposition, yposition, tant, key, change;
    int a_x, a_y, xchoix, ychoix;
    int deplacements[N][N];
    //a_x/y estla position précédente du curseur

    //pour la position initiale du  curseur
    a_x = xposition = cotex(joueur-1, 1, nb_j);
    a_y = yposition = cotey(joueur-1, 1, nb_j);

    do
    {
        gotoligcol(50, 0);
        texte(9, joueur, langue, couleurp); //9) c'est au joueur x de jouer
        change = 0;
        tant = -1;
        do //choix du pion
        {
            do
            {
                afficher_curseur(joueur, a_x, a_y, xposition, yposition, couleurp); //on affiche le curseur

                a_x = xposition; //on garde la position précédente du curseur
                a_y = yposition;

                key = getkey();

                if (key==6) //appuye sur p
                {
                    pause_game(joueur, langue, nb_j, n, xpions, ypions, intel); //mettre le jeu en pause
                    afficher_damier(couleurp, nb_j, n, xpions, ypions);
                    texte(9, joueur, langue, couleurp); //9) c'est au joueur x de jouer
                }
                else if (key==2 && xposition!=1) //vers la gauche
                {
                    xposition--;
                }
                else if (key==4 && xposition!=N) //vers la droite
                {
                    xposition++;
                }
                else if (key==1 && yposition!=N) //vers le haut
                {
                    yposition++;
                }
                else if (key==3 && yposition!=1) //vers le bas
                {
                    yposition--;
                }
            }while (key!=5); //tant que l'utilisateur n'appuye pas sur enter

            tant = quel_pion(xposition, yposition, joueur, nb_j, n, xpions, ypions); //on regarde quel pion

            if (tant!=-1 && pion_possible(tant, joueur, nb_j, n, xpions, ypions)==0) //si le pion ne peut pas être déplacé
            {
                tant = -1;
            }
        } while (tant==-1); //il ne s'est rien passé, il faut séléctionner un autre pion

        *choix = tant;

        coordonnees_dry(&xchoix, &ychoix, joueur, *choix, nb_j, n, xpions, ypions); //pour travailler avec juste xchoix et ychoix au lieu de x/ypions[][]...

        //fin du choix du pion

        afficher_deplacements(joueur, *choix, couleurp, nb_j, n, xpions, ypions); //afficher les déplacements possibles

        a_x = xposition;
        a_y = yposition;

        mouvements(deplacements, joueur, *choix, nb_j, n, xpions, ypions, NULL); //on regarde tous les déplacements possibles (dans deplacements[16][16]

        gotoligcol(50, 0); //ça s'était éffacé avec des affichages...
        texte(9, joueur, langue, couleurp); //9) c'est au joueur x de jouer

        do//choix de la case d'arrivée
        {
            do //comme pour la selection du pion
            {
                afficher_curseur(joueur, a_x, a_y, xposition, yposition, couleurp);
                a_x = xposition;
                a_y = yposition;

                key = getkey();

                if (key==6)
                {
                    pause_game(joueur, langue, nb_j, n, xpions, ypions, intel);
                    afficher_deplacements(joueur, *choix, couleurp, nb_j, n, xpions, ypions);
                    texte(9, joueur, langue, couleurp); //9) c'est au joueur x de jouer
                }
                else if (key==2 && xposition!=1)
                {
                    xposition--;
                }
                else if (key==4 && xposition!=N)
                {
                    xposition++;
                }
                else if (key==1 && yposition!=N)
                {
                    yposition++;
                }
                else if (key==3 && yposition!=1)
                {
                    yposition--;
                }
            }while (key!=5); //tnt que l'utilisateur n'appuye pas sur enter

            if ((xposition==xchoix) && (yposition==ychoix)) //si l'utilisateur décide de choisir un autre pion
            {
                change = 1; //pour rester dans la boucle générale (et revenir au choix du pion)
                tant = 1; //pour sortir de la boucle de choix d'arrivée
                afficher_damier(couleurp, nb_j, n, xpions, ypions);
                afficher_curseur(joueur, a_x, a_y, xposition, yposition, couleurp);
            }
            else
            {
                tant = deplacements[xposition-1][yposition-1]; //si le déplacement n'est pas possible alors tant prend la valeur 0
            }
        } while (tant==0);
        //fin du choix de la case d'arrivée

    } while (change);


    *xdestination = xposition; //pour garder les nouvelles coordonnées?
    *ydestination = yposition;
}

//fin du fichier

